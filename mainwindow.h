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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    ESI_manager *ManagerESI = nullptr;
    Ui::MainWindow *ui;
    void InitialLoginWindow();
    void GetClientIDByToken();

private slots:
    void on_actionManage_characters_triggered();
    void on_LoginSSOButton_clicked();
    void GetAuthData(QByteArray tBArr);
    void GetCharactersData(QByteArray tBArr);

signals:
    void NewToken();
    void UpdateCharacter(QUserDBWork tDB);
    void TransfCharacterData(int CharID, QString CharName, QString CharToken);

};

#endif // MAINWINDOW_H
