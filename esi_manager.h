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
    QNetworkAccessManager* Net_Manager = nullptr;
    QNetworkReply *reply = nullptr;
    QSslConfiguration sslConfig;
    QNetworkRequest Request;
    QUrlQuery ParmReq;

public:

    static const QString SSOAddress;
    static const QString ReponseType;
    static const QString RedirectURL;
    static const QString ClientID;
    static const QString SecretKey;

    //Значения типов контента
    enum TypeHead {
        TypeJson,       //JSON данные
        TypeURLENCODED  //Стандартные
    };

    //Тип доступа
    enum GrantType {
        Authorize,     //Авторизация
        Refresh Token  //Обновление токена доступа
    };

    //Тип второго параметра
    enum TypeParm{
        Code,   //Код
        Refresh //Обновление токена
    }

    ESI_manager();
    ~ESI_manager();

    void Set_sslConfig(QSsl::SslProtocol protocol);
    void Set_Request(ESI_manager::TypeHead Type);
    void Set_Query(ESI_manager::GrantType Type, ESI_manager::TypeParm Parm1, QString Value1);
    void Set_QueryJson();
    void SendPost();


public slots:
    void slotError(QNetworkReply::NetworkError tcode);
    void slotSSLError(QList<QSslError> ListError);
    void slotSSLError2(QNetworkReply*, QList<QSslError>);
    void GetResponse(QNetworkReply *reply);
};

#endif // ESI_MANAGER_H
