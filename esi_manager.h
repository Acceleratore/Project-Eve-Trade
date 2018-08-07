#ifndef ESI_MANAGER_H
#define ESI_MANAGER_H

#include <QObject>
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
#include <QUrlQuery>
#include "logger.h"

class ESI_manager : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* Net_Manager = nullptr;
    QSslConfiguration sslConfig;
    QNetworkRequest Request;
    QUrlQuery ParmReq;

public:

    static const QString SSOAddress;
    static const QString ReponseType;
    static const QString RedirectURL;
    static const QString ClientID;
    static const QString SecretKey;
    static const QString VerifuAddress;
    static const QString ESIAddress;

    //Значения типов контента
    enum TypeHead {
        TypeJson,       //JSON данные
        TypeURLENCODED  //Стандартные
    };

    //Тип доступа
    enum GrantType {
        Authorize,     //Авторизация
        RefreshToken  //Обновление токена доступа
    };

    //Тип второго параметра
    enum TypeParm{
        Code,   //Код
        Refresh //Обновление токена
    };

    //ESI_manager();
    explicit ESI_manager(QObject *parent = nullptr);
    ~ESI_manager();

    void Set_sslConfig(QSsl::SslProtocol protocol);
    void Set_Request(ESI_manager::TypeHead Type);                    //Для авторизации по SSO
    void Set_Request(QString Host, QString TypeAuth, QString Token); //Для отправки запроса с авторизацией, но без выбора типа возвращаемых данных
    void Set_Query(ESI_manager::GrantType Type, ESI_manager::TypeParm Parm1, QString Value1);
    void Set_QueryJson();
    void SendPost();
    void SendGet();

public slots:
    void slotError(QNetworkReply::NetworkError tcode);
    void slotSSLError(QList<QSslError> ListError);
    void slotSSLErrorManeger(QNetworkReply*, QList<QSslError>);
    void GetResponse(QNetworkReply *reply);
signals:
    void ReturnData(QByteArray tBArr);
};

#endif // ESI_MANAGER_H
