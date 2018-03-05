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

void ManageCharacters::SetDataInTable(QString &_NameChar)
{
    int IndChr;

    IndChr = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(IndChr);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(_NameChar);
    ui->tableWidget->setItem(IndChr, 0, item);
}

