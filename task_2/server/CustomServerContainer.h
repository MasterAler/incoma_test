#ifndef CUSTOMSERVERCONTAINER_H
#define CUSTOMSERVERCONTAINER_H

#include "AbstractServerContainer.h"
#include "StripedHashMap.h"

/*!
 * \brief The CustomServerContainer class это такой
 * способ, несмотря на вот это вот "обёртка над обёрткой", продемонстрировать
 * старательно написанный велосип^w^w оптимизи^w^w _кастомный_ класс хранения данных
 */
class CustomServerContainer: public AbstractServerContainer
{
public:
    QStringList getKeys() const override;

    void setSomeValue(const QString& key, const QVariant& value) override;

    QVariant getSomeValue(const QString& key) const override;

private:
    // да, я читер, 4096 это условное "тяжело быстро достижимое число", чтобы
    // перехеширование как можно дольше не наступало, можно и побольше вписать
    StripedHashMap<std::string, std::string, 4096> m_concurrentData;
};

#endif // CUSTOMSERVERCONTAINER_H
