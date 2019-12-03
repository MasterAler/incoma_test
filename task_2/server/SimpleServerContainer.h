#ifndef SIMPLESERVERCONTAINER_H
#define SIMPLESERVERCONTAINER_H

#include "AbstractServerContainer.h"
#include <QReadWriteLock>

class SimpleServerContainer: public AbstractServerContainer
{
public:
    SimpleServerContainer();

    QStringList getKeys() const override;

    void setSomeValue(const QString& key, const QVariant& value) override;

    QVariant getSomeValue(const QString& key) const override;

private:
    mutable QReadWriteLock m_lock;
    QVariantHash           m_serverData;
};

#endif // SIMPLESERVERCONTAINER_H
