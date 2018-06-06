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


void WebSSOLogin::WaitUrl(const QUrl &url)
{
    QUrl RedUrl = QUrl(RedirectURL, QUrl::TolerantMode);


    if ( (url.matches(RedUrl, QUrl ::RemoveQuery)) && (QUrlQuery(url.query()).queryItemValue("state") == "TestState001") )
    {
        //TODO Забираем параметры из URL, проверяем их и закрываем окно

        //Забрали из URL проверочный токен
        QString AccessToken = QUrlQuery(url.query()).queryItemValue("code").trimmed();

        //Собираем POST запрос на получение токена для получения данных
        QString AuthStr = QString(ClientID+":"+SecretKey).trimmed();

        QNetworkRequest Request;
        Request.setUrl(QUrl(SSOAddress+"/token"));
        Request.setRawHeader("Authorization", "Basic "+AuthStr.toLatin1().toBase64());
        Request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

        QUrlQuery ParmReq;
        QUrl QueryPOST;
        ParmReq.addQueryItem("grant_type", "authorization_code");
        ParmReq.addQueryItem("code", AccessToken);

        QueryPOST.setQuery(ParmReq);

        //ТЕСТ
        //QNetworkReply *reply = Net_Manager->post(Request, QString("grant_type=authorization_code&code="+AccessToken).toUtf8());
        QNetworkRequest TestReq;
        TestReq.setUrl(QUrl("http://httpbin.org/post"));
        TestReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply * reply = Net_Manager->post(TestReq, "");

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));

        LoginWebView->close();
        /*
        this->close();
        */

    }

}

void WebSSOLogin::GetResponse(QNetworkReply *reply)
{
    QByteArray *DataPOSTToken = new QByteArray(reply->readAll());
    QString tmp = DataPOSTToken->data();

    QMessageBox::information(
        this,
        trUtf8( "Ответ сервера" ),
        tmp,
        QMessageBox::Ok
    );

    reply->deleteLater();
    //emit WebSSOLogin::ReturnToken(Resp->size->toString);
}

void WebSSOLogin::slotError(QNetworkReply::NetworkError tcode)
{
    /*
    QMessageBox::information(
        this,
        trUtf8( "Ответ сервера" ),
        QString::number(tcode) ,
        QMessageBox::Ok
    );*/
}
