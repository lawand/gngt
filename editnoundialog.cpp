/****************************************************************************
**
** Copyright 2010 Omar Lawand Dalatieh.
** Contact: see the README file.
**
** This file is part of GNGT.
**
** GNGT is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GNGT is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser Public License for more details.
**
** You should have received a copy of the GNU Lesser Public License
** along with GNGT.  If not, see <http://www.gnu.org/licenses/>.
**
** For more information, check out the GNU General Public license found
** in the COPYING file, the GNU Lesser Public license found in the
** COPYING.LESSER file and the README file.
**
****************************************************************************/

//implementation-specific data type(s)
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopWidget>
#include <QTimer>

//corresponding header file(s)
#include "editnoundialog.h"
#include "ui_editnoundialog.h"

EditNounDialog::EditNounDialog(QList<Noun>* nouns, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    //fix window size
#ifndef Q_OS_SYMBIAN
    layout()->setSizeConstraint(QLayout::SetFixedSize);
#endif

    //manage width of dialog
#ifdef Q_OS_SYMBIAN
    //smaller on small screens
    if(QApplication::desktop()->screenGeometry().width() <= 240)
    {
        QFont smallerFont = this->font();
        smallerFont.setPointSize(this->font().pointSize() - 3);
        ui->capitalAWithDiaeresisToolButton->setFont(smallerFont);
        ui->capitalOWithDiaeresisToolButton->setFont(smallerFont);
        ui->capitalUWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallSharpSToolButton->setFont(smallerFont);
        ui->smallAWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallOWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallUWithDiaeresisToolButton->setFont(smallerFont);
    }

    //larger on large screens
    if(qApp->navigationMode() == Qt::NavigationModeNone)
    {
        //delay this so that the dialog can first get correct geometry values
        QTimer::singleShot(500, this, SLOT(setupWidth()));
    }
#endif

}

EditNounDialog::~EditNounDialog()
{
    //delete data members
    delete ui;
}

void EditNounDialog::setText(QString text)
{
    ui->definiteArticleAndSingularFormLineEdit->setText(text);
}

QString EditNounDialog::getText()
{
    return ui->definiteArticleAndSingularFormLineEdit->text();
}

void EditNounDialog::on_buttonBox_accepted()
{
    //handle the case of an invalid noun string
    if(! Noun::isValid(ui->definiteArticleAndSingularFormLineEdit->text()))
    {
        QMessageBox::information(this, "Noun Format Error",
                                 "Incorrect format. The correct format is "
                                 "like this: 'das Buch' \n"
                                 "Note that the singular form can contain at "
                                 "most 23 characters."
                                 );

        return;
    }

    //handle the case of a duplicate noun
    Noun noun(ui->definiteArticleAndSingularFormLineEdit->text());
    if(nouns->indexOf(noun) != -1)
    {
        QMessageBox::information(this,
                                 "Duplicate Noun",
                                 "This noun already exists");

        return;
    }

    //at this point, accept can be called
    accept();
}

void EditNounDialog::on_definiteArticleAndSingularFormLineEdit_textChanged(
        QString
        )
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            ! ui->definiteArticleAndSingularFormLineEdit->text().isEmpty()
            );
}

void EditNounDialog::on_capitalAWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xC4));
}

void EditNounDialog::on_capitalOWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xD6));
}

void EditNounDialog::on_capitalUWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xDC));
}

void EditNounDialog::on_smallSharpSToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xDF));
}

void EditNounDialog::on_smallAWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xE4));
}

void EditNounDialog::on_smallOWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xF6));
}

void EditNounDialog::on_smallUWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndSingularFormLineEdit->insert(QChar(0xFC));
}

#ifdef Q_OS_SYMBIAN
void EditNounDialog::setupWidth()
{
    QRect geometry = this->geometry();
    geometry.setWidth(360);
    setGeometry(geometry);
}
#endif
