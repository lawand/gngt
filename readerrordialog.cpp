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
#include "editnoundialog.h"

//corresponding header file(s)
#include "readerrordialog.h"
#include "ui_readerrordialog.h"

ReadErrorDialog::ReadErrorDialog(int lineNumber,
                                 QString line,
                                 int numberOfLines,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadErrorDialog),
    ignoreAll(false)
{
    ui->setupUi(this);

    this->lineNumber = lineNumber;
    this->line = line;
    this->numberOfLines = numberOfLines;

    ui->label->setText(QString("Cannot read line number: %1 which contains: "
                               "'%2' \nDo you want to edit this line or ignore "
                               "it?").arg(lineNumber).arg(line)
                       );

    ui->ignoreAllPushButton->setText(QString("Ignore All (%1)")
                                     .arg(numberOfLines)
                                     );
}

ReadErrorDialog::~ReadErrorDialog()
{
    delete ui;
}

bool ReadErrorDialog::shouldIgnoreAll()
{
    return ignoreAll;
}

Noun ReadErrorDialog::getNoun()
{
    return noun;
}

void ReadErrorDialog::on_fixPushButton_clicked()
{
    EditNounDialog editNounDialog;
    int result = editNounDialog.exec();
    if(result == QDialog::Accepted)
    {
        noun = Noun(editNounDialog.getText());
        accept();
    }
}

void ReadErrorDialog::on_ignorePushButton_clicked()
{
    reject();
}

void ReadErrorDialog::on_ignoreAllPushButton_clicked()
{
    ignoreAll = true;
    reject();
}
