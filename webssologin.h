#ifndef WEBSSOLOGIN_H
#define WEBSSOLOGIN_H

#include <QDialog>
#include <QWebEngineView>
#include <QMessageBox>
#include <QUrlQuery>
#include "logger.h"
#include "esi_manager.h"

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
signals:
    void ReturnToken(QString str);
private:
    Ui::WebSSOLogin *ui;
    ESI_manager *ManagerESI = nullptr;
    QWebEngineView *LoginWebView = nullptr;
};

#endif // WEBSSOLOGIN_H
