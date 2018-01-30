#ifndef MANAGECHARACTERS_H
#define MANAGECHARACTERS_H

#include <QDialog>

namespace Ui {
class ManageCharacters;
}

class ManageCharacters : public QDialog
{
    Q_OBJECT

public:
    explicit ManageCharacters(QWidget *parent = 0);
    ~ManageCharacters();

private:
    Ui::ManageCharacters *ui;
};

#endif // MANAGECHARACTERS_H
