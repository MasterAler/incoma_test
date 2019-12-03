#include "SimpleServerContainer.h"

#include <QReadLocker>
#include <QWriteLocker>

QStringList SimpleServerContainer::getKeys() const
{
    QReadLocker locker(&m_lock);
    return m_serverData.keys();
}

void SimpleServerContainer::setSomeValue(const QString& key, const QVariant& value)
{
    QWriteLocker locker(&m_lock);
    m_serverData[key] = value;
}

QVariant SimpleServerContainer::getSomeValue(const QString& key) const
{
    QReadLocker locker(&m_lock);
    return m_serverData.value(key);
}
