#ifndef CUSTOMSERVERCONTAINER_H
#define CUSTOMSERVERCONTAINER_H

#include "AbstractServerContainer.h"
#include "StripedHashMap.h"

class CustomServerContainer: public AbstractServerContainer
{
public:
    QStringList getKeys() const override;

    void setSomeValue(const QString& key, const QVariant& value) override;

    QVariant getSomeValue(const QString& key) const override;

private:

    StripedHashMap<std::string, std::string, 4096> m_concurrentData;
};

#endif // CUSTOMSERVERCONTAINER_H
