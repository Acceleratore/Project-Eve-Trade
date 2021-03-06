#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "managecharacters.h"
#include "workdb.h"
#include "webssologin.h"
#include "logger.h"
#include "esi_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ManageCharacters wMC;
    QDialog *wWSSOL = nullptr;
    QString TempToken;
    QString RefreshToken;
    QString TypeToken;
    ESI_manager *ManagerESI = nullptr;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void InitialLoginWindow();

private slots:
    void on_actionManage_characters_triggered();
    void on_LoginSSOButton_clicked();
    void GetAuthData(QByteArray tBArr);
    void GetCharactersData(QByteArray tBArr);
    void GetClientIDByToken();
    //void RefreshListChar();
    void UpdateStatusBar(QString _text);

signals:
    void NewToken();
    void UpdateCharacter(QUserDBWork tDB);
    void TransfCharacterData(int CharID, QString CharName, QString CharToken);

};

#endif // MAINWINDOW_H
