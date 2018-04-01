#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Проверка на наличия драйвера SQLITE
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "Для работы необходим SQLITE драйвер");

    QUserDBWork CharactersDB;

    //Инициализация ДБ
    QSqlError err = CharactersDB.InitDB("QSQLITE", "M:\\ProjectEVET.sqlite");
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Неудачная попытка инициализации базы данных",
                    "Не удалось инициализаровать базу данных, ошибка №: " + err.text());
        return;
    }

    MainWindow::wMC.SetDataInCharTable(CharactersDB.GetListCharacters());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionManage_characters_triggered()
{
    MainWindow::wMC.show();
}
