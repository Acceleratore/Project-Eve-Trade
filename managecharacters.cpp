#include "managecharacters.h"
#include "ui_managecharacters.h"

ManageCharacters::ManageCharacters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageCharacters)
{
    ui->setupUi(this);
}

ManageCharacters::~ManageCharacters()
{
    delete ui;
}
