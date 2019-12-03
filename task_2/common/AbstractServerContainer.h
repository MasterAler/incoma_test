#ifndef ABSTRACTSERVERCONTAINER_H
#define ABSTRACTSERVERCONTAINER_H

#include <QVariant>

class AbstractServerContainer
{
public:
    virtual ~AbstractServerContainer() {}

    virtual QStringList getKeys() const = 0;

    virtual void setSomeValue(const QString& key, const QVariant& value) = 0;

    virtual QVariant getSomeValue(const QString& key) const = 0;
};

#endif // ABSTRACTSERVERCONTAINER_H
