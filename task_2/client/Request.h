#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QVariant>
#include <QTcpSocket>

class RequestPrivate;

/*!
 * \brief The Request class это та штука, которая делает запросы.
 * Дополнительно сюда напихан примерный замер среднего времени обработки запроса и
 * полезные сигналы. Соединение держится в установенном состоянии, чтобы не тратиться
 * на рукопожания TCP. Это не всегда правильно, но здесь сделано так, ибо и так сложно
 * увидеть разницу в производительности контейнера на сервера.
 *
 * Я не возился со 100% переподключением при ошибке соединения, это можно было бы добавить
 * при использовании данного класса или в его реализации.
 */
class Request : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Request)

public:
    explicit Request(QObject *parent = nullptr);
    ~Request();

    // очевидно
    void setConnectionParams(const QString& serverAddr, quint16 port);

    // очевидно
    void send(const QVariantMap& messageData);

    /*!
     * \brief getAverageTiming -- среднее время запроса в микросекундах
     * \return                 -- суммарное время / суммарное количество запросов
     */
    qint64 getAverageTiming() const;

public slots:
    // очевидно
    void closeConnection();

signals:
    // асинхронное получение ответа
    void responseReceived(const QByteArray& response);

    void error(const QString& errMsg);
    void critical();

private:
    QScopedPointer<RequestPrivate> d_ptr;
};

#endif // REQUEST_H
