#ifndef RESPONDERTHREAD_H
#define RESPONDERTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ResponderThread : public QThread
{
    Q_OBJECT
public:
    explicit ResponderThread(int socketDescriptor, QObject *parent = nullptr);

    void run() override;

signals:
    void error(QAbstractSocket::SocketError socketError);

private:
    int m_socketDescriptor;
};

#endif // RESPONDERTHREAD_H
