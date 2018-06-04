#include "webssologin.h"
#include "ui_webssologin.h"

const QString WebSSOLogin::SSOAddress  = QString("https://login.eveonline.com/oauth/authorize/");
const QString WebSSOLogin::ReponseType = QString("code");
const QString WebSSOLogin::RedirectURL = QString("http://localhost:755/");
const QString WebSSOLogin::ClientID    = QString("703adcbe76c94fd59679ae32a04d8329");

WebSSOLogin::WebSSOLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSSOLogin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

WebSSOLogin::~WebSSOLogin()
{
    delete ui;
    delete LoginWebView;
}

void WebSSOLogin::ShowLogin()
{
    QUrl *AddressLogin = new QUrl;

    AddressLogin->setUrl(SSOAddress+"?"+"response_type="+ReponseType+"&"+"redirect_uri="+RedirectURL+"&"+"client_id="+ClientID+"&"+"scope=publicData%20characterWalletRead%20characterIndustryJobsRead%20characterMarketOrdersRead%20characterSkillsRead%20characterAccountRead%20characterClonesRead"+"&"+"state=TestState001");
    LoginWebView = new QWebEngineView(this);

    LoginWebView->setSizePolicy(QSizePolicy ::Expanding, QSizePolicy ::Expanding);
    LoginWebView->setGeometry(this->geometry());
    //LoginWebView->load(QUrl("https://login.eveonline.com/oauth/authorize/?response_type=code&redirect_uri=http://localhost:755/&client_id=703adcbe76c94fd59679ae32a04d8329&scope=publicData%20characterWalletRead%20characterIndustryJobsRead%20characterMarketOrdersRead%20characterSkillsRead%20characterAccountRead%20characterClonesRead&state=TestState001"));
    LoginWebView->load(AddressLogin->url());
    LoginWebView->showFullScreen();

    connect(LoginWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(WaitUrl(QUrl)), Qt::AutoConnection );

    this->exec();

}


void WebSSOLogin::WaitUrl(const QUrl &url)
{
    QUrl TestUrl = QUrl(RedirectURL, QUrl::TolerantMode);

    if (url.matches(TestUrl, QUrl ::RemoveQuery))
    {
        //TODO Возвращаем параметры из URL, проверяем их и закрываем окно
        LoginWebView->close();
        this->close();
    }

}
