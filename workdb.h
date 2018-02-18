#ifndef WORKDB_H
#define WORKDB_H

#include <QtSql>
#include "QSqlQuery"

QSqlError InitDB()
{
    QSqlDatabase db;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("M:\\ProjectEVET.sqlite");

    if (!db.open())
        return db.lastError();

    return QSqlError();
}
/*
QSqlError GetListCharacters(QTableWidget *_TbW)
{



    return QSqlError();
}
*/

#endif // WORKDB_H
