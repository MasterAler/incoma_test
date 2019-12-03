#include "RequestServer.h"

#include <Logger.h>

#include "ResponderThread.h"
#include "RequestHandler.h"

RequestServer::RequestServer(QObject* parent)
    : QTcpServer(parent)
{ }

void RequestServer::setHandler(std::shared_ptr<RequestHandler> handler)
{
    m_handler = handler;
}

void RequestServer::incomingConnection(qintptr socketDescriptor)
{
    ResponderThread* thread = new ResponderThread(socketDescriptor, this);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->setHandler(m_handler);
    thread->start();
}

