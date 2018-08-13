#include "managecharacters.h"
#include "ui_managecharacters.h"

//Конструктор
ManageCharacters::ManageCharacters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageCharacters)
{
    ui->setupUi(this);
}

//Деструктор
ManageCharacters::~ManageCharacters()
{
    delete ui;
}

//Сигнал на добавление новой строки в таблицу
void ManageCharacters::on_pushButton_add_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}


//Заполнить таблицу данными
void ManageCharacters::SetDataInCharTable(QSqlQuery _Query)
{
    int IndChr = 0;
    int IndCol = 0;

    while (_Query.next())
    {
        //Узнаем сколько в таблице строк и вставляем новую (количество начинается с 1, исчесление с 0)
        IndChr = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(IndChr);

        //Вставляем в каждый столбец информацию, полученную из таблицы
        IndCol = 0;
        while (ui->tableWidget->columnCount() > IndCol)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(_Query.value(IndCol).toString());
            ui->tableWidget->setItem(IndChr, IndCol, item);

            IndCol++;
        }
    }
}

void ManageCharacters::RefreshData(QUserDBWork tDB)
{
    this->ui->tableWidget->clear();
    this->SetDataInCharTable(tDB.GetListCharacters());
}

void AddCharacter(int CharID, QString CharName, QString CharToken)
{
    QUserDBWork db = QUserDBWork( "QSQLITE", "MainDB" );

    if (db.FindCharacterID(CharID))
    {
        db.UpdateCharacter(CharID, CharName, CharToken);
    } else
        db.InsertCharacter(CharID, CharName, CharToken);
}
