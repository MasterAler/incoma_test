#ifndef REQUESTSERVER_H
#define REQUESTSERVER_H

#include <memory>
#include <QObject>
#include <QTcpServer>

class RequestHandler;

/*!
 * \brief The RequestServer class -- думаю, очевидно
 *
 * Отдельно прокомментирую, что в кажестве улучшения можно было бы
 * сделать независимо потоки соединения и логики... или даже гораздо лучше входящие
 * подключения обрабатывать в цикле обработки сообщений (1 поток), а через сигналы взаимодействовать
 * с пулом потоков, в котором уже происходило бы что-то сложное. Общение с базой я сделал бы через QtConcurrent::run,
 * пока хватало бы производительности.
 */
class RequestServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RequestServer(QObject* parent = nullptr);

    // потому что лучше, чем замусоривать всё лишними абстракциями
    void setHandler(std::shared_ptr<RequestHandler> handler);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    std::shared_ptr<RequestHandler> m_handler;
};

#endif // REQUESTSERVER_H
