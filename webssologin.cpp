#include "webssologin.h"
#include "ui_webssologin.h"

const QString WebSSOLogin::SSOAddress  = QString("https://login.eveonline.com/oauth");
const QString WebSSOLogin::ReponseType = QString("code");
const QString WebSSOLogin::RedirectURL = QString("http://localhost:755/");
const QString WebSSOLogin::ClientID    = QString("703adcbe76c94fd59679ae32a04d8329");
const QString WebSSOLogin::SecretKey   = QString("EdMrlKYJnMgq8eZIAxlHUWJDol3ikywjX25T6xCY");

WebSSOLogin::WebSSOLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSSOLogin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Вход по SSO");
    Net_Manager = new QNetworkAccessManager( this );

    connect(Net_Manager, SIGNAL(finished(QNetworkReply*)) , this, SLOT(GetResponse(QNetworkReply*)));
    connect(Net_Manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(slotSSLError2(QNetworkReply*,QList<QSslError>)));
}

WebSSOLogin::~WebSSOLogin()
{
    delete ui;
    delete LoginWebView;
    delete Net_Manager;
}

void WebSSOLogin::ShowLogin()
{
    QUrl AddressLogin;
    QUrlQuery AddressQuery;

    //Собираем адресную строку
    AddressLogin.setUrl(SSOAddress+"/authorize/");

    AddressQuery.addQueryItem("response_type", ReponseType);
    AddressQuery.addQueryItem("redirect_uri", RedirectURL);
    AddressQuery.addQueryItem("client_id", ClientID);
    AddressQuery.addQueryItem("scope", "publicData%20characterWalletRead%20characterIndustryJobsRead%20characterMarketOrdersRead%20characterSkillsRead%20characterAccountRead%20characterClonesRead");
    AddressQuery.addQueryItem("state", "TestState001");
    AddressLogin.setQuery(AddressQuery);

    if (AddressLogin.isValid())
    {
        LoginWebView = new QWebEngineView(this);

        LoginWebView->setSizePolicy(QSizePolicy ::Expanding, QSizePolicy ::Expanding);
        LoginWebView->setGeometry(this->geometry());
        LoginWebView->load(AddressLogin.url());
        LoginWebView->showFullScreen();

        //Для отслеживания редиректа на localhost, это означает что логин прошел и токен был получен
        connect( LoginWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(WaitUrl(QUrl)), Qt::AutoConnection );
    } else
        QMessageBox::critical(this, "Ошибочный URL", "URL не валиден: " + AddressLogin.toString());


    //delete AddressLogin;
    this->exec();

}


//Слот принимает URL при смене страницы и проверяет на ожидаемый
//В случае совпадения, он забирает с него переданный код
void WebSSOLogin::WaitUrl(const QUrl &url)
{
    QUrl RedUrl = QUrl(RedirectURL, QUrl::TolerantMode);

    //Если это необходимая страница и код статуса совпадает с переданым, извлекаем токен авторизации и посылаем POST запрос на получения токена доступа
    if ( (url.matches(RedUrl, QUrl ::RemoveQuery)) && (QUrlQuery(url.query()).queryItemValue("state") == "TestState001") )
    {

        //Забрали из URL проверочный токен
        QString AccessToken = QUrlQuery(url.query()).queryItemValue("code").trimmed();

        //Собираем POST запрос на получение токена для получения данных
        //Код авторизации
        QString AuthStr = QString(ClientID+":"+SecretKey).trimmed();

        //Передаем данные загаловка

        QNetworkRequest Request;
        Request.setUrl(QUrl( SSOAddress+"/token" ));
        Request.setRawHeader("Authorization", QString("Basic "+AuthStr.toLatin1().toBase64()).toUtf8());
        Request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

        QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
        //QList<QSslCertificate> ca_new = QSslCertificate::fromData("CaCertificates");
        sslConfig.setProtocol(QSsl::SslV3);
        //sslConfig.setCaCertificates(ca_new);
        Request.setSslConfiguration(sslConfig);

        //Данные запроса
        QJsonObject QueryJsonPOST;
        QueryJsonPOST.insert("grant_type", QJsonValue::fromVariant("authorization_code"));
        QueryJsonPOST.insert("code", QJsonValue::fromVariant(AccessToken));
        QJsonDocument JsonDoc(QueryJsonPOST);

        QNetworkReply *reply = nullptr;
        //Отправка запроса
        reply = Net_Manager->post(Request, JsonDoc.toBinaryData());


        //ТЕСТ
/*
        QNetworkRequest TestReq;
        TestReq.setUrl(QUrl("http://httpbin.org/post"));
        TestReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        TestReq.setRawHeader("Authorization", "Basic "+AuthStr.toLatin1().toBase64());
        QNetworkReply * reply = Net_Manager->post(TestReq, QString("123").toUtf8());
*/
        //ТЕСТ

        //Соединение сигнала ошибок с слотом обработки ошибки при отправке
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)),        this, SLOT(slotSSLError(QList<QSslError>)));

        LoginWebView->close();
    }

}

void WebSSOLogin::GetResponse(QNetworkReply *reply)
{
    QByteArray *DataPOSTToken = new QByteArray(reply->readAll());
    QString tmp = DataPOSTToken->data();
/*
    QMessageBox::information(
        this,
        trUtf8( "Ответ сервера" ),
        tmp,
        QMessageBox::Ok
    );
*/
    qDebug(logDebug) << "Ответ сервера на сообщение: "+tmp;

    reply->deleteLater();
    //emit WebSSOLogin::ReturnToken(Resp->size->toString);
}

//Слот для приема номера ошибки при отправке POST сообщения
void WebSSOLogin::slotError(QNetworkReply::NetworkError tcode)
{
    /*
    QMessageBox::information(
        this,
        trUtf8( "Ошибка сервера" ),
        QString("Number POST error: ")+QString::number(tcode),
        QMessageBox::Ok
    );
*/
    qWarning(logWarning) << "Ошибка отправки POST сообщения №: " << QString::number(tcode);

}

void WebSSOLogin::slotSSLError(QList<QSslError> ListError)
{
    QMessageBox::information(
        this,
        trUtf8( "Ошибка сервера" ),
        trUtf8( "Ошибка сервера" ),
        QMessageBox::Ok
    );

    foreach( const QSslError &ListError, ListError )
    {
        qDebug(logDebug) << "Ошибка SSL: " << ListError.errorString();
    }
}

void WebSSOLogin::slotSSLError2(QNetworkReply * rep, QList<QSslError> listerr)
{
    qWarning(logWarning) << "SSL ошибка отправки POST сообщения №: " << QString::number(rep->error());
    foreach( const QSslError &listerr, listerr )
    {
        qDebug(logDebug) << "Ошибка SSL: " << listerr.errorString();
    }
}
