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
