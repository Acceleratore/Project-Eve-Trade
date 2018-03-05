#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workdb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Проверка на наличия драйвера SQLITE
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "Для работы необходим SQLITE драйвер");

    //Инициализация ДБ

    QSqlError err = InitDB();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to initialize Database",
                    "Error initializing database: " + err.text());
        return;
    }

/*
    QSqlDatabase db;
    QSqlError err;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("M:\\ProjectEVET.sqlite");

    if (!db.open())
        err = db.lastError();

    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to initialize Database", "Error initializing database: " + err.text());
    }
*/
    QSqlQuery query;
    query.exec("SELECT * FROM \"Characters\"");

    //Выводим значения из запроса
    while (query.next())
    {
        QString Name = query.value(0).toString();
        MainWindow::wMC.SetDataInTable(Name);
        //ui->textEdit->insertPlainText(Name);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionManage_characters_triggered()
{
    MainWindow::wMC.show();
}
