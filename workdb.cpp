#include "workdb.h"

//Конструктор класса
QUserDBWork::QUserDBWork(QString _NameDriver, QString _NameConnect)
{

    /*if (this->db.isOpen())
    {
        this->db = QSqlDatabase::cloneDatabase(QSqlDatabase::database( _NameConnect ), "Clone"+_NameConnect );
        if (!this->db.open())
            qDebug(logCritical()) << "Не удалось открыть клонированное подключение к базе данных " << _NameConnect;
    } else*/
        this->db = QSqlDatabase::addDatabase(_NameDriver, _NameConnect);
}

QUserDBWork::QUserDBWork()
{
    //...
}

//Деструктор класса
QUserDBWork::~QUserDBWork()
{
    this->db.close();
}

void QUserDBWork::CloneDB(QString _NameConnect)
{
    this->db = QSqlDatabase::cloneDatabase(QSqlDatabase::database( _NameConnect ), "Clone"+_NameConnect );

    if (!this->db.open())
        qDebug(logCritical()) << "Не удалось открыть клонированное подключение к базе данных " << _NameConnect;
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

bool QUserDBWork::FindCharacterID(int CharID)
{
    QSqlQuery _Query(this->db);

    _Query.exec("SELECT * FROM \"Characters\" WHERE CharacterID = " + QString::number(CharID));

    return _Query.isNull(0);
}

void QUserDBWork::InsertCharacter(int CharID, QString CharName, QString Token)
{
    QSqlQuery _Query(this->db);
    bool FlagQuery;

    _Query.prepare("INSERT INTO Characters (CharacterID, Name, Token) VALUES (:id, :name, :token)");

    _Query.bindValue(":id",    CharID  );
    _Query.bindValue(":name",  CharName);
    _Query.bindValue(":token", Token   );

    FlagQuery = _Query.exec();


    qDebug(logInfo()) << "Выполнение запроса: " << _Query.lastQuery();
    qDebug(logInfo()) << "Статус выполнения запроса: " << FlagQuery;

}


void QUserDBWork::UpdateCharacter(int CharID, QString CharName, QString Token)
{
    QSqlQuery _Query(this->db);
    bool FlagQuery;

    _Query.prepare("UPDATE Characters SET Name = :name, Token = :token WHERE CharacterID = :id");

    _Query.bindValue(":id",    CharID  );
    _Query.bindValue(":name",  CharName);
    _Query.bindValue(":token", Token   );

    FlagQuery = _Query.exec();


    qDebug(logInfo()) << "Выполнение запроса: " << _Query.lastQuery();
    qDebug(logInfo()) << "Статус выполнения запроса: " << FlagQuery;

}
