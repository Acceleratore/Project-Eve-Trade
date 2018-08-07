#ifndef MANAGECHARACTERS_H
#define MANAGECHARACTERS_H

#include <QDialog>
#include <QtWidgets>
#include "QSqlQuery"
#include "workdb.h"

namespace Ui {
class ManageCharacters;
}

class ManageCharacters : public QDialog
{
    Q_OBJECT
public:
    explicit ManageCharacters(QWidget *parent = 0);
    ~ManageCharacters();
    void SetDataInCharTable(QSqlQuery _Query);

public slots:
    void RefreshData(QUserDBWork tDB);
    void AddCharacter(int CharID, QString CharName, QString CharToken);

private slots:
    void on_pushButton_add_clicked();

private:
    Ui::ManageCharacters *ui;

};

#endif // MANAGECHARACTERS_H
