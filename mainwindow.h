#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "managecharacters.h"
#include "workdb.h"
#include "webssologin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ManageCharacters wMC;
    QDialog *wWSSOL = 0;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void InitialLoginWindow();

private slots:
    void on_actionManage_characters_triggered();
    void on_LoginSSOButton_clicked();
};

#endif // MAINWINDOW_H
