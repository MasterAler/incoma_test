#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <memory>
#include "AbstractServerContainer.h"

/*!
 * \brief The RequestHandler class это прослойка-контроллер, в которой
 * содержится логика ответов сервера.
 * Им все владеют: RequestServer, RequestThread, я счёл излишним плодить
 * дополнительные абстракции, ибо это легко рефакторится.
 *
 * Тип контейнера передаётся в конструктор.
 */
class RequestHandler
{
public:
    explicit RequestHandler(std::shared_ptr<AbstractServerContainer> container);

    // обрабатывает запрос, вся логика тут
    QVariantMap handleRequest(const QVariantMap& requestParams);

    // штука нужна скорее для отладки/демонстрации, чтобы куда-то
    // писать, сколько там элементов в контейнере
    int getContainerCount() const;

private:
    std::shared_ptr<AbstractServerContainer> m_container;
};

#endif // REQUESTHANDLER_H
