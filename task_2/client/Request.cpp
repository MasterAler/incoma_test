#include "Request.h"

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
            emit q_ptr->error(QString{"Socket error: %1"}.arg(m_clientSocket->errorString()));
            m_clientSocket->abort();
        });

        // response reading
        QObject::connect(m_clientSocket, &QAbstractSocket::readyRead, [this]
        {
            m_readingStream.startTransaction();

            QByteArray response;
            m_readingStream >> response;

            if (!m_readingStream.commitTransaction())
                emit q_ptr->error("Something went wrong with response");

            emit q_ptr->responseReceived(response);
        });

        m_readingStream.setDevice(m_clientSocket);
        m_readingStream.setVersion(QDataStream::Qt_5_0);
    }

    Request * const         q_ptr;
    QTcpSocket*             m_clientSocket;
    QString                 m_addr;
    quint16                 m_port;

    QDataStream             m_readingStream;

private:
    bool connect()
    {
        m_clientSocket->connectToHost(m_addr, m_port);
        if (!m_clientSocket->waitForConnected(500))
        {
            emit q_ptr->critical(QString{"Could not connect to %1:%2 Cause: %3"}
                                 .arg(m_addr)
                                 .arg(m_port)
                                 .arg(m_clientSocket->errorString()));
            return false;
        }
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

    d->m_clientSocket->write(Message::encodeCommand(messageData));
    LOG_INFO("Request sent");
}

void Request::closeConnection()
{
    Q_D(Request);
    d->m_clientSocket->close();
}
