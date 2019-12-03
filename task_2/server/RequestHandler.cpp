#include "RequestHandler.h"

#include <Logger.h>
#include <Message.h>

RequestHandler::RequestHandler(std::shared_ptr<AbstractServerContainer> container)
    : m_container(container)
{ }

QVariantMap RequestHandler::handleRequest(const QVariantMap& requestParams)
{
    QVariantMap responseParams;

    if (!requestParams.contains(Message::TYPE_KEY))
    {
        responseParams[Message::TYPE_KEY] = QVariant::fromValue(Message::STATUS_MSG);
        responseParams[Message::DATA_KEY] = "BAD COMMAND";
    }
    else
    {
        responseParams[Message::TYPE_KEY] = requestParams[Message::TYPE_KEY];
        Message::MessageType reqType = requestParams[Message::TYPE_KEY].value<Message::MessageType>();

        switch (reqType)
        {
        case Message::GET_KEYS:
        {
            LOG_TRACE_TIME("GET_KEYS");
            responseParams[Message::DATA_KEY] = m_container->getKeys();
            break;
        }
        case Message::GET_VALUE:
        {
            LOG_TRACE_TIME("GET_VALUE");
            responseParams[Message::DATA_KEY] = m_container->getSomeValue(
                                                        requestParams.value(Message::DICT_KEY).toString());
            break;
        }
        case Message::SET_VALUE:
        {
            LOG_TRACE_TIME("SET_VALUE");
            if (requestParams.contains(Message::VALUE_KEY))
            {
                m_container->setSomeValue(requestParams.value(Message::DICT_KEY).toString()
                                          , requestParams[Message::VALUE_KEY]);
                responseParams[Message::DATA_KEY] = true;
            }
            else
                responseParams[Message::DATA_KEY] = false;
            break;
        }
        default:
            LOG_WARNING("BAD REQUEST");
        }
    }

    return responseParams;
}

int RequestHandler::getContainerCount() const
{
    return m_container->getKeys().length();
}
