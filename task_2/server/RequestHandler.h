#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <memory>
#include "AbstractServerContainer.h"

class RequestHandler
{
public:
    explicit RequestHandler(std::shared_ptr<AbstractServerContainer> container);

    QVariantMap handleRequest(const QVariantMap& requestParams);

    int getContainerCount() const;

private:
    std::shared_ptr<AbstractServerContainer> m_container;
};

#endif // REQUESTHANDLER_H
