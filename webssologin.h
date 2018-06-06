#ifndef WEBSSOLOGIN_H
#define WEBSSOLOGIN_H

#include <QDialog>
#include "QWebEngineView"
#include "QMessageBox"
#include <QUrlQuery>
#include "QNetworkReply"
#include "QNetworkAccessManager"
#include "QByteArray"
#include "QNetworkRequest"

namespace Ui {
class WebSSOLogin;
}

class WebSSOLogin : public QDialog
{
    Q_OBJECT

public:
    explicit WebSSOLogin(QWidget *parent = 0);
    ~WebSSOLogin();
public slots:
    void ShowLogin();
    void WaitUrl(const QUrl &url);
    void GetResponse(QNetworkReply *reply);
    void slotError(QNetworkReply::NetworkError tcode);
signals:
    void ReturnToken(QString str);
private:
    Ui::WebSSOLogin *ui;

    static const QString SSOAddress;
    static const QString ReponseType;
    static const QString RedirectURL;
    static const QString ClientID;
    static const QString SecretKey;
    QWebEngineView *LoginWebView = 0;
    QNetworkAccessManager* Net_Manager = 0;
};

#endif // WEBSSOLOGIN_H
