#ifndef RESPONDERTHREAD_H
#define RESPONDERTHREAD_H

#include <QThread>
#include <QTcpSocket>

class RequestHandler;

/*!
 * \brief The ResponderThread class -- это
 * поток, обрабатывающий соединение с одним клиентом. Очень-очень не люблю наследовать потоки, но...
 * "Note: If you want to handle an incoming connection as a new QTcpSocket object in another thread you have to pass the socketDescriptor
 * to the other thread and create the QTcpSocket object there and use its setSocketDescriptor() method."
 */
class ResponderThread : public QThread
{
    Q_OBJECT
public:
    explicit ResponderThread(int socketDescriptor, QObject *parent = nullptr);

    // потому что лучше, чем замусоривать всё лишними абстракциями
    void setHandler(std::shared_ptr<RequestHandler> handler);

    // тут вся логика соединения
    void run() override;

private:
    int                             m_socketDescriptor;
    std::shared_ptr<RequestHandler> m_handler;
};

#endif // RESPONDERTHREAD_H
