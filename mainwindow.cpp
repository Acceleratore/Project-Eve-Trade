#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString PathToDir = QCoreApplication::applicationDirPath();

    /* БЛОК ДЛЯ ОТЛАДКИ */
    bool DEBUG_MODE = false;

    #ifndef QT_NO_DEBUG
        DEBUG_MODE = true;
    #endif

    //Для упрощения отладки, перед релизом можно убрать
    if (DEBUG_MODE)
    {
        PathToDir = PRO_FILE_PWD;
    } else
    {
        PathToDir = QCoreApplication::applicationDirPath();
    }

    qDebug(logDebug()) << "Директория до папки с проектом установлена в: "+PathToDir;
    /* ОКОНЧАНИЯ БЛОКА ИНФОРМАЦИИ ОТЛАДКИ */


    //Инициализация менеджера запросов
    this->ManagerESI = new ESI_manager(this);


    /* Блок подключения сигналов и слотов */

    connect( this, SIGNAL(NewToken()),                   this,       SLOT(GetClientIDByToken()),     Qt::AutoConnection );
    connect( this, SIGNAL(UpdateCharacter(QUserDBWork)), &wMC,       SLOT(RefreshData(QUserDBWork)), Qt::AutoConnection );
    connect( this, SLOT(GetCharactersData(QByteArray)),  ManagerESI, SIGNAL(ReturnData(QByteArray)), Qt::UniqueConnection);

    connect( this, SIGNAL(TransfCharacterData(int,QString,QString)), &wMC, SLOT(AddCharacter(int,QString,QString)), Qt::UniqueConnection);

    /* Окончание блока подключения сигналов и слотов */


    //Проверка на наличия драйвера SQLITE
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "Unable to load database", "Для работы необходим SQLITE драйвер");
        qCritical(logCritical()) << "Не найден драйвер для работы базы данных. Для работы необходим SQLITE драйвер!";
    }

    QUserDBWork MainDB("QSQLITE", "MainDB");

    //Инициализация ДБ
    QSqlError err = MainDB.InitDB(PathToDir+"/ProjectEVET.sqlite");
    if (err.type() != QSqlError::NoError)
    {
        QMessageBox::critical(this, "Неудачная попытка инициализации базы данных", "Не удалось инициализаровать базу данных, ошибка №: " + err.text());
        qCritical(logCritical()) << "Не удалось инициализаровать базу данных, ошибка №: " + err.text();
        return;
    }

    //Заполнение таблицы персонажей
    emit UpdateCharacter(MainDB);

    InitialLoginWindow();//Обрати внимание!!!???

}

//Инициализация окна с логином по SSO
void MainWindow::InitialLoginWindow()
{
    wWSSOL = new WebSSOLogin(this);
    connect( ui->LoginSSOButton, SIGNAL(clicked()),              wWSSOL, SLOT(ShowLogin()),             Qt::UniqueConnection );
    connect( wWSSOL,             SIGNAL(ReturnData(QByteArray)), this,   SLOT(GetAuthData(QByteArray)), Qt::UniqueConnection );
}

MainWindow::~MainWindow()
{
    qDebug(logInfo()) << "Закрытие программы";
    delete ui;
    delete wWSSOL;
}

void MainWindow::on_actionManage_characters_triggered()
{
    MainWindow::wMC.show();
}

//Слот для сигнала нажатия на кнопку входа
void MainWindow::on_LoginSSOButton_clicked()
{
    InitialLoginWindow();
}

//Слот для получения токенов при входе по SSO
void MainWindow::GetAuthData(QByteArray tBArr)
{
    QJsonDocument JsonDoc = QJsonDocument::fromJson(tBArr);

    if(!JsonDoc.isNull())
    {
        QJsonObject jObj = JsonDoc.object();

        this->TempToken   = QString(jObj.value("access_token").toString());
        this->RefreshToken = QString(jObj.value("refresh_token").toString());
        this->TypeToken   = QString(jObj.value("token_type").toString());

        emit NewToken();

        qDebug(logDebug()) << QString("Получены данные в формате Json: " + JsonDoc.toJson());
        qDebug(logDebug()) << QString("Выделенно значения токена: " + this->TempToken);
        qDebug(logDebug()) << QString("Выделенно значения токена обновления: " + this->RefreshToken);

    } else
        qDebug(logWarning()) << "Ошибка получения токена, ответ не содержит данные или данные не в формате Json. Размер ответа: " << QString::number(tBArr.size()) << " байт";

}

//Слот вызывается по сигналу о том, что был выдан новый токен обновления и его необходимо изменить
void MainWindow::GetCharactersData(QByteArray tBArr)
{
    //TODO !!!!
    QJsonDocument JsonDoc = QJsonDocument::fromJson(tBArr);

    if(!JsonDoc.isNull())
    {
        QJsonObject jObj = JsonDoc.object();

        int     CharacterID   = jObj.value("CharacterID").toInt();
        QString CharacterName = QString(jObj.value("CharacterName").toString());

        emit TransfCharacterData(CharacterID, CharacterName, this->RefreshToken);

        qDebug(logDebug()) << QString("Получены данные в формате Json: " + JsonDoc.toJson());

    } else
        qDebug(logWarning()) << "Ошибка получения токена, ответ не содержит данные или данные не в формате Json. Размер ответа: " << QString::number(tBArr.size()) << " байт";

}

//Получить ID персонажа по временному токену
void MainWindow::GetClientIDByToken()
{
    ManagerESI->Set_sslConfig(QSsl::AnyProtocol);
    ManagerESI->Set_Request(ESI_manager::VerifuAddress, this->TypeToken, this->TempToken, ESI_manager::TypeJson);
    ManagerESI->SendGet();
}
