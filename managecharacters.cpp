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
    int IndChr;

    //Узнаем сколько в таблице строк и вставляем новую (количество начинается с 1, исчесление с 0)
    IndChr = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(IndChr);

    while (_Query.next())
    {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(_Query.value(0).toString());
        ui->tableWidget->setItem(IndChr, 0, item1);
    }

    //Нужно другое решение
    /*
    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setText(_CharName);
    ui->tableWidget->setItem(IndChr, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setText(_Key);
    ui->tableWidget->setItem(IndChr, 1, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setText(_VCode);
    ui->tableWidget->setItem(IndChr, 2, item3);
    */
}
