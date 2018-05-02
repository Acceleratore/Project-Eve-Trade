#ifndef WORKDB_H
#define WORKDB_H

#include <QtSql>
#include "QSqlQuery"

class QUserDBWork
{
    QSqlDatabase db;

    public:
    QUserDBWork(QString _NameDriver);
    ~QUserDBWork();
    QSqlError InitDB (QString _NameDB);
    QSqlQuery GetListCharacters();

};

#endif // WORKDB_H
