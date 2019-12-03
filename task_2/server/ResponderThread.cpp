#include "ResponderThread.h"

#include <QEventLoop>
#include <QDataStream>

#include <Logger.h>
#include <Message.h>

#include "RequestHandler.h"

ResponderThread::ResponderThread(int socketDescriptor, QObject *parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
{ }

void ResponderThread::setHandler(std::shared_ptr<RequestHandler> handler)
{
    m_handler = handler;
}

void ResponderThread::run()
{
    QTcpSocket serverSocket;
    QEventLoop socketLoop;

    QObject::connect(&serverSocket, &QAbstractSocket::disconnected, [this, &socketLoop]
    {
        LOG_INFO(QString{"\n\n Container contains %1 records \n\n"}.arg(m_handler->getContainerCount()));
        socketLoop.quit();
    });

    QObject::connect(&serverSocket
                     , QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error)
                     , [&](QAbstractSocket::SocketError /*socketError*/)
    {
        LOG_ERROR(serverSocket.errorString());
        serverSocket.disconnectFromHost();
    });

    QDataStream readingStream;
    readingStream.setDevice(&serverSocket);
    readingStream.setVersion(QDataStream::Qt_5_0);

    QObject::connect(&serverSocket, &QAbstractSocket::readyRead, [&]
    {
        LOG_DEBUG_TIME("= REQUEST HANDLING =");

        readingStream.startTransaction();

        QByteArray request;
        readingStream >> request;

        if (!readingStream.commitTransaction())
        {
            LOG_ERROR("Something went wrong with request");
            serverSocket.disconnectFromHost();
        }
        else
        {
            LOG_TRACE(QString{"Request: %1"}.arg(QString::fromUtf8(request)));
            QVariantMap responseData = m_handler->handleRequest(Message::decodeMessage(request));
            serverSocket.write(Message::encodeMessage(responseData));
        }
    });

    if (!serverSocket.setSocketDescriptor(m_socketDescriptor))
    {
        LOG_ERROR(serverSocket.errorString());
        return;
    }

    socketLoop.exec();
}
