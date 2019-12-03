#include "ClientRandomRequestGenerator.h"

#include <QRandomGenerator>
#include <Message.h>

QVariantMap ClientRandomRequestGenerator::createRandomRequest()
{
    QVariantMap result;
    int val = QRandomGenerator::global()->bounded(0, 100);
    int cmdPos = QRandomGenerator::global()->bounded(0, m_knownKeys.length());

    if (val > 90) // read
    {
        if (val < 20) // read, keys list
        {
            result["type"] = Message::GET_KEYS;
        }
        else // read, getter
        {
            result["type"] = Message::GET_VALUE;
            result["key"]  = m_knownKeys[cmdPos];
        }
    }
    else // write
    {
        if (val > 95) // write, new value
        {
            result["type"]  = Message::SET_VALUE;
            result["key"]   = QString{"new_key_%1"}.arg(QRandomGenerator::global()->generate());
            result["value"] = QRandomGenerator::global()->generate();
        }
        else // write, setter
        {
            result["type"]  = Message::SET_VALUE;
            result["key"]   = m_knownKeys[cmdPos];
            result["value"] = QRandomGenerator::global()->generate();
        }
    }

    return result;
}

void ClientRandomRequestGenerator::setKnownKeys(const QStringList& keys)
{
    m_knownKeys = keys;
}

QVariantMap ClientRandomRequestGenerator::createKeyListRequest()
{
    return { {"type", Message::GET_KEYS} };
}
