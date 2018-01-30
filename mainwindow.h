#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "managecharacters.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ManageCharacters wMC;
public:
    explicit MainWindow(QWidget *parent = 0);
    void CreateMCW();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_actionManage_characters_triggered();
};

#endif // MAINWINDOW_H
