#ifndef WEBSSOLOGIN_H
#define WEBSSOLOGIN_H

#include <QDialog>
#include "QWebEngineView"

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

private:
    Ui::WebSSOLogin *ui;
    QWebEngineView *LoginWebView = 0;
};

#endif // WEBSSOLOGIN_H