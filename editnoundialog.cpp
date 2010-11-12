//implementation-specific data type(s)
#include <QMessageBox>
#include <noun.h>

//corresponding header file(s)
#include "editnoundialog.h"
#include "ui_editnoundialog.h"

EditNounDialog::EditNounDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounDialog)
{
    ui->setupUi(this);
}

EditNounDialog::~EditNounDialog()
{
    delete ui;
}

void EditNounDialog::on_buttonBox_accepted()
{
    if(Noun::isValid(ui->definiteArticleAndSingularFormLineEdit->text()))
    {
        accept();
    }
    else
    {
        QMessageBox::information(this, "Noun Format Error",
                                 "Incorrect format. The correct format is "
                                 "composed of the singular form of the noun "
                                 "preceded by it's corresponding definite "
                                 "article (i.e. 'das Buch' (without the "
                                 "quotes))."
                                 );
    }
}
