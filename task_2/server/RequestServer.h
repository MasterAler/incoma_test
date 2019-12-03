#ifndef REQUESTSERVER_H
#define REQUESTSERVER_H

#include <memory>
#include <QObject>
#include <QTcpServer>

class RequestHandler;

class RequestServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RequestServer(QObject* parent = nullptr);

    void setHandler(std::shared_ptr<RequestHandler> handler);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    std::shared_ptr<RequestHandler> m_handler;
};

#endif // REQUESTSERVER_H
