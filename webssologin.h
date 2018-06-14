#ifndef WEBSSOLOGIN_H
#define WEBSSOLOGIN_H

#include <QDialog>
#include <QWebEngineView>
#include <QMessageBox>
#include <QUrlQuery>
#include "logger.h"

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
    void slotSSLError(QList<QSslError> ListError);
    void slotSSLError2(QNetworkReply*, QList<QSslError>);
signals:
    void ReturnToken(QString str);
private:
    Ui::WebSSOLogin *ui;


    QWebEngineView *LoginWebView = 0;
};

#endif // WEBSSOLOGIN_H
