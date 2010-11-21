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
#include <QStringList>
#include "editnoundialog.h"
#include "noun.h"

//corresponding header file(s)
#include "editnounsdialog.h"
#include "ui_editnounsdialog.h"

EditNounsDialog::EditNounsDialog(QStringList* erroneousLines,
                                 QList<Noun>* nouns,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounsDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;
    this->erroneousLines = erroneousLines;

    //fix window size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    //initial update
    updateState();
}

EditNounsDialog::~EditNounsDialog()
{
    delete ui;
}

void EditNounsDialog::updateState()
{
    ui->numberOfNounsLabel->setText(
            QString("Number Of Nouns: %2").arg(nouns->length())
            );

    ui->listWidget->clear();

    if(nouns->isEmpty() & erroneousLines->isEmpty())
    {
        ui->editPushButton->setEnabled(false);
        ui->removePushButton->setEnabled(false);
        ui->removeAllPushButton->setEnabled(false);
    }
    else
    {
        foreach(QString line, *erroneousLines)
        {
            QListWidgetItem *qListWidgetItem = new QListWidgetItem(line);
            qListWidgetItem->setTextColor(Qt::red);
            ui->listWidget->addItem(qListWidgetItem);
        }

        qSort(*nouns);

        foreach(Noun noun, *nouns)
            ui->listWidget->addItem(noun.toString());

        ui->editPushButton->setEnabled(true);
        ui->removePushButton->setEnabled(true);
        ui->removeAllPushButton->setEnabled(true);
    }
}

void EditNounsDialog::on_addPushButton_clicked()
{
    EditNounDialog editNounDialog(nouns, this);

    if(editNounDialog.exec() == QDialog::Accepted)
    {
        Noun nounToAppend(editNounDialog.getText());
        nouns->append(nounToAppend);

        updateState();

        ui->listWidget->setCurrentRow(
                nouns->indexOf(nounToAppend) + erroneousLines->length()
                );
    }
}

void EditNounsDialog::on_editPushButton_clicked()
{
    //if nothing is selected
    if(ui->listWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select An Item",
                                 "An item must be selected");
    }
    else
    {
        EditNounDialog editNounDialog(nouns, this);
        editNounDialog.setText(ui->listWidget->currentItem()->text());

        if(editNounDialog.exec() == QDialog::Accepted)
        {
            bool editingANoun = true;
            Noun nounToRemove(ui->listWidget->currentItem()->text());
            if(! nouns->removeOne(nounToRemove))
            {
                editingANoun = false;
                erroneousLines->removeAt(ui->listWidget->currentRow());
            }

            Noun nounToAppend(editNounDialog.getText());
            nouns->append(nounToAppend);

            updateState();

            ui->listWidget->setCurrentRow(
                    nouns->indexOf(nounToAppend) + erroneousLines->length()
                    );
        }
    }
}

void EditNounsDialog::on_removePushButton_clicked()
{
    //if nothing is selected
    if(ui->listWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select An Item",
                                 "An item must be selected");
    }
    else
    {
        int tempRow = ui->listWidget->currentRow();

        Noun nounToRemove(ui->listWidget->currentItem()->text());
        if(! nouns->removeOne(nounToRemove))
            erroneousLines->removeAt(ui->listWidget->currentRow());

        updateState();

        if(tempRow != ui->listWidget->count())
            ui->listWidget->setCurrentRow(tempRow);
        else
            ui->listWidget->setCurrentRow(tempRow - 1);
    }
}

void EditNounsDialog::on_removeAllPushButton_clicked()
{
    if( QMessageBox::question(this,
                              "Are You Sure?",
                              "Are you sure you want to remove all nouns "
                              "erroneous lines? (this operation can't be "
                              "undo-ed)",
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::No
                              ) == QMessageBox::Yes )
        {
            nouns->clear();
            erroneousLines->clear();

            updateState();
        }
}

void EditNounsDialog::on_donePushButton_clicked()
{
    accept();
}
