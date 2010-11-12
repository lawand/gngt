#ifndef EDITNOUNDIALOG_H
#define EDITNOUNDIALOG_H

//base class(es)
#include <QDialog>

namespace Ui {
    class EditNounDialog;
}

class EditNounDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNounDialog(QWidget *parent = 0);
    ~EditNounDialog();

private:
    Ui::EditNounDialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // EDITNOUNDIALOG_H
