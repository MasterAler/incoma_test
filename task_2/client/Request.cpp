#include "Request.h"

#include <QElapsedTimer>

#include <Logger.h>
#include "Message.h"

class RequestPrivate
{
    Q_DECLARE_PUBLIC(Request)

    RequestPrivate(Request* ownerPtr)
        : q_ptr(ownerPtr)
        , m_clientSocket{new QTcpSocket(ownerPtr)}
    {
        // error handling
        QObject::connect(m_clientSocket
                         , QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error)
                         , [this](QAbstractSocket::SocketError /*socketError*/)
        {
            m_clientSocket->close();
            emit q_ptr->error(QString{"Socket error: %1"}.arg(m_clientSocket->errorString()));
        });

        // response reading
        QObject::connect(m_clientSocket, &QAbstractSocket::readyRead, [this]
        {
            m_readingStream.startTransaction();

            QByteArray response;
            m_readingStream >> response;

            if (!m_readingStream.commitTransaction())
                emit q_ptr->error("Something went wrong with response");
            else
            {
                qint64 timing = m_perfTimer.elapsed();
                m_avgTiming = m_avgTiming * m_reqCount + timing;
                ++m_reqCount;
                m_avgTiming /= m_reqCount;

                LOG_TRACE(QString{"Request timing: %1 ms"}.arg(timing));
                emit q_ptr->responseReceived(response);
            }
        });
    }

    Request * const         q_ptr;
    QTcpSocket*             m_clientSocket;
    QString                 m_addr;
    quint16                 m_port;

    QDataStream             m_readingStream;

    // benchmarking
    QElapsedTimer           m_perfTimer;
    qint64                  m_avgTiming = 0;
    qint64                  m_reqCount = 0;

private:
    bool connect()
    {
        m_clientSocket->connectToHost(m_addr, m_port);
        if (!m_clientSocket->waitForConnected())
        {
            LOG_ERROR(QString{"Could not connect to %1:%2 Cause: %3"}
                      .arg(m_addr)
                      .arg(m_port)
                      .arg(m_clientSocket->errorString()));
            emit q_ptr->critical();
            return false;
        }

        m_readingStream.setDevice(m_clientSocket);
        m_readingStream.setVersion(QDataStream::Qt_5_0);
        return true;
    }
};

/************************************************************************************************************/

Request::Request(QObject *parent)
    : QObject(parent)
    , d_ptr{new RequestPrivate(this)}
{ }

Request::~Request()
{ }

void Request::setConnectionParams(const QString &serverAddr, quint16 port)
{
    Q_D(Request);
    d->m_addr = serverAddr;
    d->m_port = port;
}

void Request::send(const QVariantMap& messageData)
{
    Q_D(Request);

    switch (d->m_clientSocket->state())
    {
    case QAbstractSocket::ConnectedState:
        break;

    case QAbstractSocket::UnconnectedState:
        if (!d->connect())
            return; //RET
        break;

    default:
        d->m_clientSocket->abort();
        if (!d->connect())
            return; //RET
    }

    d->m_perfTimer.restart();
    QByteArray message = Message::encodeMessage(messageData);
    d->m_clientSocket->write(message);
    LOG_TRACE(QString{"Request sent: %1"}.arg(QString::fromUtf8(message)));
}

qint64 Request::getAverageTiming() const
{
    Q_D(const Request);
    return d->m_avgTiming;
}

void Request::closeConnection()
{
    Q_D(Request);
    d->m_clientSocket->close();
}
