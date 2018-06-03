#include "webssologin.h"
#include "ui_webssologin.h"
#include <QDebug>

WebSSOLogin::WebSSOLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSSOLogin)
{
    ui->setupUi(this);
}

WebSSOLogin::~WebSSOLogin()
{
    delete ui;
    delete LoginWebView;
}

void WebSSOLogin::ShowLogin()
{
    LoginWebView = new QWebEngineView(this);

    LoginWebView->setSizePolicy(QSizePolicy ::Expanding, QSizePolicy ::Expanding);
    //LoginWebView->setGeometry(this->geometry());
    LoginWebView->load(QUrl("https://login.eveonline.com/oauth/authorize/?response_type=code&redirect_uri=http://localhost:755/&client_id=703adcbe76c94fd59679ae32a04d8329&scope=publicData%20characterWalletRead%20characterIndustryJobsRead%20characterMarketOrdersRead%20characterSkillsRead%20characterAccountRead%20characterClonesRead&state=TestState001"));
    LoginWebView->showFullScreen();

    connect(LoginWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(WaitUrl(QUrl)), Qt::AutoConnection );

    this->exec();

}


void WebSSOLogin::WaitUrl(const QUrl &url)
{
    QUrl TestUrl = QUrl("http://localhost:755/", QUrl::TolerantMode);

    if (url.matches(TestUrl, QUrl ::RemoveQuery))
    {
        //TODO Возвращаем параметры из URL, проверяем их и закрываем окно
        LoginWebView->close();
        this->close();
    }

}
