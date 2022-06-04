#include "CustomServerContainer.h"

#include <list>

#include <QJsonDocument>

QStringList CustomServerContainer::getKeys() const
{
    QStringList result;
    auto currentKeys = m_concurrentData.keys();

    for(auto key : currentKeys)
        result << QString::fromStdString(key);

    return result;
}

void CustomServerContainer::setSomeValue(const QString& key, const QVariant& value)
{
    const std::string data_key = key.toStdString();
    if (m_concurrentData.contains(data_key))
        m_concurrentData.replace(std::move(data_key), value.toString().toStdString());
    else
        m_concurrentData.insert(std::move(data_key), value.toString().toStdString());
}

QVariant CustomServerContainer::getSomeValue(const QString& key) const
{
    const std::string data_key = key.toStdString();
    return QString::fromStdString(m_concurrentData.value(data_key));
}
