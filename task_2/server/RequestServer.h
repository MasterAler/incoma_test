#ifndef REQUESTSERVER_H
#define REQUESTSERVER_H

#include <QObject>
#include <QTcpServer>

class RequestServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RequestServer(QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // REQUESTSERVER_H
