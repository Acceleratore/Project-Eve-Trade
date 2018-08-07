#ifndef WORKDB_H
#define WORKDB_H

#include <QtSql>
#include <QSqlQuery>
#include "logger.h"

class QUserDBWork
{
    QSqlDatabase db;

public:
    QUserDBWork(QString _NameDriver);
    ~QUserDBWork();
    QSqlError InitDB (QString _NameDB);
    QSqlQuery GetListCharacters();
    bool FindCharacterID(int CharID);
    void UpdateCharacter(int CharID, QString CharName, QString Token);
    void InsertCharacter(int CharID, QString CharName, QString Token);

};

#endif // WORKDB_H
