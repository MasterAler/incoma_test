#ifndef SIMPLESERVERCONTAINER_H
#define SIMPLESERVERCONTAINER_H

#include "AbstractServerContainer.h"
#include <QReadWriteLock>

/*!
 * \brief The SimpleServerContainer class  демонстрирует
 * самый простой способ сделать многопоточный доступ к контейнеру:
 * 1) берём RW-lock
 * 2) задача решена
 *
 * Минусы: большая гранулярность блокировки, на CppRussia 2019, например,
 * докладчик только ради этого заменил подобное решение самодельным HashMap постоянной длины
 * с блокировками на каждую ячейку (Bucket), тут можно выбрать один из кучи
 * подходов к оптимизации, но в основном все делают мелкогранулярные блокировки, а более сложные решения
 * ищут уже в реализованном виде.
 */
class SimpleServerContainer: public AbstractServerContainer
{
public:
    // ---
    QStringList getKeys() const override;

    // ---
    void setSomeValue(const QString& key, const QVariant& value) override;

    // ---
    QVariant getSomeValue(const QString& key) const override;

private:
    mutable QReadWriteLock m_lock;
    QVariantHash           m_serverData;
};

#endif // SIMPLESERVERCONTAINER_H
