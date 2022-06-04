#include "ClientRandomRequestGenerator.h"

#include <QRandomGenerator>
#include <Message.h>

QVariantMap ClientRandomRequestGenerator::createRandomRequest()
{
    QVariantMap result;
    int val = !keyListIsEmpty() ? QRandomGenerator::global()->bounded(0, 100) : 100;

    if (val < 90) // read
    {
        if (val < 20) // read, keys list
        {
            result[Message::TYPE_KEY] = QVariant::fromValue(Message::GET_KEYS);
        }
        else // read, getter
        {
            int cmdPos = QRandomGenerator::global()->bounded(0, m_knownKeys.length());
            result[Message::TYPE_KEY] = QVariant::fromValue(Message::GET_VALUE);
            result[Message::DICT_KEY] = m_knownKeys[cmdPos];
        }
    }
    else // write
    {
        if (val > 95) // write, new value
        {
            result[Message::TYPE_KEY]  = QVariant::fromValue(Message::SET_VALUE);
            result[Message::DICT_KEY]  = QString{"new_key_%1"}.arg(QRandomGenerator::global()->generate());
            result[Message::VALUE_KEY] = QRandomGenerator::global()->generate();

            m_knownKeys << result[Message::DICT_KEY].toString();
        }
        else // write, setter
        {
            int cmdPos = QRandomGenerator::global()->bounded(0, m_knownKeys.length());
            result[Message::TYPE_KEY]  = QVariant::fromValue(Message::SET_VALUE);
            result[Message::DICT_KEY]  = m_knownKeys[cmdPos];
            result[Message::VALUE_KEY] = QRandomGenerator::global()->generate();
        }
    }

    return result;
}

void ClientRandomRequestGenerator::setKnownKeys(const QStringList& keys)
{
    m_knownKeys = keys;
}

bool ClientRandomRequestGenerator::keyListIsEmpty() const
{
    return m_knownKeys.isEmpty();
}

QVariantMap ClientRandomRequestGenerator::createKeyListRequest()
{
    return { {Message::TYPE_KEY, Message::GET_KEYS} };
}
