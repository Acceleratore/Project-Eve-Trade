#ifndef WORKDB_H
#define WORKDB_H

#include <QtSql>
#include "QSqlQuery"

class QUserDBWork
{
    QSqlDatabase db;

    public:
    QUserDBWork();
    ~QUserDBWork();
    QSqlError InitDB (QString _NameDriver, QString _NameDB);
    QSqlQuery GetListCharacters();

};


QSqlError QUserDBWork::InitDB(QString _NameDriver, QString _NameDB)
{

    db = QSqlDatabase::addDatabase(_NameDriver);
    db.setDatabaseName(_NameDB);

    if (!db.open())
        return db.lastError();

    return QSqlError();
}

//Запрос списка персонажей в БД
QSqlQuery QUserDBWork::GetListCharacters()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM \"Characters\"");

    return query;
}

#endif // WORKDB_H
