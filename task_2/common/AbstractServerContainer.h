#ifndef ABSTRACTSERVERCONTAINER_H
#define ABSTRACTSERVERCONTAINER_H

#include <QVariant>

/*!
 * \brief The AbstractServerContainer class это
 * базоый класс для хитрого серверного контейнера.
 *
 * !ВНИМАНИЕ! В коде что-то люто выделяется на куче через умные указатели и т.д.
 * Это, а особенно данный базовый класс нужны затем и только затем, чтобы можно было
 * попробовать разные контейнеры для сравнения. В реальности редко зачем-то нужно
 * в рантайме выбирать такие вещи.
 *
 * Считаем, что интерфейс класса описывает то, что я поддержал:
 */
class AbstractServerContainer
{
public:
    virtual ~AbstractServerContainer() {}

    // получение ключей (см. клиент, зачем оно)
    virtual QStringList getKeys() const = 0;

    // запись
    virtual void setSomeValue(const QString& key, const QVariant& value) = 0;

    // чтение
    virtual QVariant getSomeValue(const QString& key) const = 0;
};

#endif // ABSTRACTSERVERCONTAINER_H
