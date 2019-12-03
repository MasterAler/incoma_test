#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QVariant>
#include <QTcpSocket>

class RequestPrivate;

class Request : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Request)

public:
    explicit Request(QObject *parent = nullptr);
    ~Request();

    void setConnectionParams(const QString& serverAddr, quint16 port);

    void send(const QVariantMap& messageData);

public slots:
    void closeConnection();

signals:
    void responseReceived(const QByteArray& response);
    void error(const QString& errMsg);
    void critical(const QString& errMsg);

private:
    QScopedPointer<RequestPrivate> d_ptr;
};

#endif // REQUEST_H
