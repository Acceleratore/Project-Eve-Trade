#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool DEBUG_MODE = false;

    #ifndef QT_NO_DEBUG
        DEBUG_MODE = true;
    #endif

    //Проверка на наличия драйвера SQLITE
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "Unable to load database", "Для работы необходим SQLITE драйвер");
        qCritical(logCritical()) << "Не найден драйвер для работы базы данных. Для работы необходим SQLITE драйвер!";
    }
    QUserDBWork CharactersDB("QSQLITE");

    QString PathToDir = "";

    //Для упрощения отладки, перед релизом можно убрать
    if (DEBUG_MODE)
    {
        PathToDir = PRO_FILE_PWD;
    } else
    {
        PathToDir = QCoreApplication::applicationDirPath();
    }

    qDebug(logDebug()) << "Директория до папки с проектом установлена в: "+PathToDir;

    //Инициализация ДБ
    QSqlError err = CharactersDB.InitDB(PathToDir+"/ProjectEVET.sqlite");
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::critical(this, "Неудачная попытка инициализации базы данных", "Не удалось инициализаровать базу данных, ошибка №: " + err.text());
        qCritical(logCritical()) << "Не удалось инициализаровать базу данных, ошибка №: " + err.text();
        return;
    }

    //Заполнение таблицы персонажей
    MainWindow::wMC.SetDataInCharTable(CharactersDB.GetListCharacters());

    InitialLoginWindow();

}

//Инициализация окна с логином по SSO
void MainWindow::InitialLoginWindow()
{
    wWSSOL = new WebSSOLogin(this);
    connect(ui->LoginSSOButton, SIGNAL(clicked()), wWSSOL, SLOT(ShowLogin()), Qt::UniqueConnection);
    connect( wWSSOL, SIGNAL(ReturnToken(QString)), this, SLOT(GetToken(QString)) );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete wWSSOL;
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
    {
        return this->db.lastError();
        qCritical(logCritical()) << "Ошибка открытия базы данных: "+_NameDB;
    }
    return QSqlError();
}

//Запрос списка персонажей в БД
QSqlQuery QUserDBWork::GetListCharacters()
{
    QSqlQuery query(this->db);
    query.exec("SELECT * FROM \"Characters\"");

    return query;
}

void MainWindow::on_LoginSSOButton_clicked()
{
    InitialLoginWindow();
}

void MainWindow::GetToken(QString str)
{

    QMessageBox::information(
        this,
        trUtf8( "Ответ сервера" ),
        str,
        QMessageBox::Ok
    );
}
