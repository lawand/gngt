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

EditNounsDialog::EditNounsDialog(QList<Noun>* nouns, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounsDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;

    //fix window size
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    //initial update
    updateGui();
}

EditNounsDialog::~EditNounsDialog()
{
    delete ui;
}

void EditNounsDialog::updateGui()
{
    ui->numberOfNounsLabel->setText(
            QString("Number Of Nouns: %2").arg(nouns->length())
            );

    ui->nounsListWidget->clear();

    if(!nouns->isEmpty())
    {
        QStringList nounStrings;

        foreach(Noun noun, *nouns)
            nounStrings.append(
                    noun.toSortingFriendlyDefiniteArticleAndSingularForm()
                    );

        nounStrings.sort();

        ui->nounsListWidget->addItems(nounStrings);

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

void EditNounsDialog::on_addPushButton_clicked()
{
    EditNounDialog editNounDialog(nouns, this);

    if(editNounDialog.exec() == QDialog::Accepted)
    {
        Noun noun;
        noun.fromDefiniteArticleAndSingularForm(editNounDialog.getText());

        nouns->append(noun);

        updateGui();
    }
}

void EditNounsDialog::on_editPushButton_clicked()
{
    //if no noun is selected
    if(ui->nounsListWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select A Noun",
                                 "A noun must be selected");
    }
    else
    {
        EditNounDialog editNounDialog(nouns);
        Noun nounToEdit;
        nounToEdit.fromSortingFriendlyDefiniteArticleAndSingularForm(
                ui->nounsListWidget->currentItem()->text()
                );
        editNounDialog.setText(nounToEdit.toDefiniteArticleAndSingularForm());

        if(editNounDialog.exec() == QDialog::Accepted)
        {
            Noun nounToReplace;
            nounToReplace.fromSortingFriendlyDefiniteArticleAndSingularForm(
                    ui->nounsListWidget->currentItem()->text()
                    );

            int index = nouns->indexOf(nounToReplace);

            Noun nounToInsert;
            nounToInsert.fromDefiniteArticleAndSingularForm(
                    editNounDialog.getText()
                    );
            nouns->replace(index, nounToInsert);
        }

        updateGui();
    }
}

void EditNounsDialog::on_removePushButton_clicked()
{
    //if no noun is selected
    if(ui->nounsListWidget->currentRow() == -1)
    {
        QMessageBox::information(this,
                                 "Select A Noun",
                                 "A noun must be selected");
    }
    else
    {
        Noun nounToRemove;
        nounToRemove.fromSortingFriendlyDefiniteArticleAndSingularForm(
                ui->nounsListWidget->currentItem()->text()
                );
        nouns->removeAt(nouns->indexOf(nounToRemove));

        updateGui();
    }
}

void EditNounsDialog::on_removeAllPushButton_clicked()
{
    nouns->clear();

    updateGui();
}
