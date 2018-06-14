#ifndef ESI_MANAGER_H
#define ESI_MANAGER_H

#include <QSslCertificate>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslConfiguration>
#include <QList>
#include <QSslError>
#include "logger.h"

class ESI_manager
{
    private:
        QNetworkAccessManager* Net_Manager = 0;
        QSslConfiguration sslConfig;
        QNetworkRequest Request;

        static const QString SSOAddress;
        static const QString ReponseType;
        static const QString RedirectURL;
        static const QString ClientID;
        static const QString SecretKey;

    public:

        enum TypeHead {
            TypeJson,
            TypeURLENCODED
        };

        ESI_manager();
        ~ESI_manager();

        void Set_sslConfig();
        void Set_Request();
        void Set_Query();
        void Set_QueryJson();
        void SendPost();
};

#endif // ESI_MANAGER_H
