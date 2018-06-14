#include "esi_manager.h"

//Конструктор
ESI_manager::ESI_manager()
{
    Net_Manager = new QNetworkAccessManager( this );

    connect(Net_Manager, SIGNAL(finished(QNetworkReply*)) , this, SLOT(GetResponse(QNetworkReply*)));
    connect(Net_Manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(slotSSLError2(QNetworkReply*,QList<QSslError>)));
}

//Деструктор (КЭП на страже)
ESI_manager::~ESI_manager()
{
    delete Net_Manager;
}


//Установка конфигурации SSL
void ESI_manager::Set_sslConfig(QSsl::SslProtocol protocol)
{

    //Настрока SSL конфигурации для POST
    this->sslConfig = QSslConfiguration::defaultConfiguration();
    this->sslConfig.setProtocol(protocol);
    this->sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    this->Request.setSslConfiguration(this->sslConfig);

    qDebug(logInfo) << "Установлен протокол SSL тунеля: " << QString::number(this->Request.sslConfiguration().protocol());
}

static QString ContentType(ESI_manager::TypeHead Type)
{
    switch (Type){
    case ESI_manager::TypeURLENCODED:
        return QString("application/x-www-form-urlencoded");
    case ESI_manager::TypeJson:
        return QString("application/json");
    }
}

//Установка параметров для запроса
void ESI_manager::Set_Request(ESI_manager::TypeHead Type)
{

    //Код авторизации
    QString AuthStr = QString(ClientID+":"+SecretKey).trimmed();
    QString tConType = ContentType(Type);

    //Установили данные загаловка
    Request.setUrl(QUrl( SSOAddress+"/token" ));
    Request.setRawHeader("Authorization", QString("Basic "+AuthStr.toLatin1().toBase64()).toUtf8());
    Request.setHeader( QNetworkRequest::ContentTypeHeader, tConType );


    qDebug(logDebug()) << "Формирование POST сообщения с параметрами: ";
    qDebug(logDebug()) << "HOST: " << QUrl( SSOAddress+"/token" ).toString();
    qDebug(logDebug()) << "Строка авторизации в BASE64: " << QString("Basic "+AuthStr.toLatin1().toBase64()).toUtf8();
    qDebug(logDebug()) << "ContentTypeHeader:" << Request.header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString();
    qDebug(logDebug()) << "UserAgentHeader:" << Request.header(QNetworkRequest::KnownHeaders::UserAgentHeader).toString();
    qDebug(logDebug()) << "Authorization: " << Request.rawHeader("Authorization");

}

void Set_Query()
{

}
