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
        QUrlQuery ParmReq;

        static const QString SSOAddress;
        static const QString ReponseType;
        static const QString RedirectURL;
        static const QString ClientID;
        static const QString SecretKey;

    public:

        //Значения типов контента
        enum TypeHead {
            TypeJson,       //JSON данные
            TypeURLENCODED  //Стандартные
        };

        //Тип доступа
        enum GrantType{
            Authorize,    //Авторизация
            RefreshToken  //Обновление токена доступа
        };

        //Тип второго параметра
        enum TypeParm{
            Code,   //Код
            Refresh //Обновление токена
        }

        ESI_manager();
        ~ESI_manager();

        void Set_sslConfig();
        void Set_Request();
        void Set_Query();
        void Set_QueryJson();
        void SendPost();
};

#endif // ESI_MANAGER_H
