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

    QString PathToDB = "";

    bool DEBUG_MODE = false;

    #ifndef QT_NO_DEBUG
        DEBUG_MODE = true;
    #endif

    //Для упрощения отладки, перед релизом можно убрать
    if (DEBUG_MODE)
    {
        PathToDB = PRO_FILE_PWD;
    } else
    {
        PathToDB = QCoreApplication::applicationDirPath();
    }

    //Инициализация ДБ
    QSqlError err = CharactersDB.InitDB(PathToDB+"/ProjectEVET.sqlite");
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::critical(this, "Неудачная попытка инициализации базы данных", "Не удалось инициализаровать базу данных, ошибка №: " + err.text());
        return;
    }

    //Заполнение таблицы персонажей
    MainWindow::wMC.SetDataInCharTable(CharactersDB.GetListCharacters());


    ui->m_webview->load(QUrl("https://google.ru/"));
    ui->m_webview->show();

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
