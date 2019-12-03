#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include <QJsonDocument>

class Message
{
    Q_GADGET
public:
    enum MessageType
    {
        GET_KEYS,
        GET_VALUE,
        SET_VALUE
    };
    Q_ENUM(MessageType)

    static QByteArray encodeCommand(const QVariantMap& params)
    {
        QByteArray result;
        QDataStream out(&result, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);

        out << QJsonDocument::fromVariant(params).toJson();
        return result;
    }

    static QVariantMap decodeResponse(const QByteArray& reponse)
    {
        return QJsonDocument::fromJson(reponse).toVariant().toMap();
    }
};

#endif
