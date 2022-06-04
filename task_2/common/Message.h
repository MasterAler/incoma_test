#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include <QJsonDocument>

/*!
 * \brief The Message class тут сделан
 * только ради Q_ENUM, чтобы это перечисление средствами
 * Qt превращать в строки. В идеале сюда можно все хелперы
 * сложить, как в namespace.
 */
class Message
{
    Q_GADGET
public:
    enum MessageType
    {
        GET_KEYS, /*!< получает список ключей */
        GET_VALUE, /*!< получает данные */
        SET_VALUE, /*!< добавляет/меняет данные */
        STATUS_MSG /*!< так приходит ответ об ошибке */
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
