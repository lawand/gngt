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
#include "readnounerrordialog.h"
#include "ui_readnounerrordialog.h"

ReadNounErrorDialog::ReadNounErrorDialog(int lineNumber,
                                 QString line,
                                 QList<Noun>* nouns,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadNounErrorDialog),
    ignoreAll(false)
{
    //initialization
    ui->setupUi(this);
    this->lineNumber = lineNumber;
    this->line = line;
    this->nouns = nouns;

    //fix window size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->label->setText(QString("Cannot read the noun in line: %1 \nWhich "
                               "contains: '%2' \n\nDo you want to edit this "
                               "line or ignore it? \nNote that ignoring lines "
                               "leads to deleting them.").arg(
                                       lineNumber).arg(line)
                       );
}

ReadNounErrorDialog::~ReadNounErrorDialog()
{
    delete ui;
}

bool ReadNounErrorDialog::shouldIgnoreAll()
{
    return ignoreAll;
}

QString ReadNounErrorDialog::getLine()
{
    return line;
}

void ReadNounErrorDialog::on_fixPushButton_clicked()
{
    EditNounDialog editNounDialog(nouns, this);
    editNounDialog.setText(line);
    int result = editNounDialog.exec();
    if(result == QDialog::Accepted)
    {
        line = editNounDialog.getText();
        accept();
    }
}

void ReadNounErrorDialog::on_ignorePushButton_clicked()
{
    reject();
}

void ReadNounErrorDialog::on_ignoreAllPushButton_clicked()
{
    ignoreAll = true;
    reject();
}
