#ifndef CLIENTRANDOMREQUESTGENERATOR_H
#define CLIENTRANDOMREQUESTGENERATOR_H

#include <QVariantMap>

class ClientRandomRequestGenerator
{
public:
    QVariantMap createRandomRequest();

    void setKnownKeys(const QStringList& keys);

    static QVariantMap createKeyListRequest();

private:
    QStringList m_knownKeys;
};


#endif // CLIENTRANDOMREQUESTGENERATOR_H