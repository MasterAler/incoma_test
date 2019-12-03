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
        SET_VALUE,
        STATUS_MSG
    };
    Q_ENUM(MessageType)

    static QByteArray encodeMessage(const QVariantMap& params)
    {
        QByteArray result;
        QDataStream out(&result, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);

        out << QJsonDocument::fromVariant(params).toJson(QJsonDocument::Compact);
        return result;
    }

    static QVariantMap decodeMessage(const QByteArray& message)
    {
        return QJsonDocument::fromJson(message).toVariant().toMap();
    }

    static const QString TYPE_KEY;
    static const QString DATA_KEY;
    static const QString DICT_KEY;
    static const QString VALUE_KEY;
};

#endif
