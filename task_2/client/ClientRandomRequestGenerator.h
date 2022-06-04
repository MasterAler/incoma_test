#ifndef CLIENTRANDOMREQUESTGENERATOR_H
#define CLIENTRANDOMREQUESTGENERATOR_H

#include <QVariantMap>

/*!
 * \brief The ClientRandomRequestGenerator class
 * это наколеночный класс, который определяет логику работы клиента, какие
 * запросы он шлёт.
 *
 * Вообще, планировалось продемонстрировать работу со словарём на сервере, потому что
 * кроме этого в голову пришло только взять где-то реализацию lock-free очереди и добавлять в
 * неё данные. И удалять, да. Но выглядит в коде не так эпично.
 *
 * Здесь используются небольшие костыли, исключительно в демонстрационных целях -- хотелось именно
 * через клиенты нагенерировать данные для удобства, потому есть запрос о списке ключей, потому же и нет
 * удаления -- это переусложнило бы демонстрацию, заставив писать много ненужной  логики. Потому добавление и
 * обновление выбираются независимо, хоть это и один вид запроса
 *
 * В общем, на основе рандома этот класс умеет готовить запрос к серверу. Чтения должно стать больше, согласно заданию,
 * именно когда наберётся адекватное количество ключей.
 *
 */
class ClientRandomRequestGenerator
{
public:
    QVariantMap createRandomRequest();

    void setKnownKeys(const QStringList& keys);

    bool keyListIsEmpty() const;

    static QVariantMap createKeyListRequest();

private:
    QStringList m_knownKeys;
};


#endif // CLIENTRANDOMREQUESTGENERATOR_H
