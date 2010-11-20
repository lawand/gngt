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

//data member(s)
#include <QStringList>

//implementation-specific data type(s)
#include <QMessageBox>
#include "editnoundialog.h"

//corresponding header file(s)
#include "editlinesdialog.h"
#include "ui_editlinesdialog.h"

EditLinesDialog::EditLinesDialog(QStringList* lines,
                                 QList<Noun>* nouns,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLinesDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;
    this->lines = lines;

    //fix window size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    //initial update
    updateGui();
}

EditLinesDialog::~EditLinesDialog()
{
    delete ui;
}

void EditLinesDialog::updateGui()
{
    ui->numberOfLinesLabel->setText(
            QString("Number Of Lines: %2").arg(nouns->length())
            );

    ui->linesListWidget->clear();

    if(!lines->isEmpty())
    {
        ui->linesListWidget->addItems(*lines);

        ui->editPushButton->setEnabled(true);
        ui->removePushButton->setEnabled(true);
        ui->removeAllPushButton->setEnabled(true);
    }
    else
    {
        ui->editPushButton->setEnabled(false);
        ui->removePushButton->setEnabled(false);
        ui->removeAllPushButton->setEnabled(false);
    }
}

void EditLinesDialog::on_editPushButton_clicked()
{
    //if no line is selected
    if(ui->linesListWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select A Line",
                                 "A line must be selected");
    }
    else
    {
        EditNounDialog editNounDialog(nouns);
        editNounDialog.setText(ui->linesListWidget->currentItem()->text());

        if(editNounDialog.exec() == QDialog::Accepted)
        {
            lines->removeAt(ui->linesListWidget->currentRow());

            Noun nounToAppend(editNounDialog.getText());
            nouns->append(nounToAppend);

            updateGui();

            ui->linesListWidget->setCurrentRow(0);
        }
    }
}

void EditLinesDialog::on_removePushButton_clicked()
{
    //if no line is selected
    if(ui->linesListWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select A Line",
                                 "A line must be selected");
    }
    else
    {
        int tempRow = ui->linesListWidget->currentRow();
        lines->removeAt(tempRow);

        updateGui();

        if(tempRow != ui->linesListWidget->count())
            ui->linesListWidget->setCurrentRow(tempRow);
        else
            ui->linesListWidget->setCurrentRow(tempRow - 1);
    }
}

void EditLinesDialog::on_removeAllPushButton_clicked()
{
    lines->clear();

    updateGui();
}

void EditLinesDialog::on_donePushButton_clicked()
{
    if(! lines->isEmpty())
    {
        QMessageBox::information(this,
                                 "Lines Still Exist",
                                 "You should fix or remove all lines");
    }
    else
    {
        accept();
    }
}
