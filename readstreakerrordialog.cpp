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
#include <QInputDialog>

//corresponding header file(s)
#include "readstreakerrordialog.h"
#include "ui_readstreakerrordialog.h"

ReadStreakErrorDialog::ReadStreakErrorDialog(int lineNumber,
                                             QString line,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadStreakErrorDialog),
    ignoreAll(false),
    value(0)
{
    //initialization
    ui->setupUi(this);
    this->lineNumber = lineNumber;
    this->line = line;

    //fix window size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->label->setText(QString("Cannot read the memorization streak for the "
                               "noun found at line number: %1 \nWhich contains: "
                               "'%2' \n\nDo you want to enter the correct "
                               "value or ignore it? \nNote that ignoring "
                               "values leads to setting them to 0.").arg(
                                       lineNumber).arg(line)
                       );
}

ReadStreakErrorDialog::~ReadStreakErrorDialog()
{
    delete ui;
}

bool ReadStreakErrorDialog::shouldIgnoreAll()
{
    return ignoreAll;
}

int ReadStreakErrorDialog::getValue()
{
    return value;
}

void ReadStreakErrorDialog::on_fixPushButton_clicked()
{
    value = QInputDialog::getInt(this,
                 "Memorization Streak Correction",
                 "Memorization Streak:",
                 0, 0, 9);

    accept();
}

void ReadStreakErrorDialog::on_ignorePushButton_clicked()
{
    reject();
}

void ReadStreakErrorDialog::on_ignoreAllPushButton_clicked()
{
    ignoreAll = true;
    reject();
}
