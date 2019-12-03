#include "ResponderThread.h"

ResponderThread::ResponderThread(int socketDescriptor, QObject *parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
{ }

void ResponderThread::run()
{
    QTcpSocket tcpSocket;
    QObject::connect(&tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error)
                     , this, &ResponderThread::error, Qt::QueuedConnection);

    if (!tcpSocket.setSocketDescriptor(m_socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    tcpSocket.waitForReadyRead();
    QByteArray request = tcpSocket.readAll();

    tcpSocket.write(request);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
