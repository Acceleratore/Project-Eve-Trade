#include "esi_manager.h"

const QString ESI_manager::SSOAddress  = QString("https://login.eveonline.com/oauth");
const QString ESI_manager::ReponseType = QString("code");
const QString ESI_manager::RedirectURL = QString("http://localhost:755/");
const QString ESI_manager::ClientID    = QString("703adcbe76c94fd59679ae32a04d8329");
const QString ESI_manager::SecretKey   = QString("EdMrlKYJnMgq8eZIAxlHUWJDol3ikywjX25T6xCY");

//Конструктор
ESI_manager::ESI_manager()
{
    Net_Manager = new QNetworkAccessManager( this );

    connect(Net_Manager, SIGNAL(finished(QNetworkReply*)) , this, SLOT(GetResponse(QNetworkReply*)));
    connect(Net_Manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(slotSSLErrorManeger(QNetworkReply*,QList<QSslError>)));
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

//Возвращает тип контента по виду в перечисляемом массиве
static QString ValueContentType(ESI_manager::TypeHead Type)
{
    switch (Type){
    case ESI_manager::TypeURLENCODED:
        return QString("application/x-www-form-urlencoded");
    case ESI_manager::TypeJson:
        return QString("application/json");
    }
}

//Возвращает тип авторизации по виду в перечисляемом массиве
static QString ValueGrantType(ESI_manager::GrantType Type)
{
    switch(type){
    case ESI_manager::Authorize:
        return QString("authorization_code");
    case ESI_manager::RefreshToken:
        return QString("refresh_token");
    }
}

//Возвращает параметр
static QString ValueTypeParm(ESI_manager::TypeParm Type)
{
    switch(type){
    case ESI_manager::Code:
        return QString("code");
    case ESI_manager::Refresh:
        return QString("refresh_token");
    }
}

//Установка параметров для запроса
void ESI_manager::Set_Request(ESI_manager::TypeHead Type)
{

    //Код авторизации
    QString AuthStr = QString(ClientID+":"+SecretKey).trimmed();
    QString tConType = ValueContentType(Type);

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

//Заполнение тела запроса
//Type - значение параметра grant_type
//Parm1 и Value1 второй параметр запроса, тип запроса заполняется из списка, значение произвольное (обычно передается от SSO)
void ESI_manager::Set_Query(ESI_manager::GrantType Type, ESI_manager::TypeParm Parm1, QString Value1)
{
    ParmReq.addQueryItem("grant_type", ValueGrantType(Type));
    ParmReq.addQueryItem(ValueTypeParm(Parm1), Value1);
}

//Отправка POST запроса
void ESI_manager::SendPost()
{

    this->reply = this->Net_Manager->post(this->Request, this->ParmReq.toString().toUtf8());

    //Соединение сигнала ошибок с слотом обработки ошибки при отправке
    connect(this->reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(this->reply, SIGNAL(sslErrors(QList<QSslError>)),        this, SLOT(slotSSLError(QList<QSslError>)));

    qDebug(logDebug) << "Вернувшийся код запроса2: " << this->Request.attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
}


//Слот для приема номера ошибки при отправке POST сообщения
void ESI_manager::slotError(QNetworkReply::NetworkError tcode)
{
    QMessageBox::information(
        this,
        QString( "Ошибка сервера" ).toUtf8(),
        QString( "Код ошибки: " )+QString::number(tcode),
        QMessageBox::Ok
    );

    qWarning(logWarning) << "Ошибка отправки POST сообщения №: " << QString::number(tcode);
}

//Слот для приема SSL ошибки от возвращаемых данных запроса
void ESI_manager::slotSSLError(QList<QSslError> ListError)
{
    foreach( const QSslError &ListError, ListError )
    {
        qDebug(logWarning) << "Ошибка SSL в возвращаемых данных: " << ListError.errorString();
    }
}

//Слот для приема SSL ошибки от менеджера
void ESI_manager::slotSSLErrorManeger(QNetworkReply * rep, QList<QSslError> listerr)
{
    qWarning(logWarning) << "SSL ошибка отправки POST сообщения №: " << QString::number(rep->error());
    foreach( const QSslError &listerr, listerr )
    {
        qDebug(logDebug) << "Ошибка SSL в менеджере отправки: " << listerr.errorString();
    }
}

void ESI_manager::GetResponse(QNetworkReply *reply)
{
    QByteArray *DataPOSTToken = new QByteArray(reply->readAll());
    QString tmp = DataPOSTToken->data();

    qDebug(logDebug) << "Ответ сервера на сообщение: "+tmp;

    this->reply->deleteLater();
    //emit WebSSOLogin::ReturnToken(Resp->size->toString);
}