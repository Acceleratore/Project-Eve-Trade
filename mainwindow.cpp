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

    QUserDBWork CharactersDB("QSQLITE");

    //Инициализация ДБ
    QSqlError err = CharactersDB.InitDB("M:\\ProjectEVET.sqlite");
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::critical(this, "Неудачная попытка инициализации базы данных", "Не удалось инициализаровать базу данных, ошибка №: " + err.text());
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

//Конструктор класса
QUserDBWork::QUserDBWork(QString _NameDriver)
{
    this->db = QSqlDatabase::addDatabase(_NameDriver);
}

//Деструктор класса
QUserDBWork::~QUserDBWork()
{
    this->db.close();
}

//Инициализация базы данных
QSqlError QUserDBWork::InitDB(QString _NameDB)
{
    this->db.setDatabaseName(_NameDB);

    if (!this->db.open())
        return this->db.lastError();

    return QSqlError();
}

//Запрос списка персонажей в БД
QSqlQuery QUserDBWork::GetListCharacters()
{
    QSqlQuery query(this->db);
    query.exec("SELECT * FROM \"Characters\"");

    return query;
}
