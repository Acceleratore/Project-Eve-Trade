#include "webssologin.h"
#include "ui_webssologin.h"

WebSSOLogin::WebSSOLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSSOLogin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Вход по SSO");
}

WebSSOLogin::~WebSSOLogin()
{
    delete ui;
    delete LoginWebView;
    delete ManagerESI;
}

//Слот через который корректно открывается окно с веб компонентом
void WebSSOLogin::ShowLogin()
{
    QUrl AddressLogin;
    QUrlQuery AddressQuery;

    //Собираем адресную строку
    AddressLogin.setUrl(ESI_manager::SSOAddress+"/authorize/");

    AddressQuery.addQueryItem("response_type", ESI_manager::ReponseType);
    AddressQuery.addQueryItem("redirect_uri", ESI_manager::RedirectURL);
    AddressQuery.addQueryItem("client_id", ESI_manager::ClientID);
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

    this->exec();

}


//Слот принимает URL при смене страницы и проверяет на ожидаемый
//В случае совпадения, он забирает с него переданный код
void WebSSOLogin::WaitUrl(const QUrl &url)
{

    QUrl RedUrl = QUrl(ESI_manager::RedirectURL, QUrl::TolerantMode);

    //Если это необходимая страница и код статуса совпадает с переданым, извлекаем токен авторизации и посылаем POST запрос на получения токена доступа
    if ( (url.matches(RedUrl, QUrl::RemoveQuery)) && (QUrlQuery(url.query()).queryItemValue("state") == "TestState001") )
    {

        //Забрали из URL проверочный токен
        QString AccessToken = QUrlQuery(url.query()).queryItemValue("code").trimmed();
        ManagerESI = new ESI_manager(this);

        //Соединение сигнала менеджера со слотом окна для получения данных назад
        //В данном случае возвращается временный токен для получения доступа к API и токен обновления,
        //для получения нового временного токена после истечения предыдущего
        connect(ManagerESI, SIGNAL(ReturnData(QByteArray)), this, SLOT(GetDataPOSTResp(QByteArray)), Qt::AutoConnection);

        ManagerESI->Set_sslConfig(QSsl::AnyProtocol);
        ManagerESI->Set_Request(ESI_manager::TypeURLENCODED);
        ManagerESI->Set_Query(ESI_manager::Authorize, ESI_manager::Code, AccessToken);
        ManagerESI->SendPost();

        LoginWebView->close();
    }

}

//Слот принимает данные из POST запроса (в данном случае временный токен, время жизни токена, токен обновления)
//и передает его в основное окно
void WebSSOLogin::GetDataPOSTResp(QByteArray tBArr)
{
    emit WebSSOLogin::ReturnData(tBArr);
    this->close();
}
