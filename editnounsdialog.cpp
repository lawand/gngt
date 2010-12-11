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
#include <QMenu>
#include <QAction>
#include <QKeyEvent>

//implementation-specific data type(s)
#include <QMessageBox>
#include <QStringList>
#include <QDesktopWidget>
#include "editnoundialog.h"
#include "noun.h"

//corresponding header file(s)
#include "editnounsdialog.h"
#include "ui_editnounsdialog.h"

EditNounsDialog::EditNounsDialog(QStringList* erroneousLines,
                                 QList<Noun>* nouns,
                                 QString nounsFileFileName,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounsDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;
    this->erroneousLines = erroneousLines;

    //set text of nounsFileFileNameLabel to mention path and fileName of
    //the nouns file
    ui->nounsFileLabel->setText(nounsFileFileName + " :");

    //establish connections of buttons
    connect(ui->addPushButton, SIGNAL(clicked()), SLOT(add()));
    connect(ui->editPushButton, SIGNAL(clicked()), SLOT(edit()));
    connect(ui->removePushButton, SIGNAL(clicked()), SLOT(remove()));
    connect(ui->removeAllPushButton, SIGNAL(clicked()), SLOT(removeAll()));
    connect(ui->donePushButton, SIGNAL(clicked()), SLOT(accept()));

    //establish connections of actions
    connect(ui->actionAdd, SIGNAL(triggered()), SLOT(add()));
    connect(ui->actionEdit, SIGNAL(triggered()), SLOT(edit()));
    connect(ui->actionRemove, SIGNAL(triggered()), SLOT(remove()));
    connect(ui->actionRemove_All, SIGNAL(triggered()), SLOT(removeAll()));
    connect(ui->actionDone, SIGNAL(triggered()), SLOT(accept()));

    //manage buttons and/or actions
#ifdef Q_OS_SYMBIAN
    //initialize softkey commands for S60
    {
        qMenu = new QMenu(this);
        qMenu->addAction(ui->actionAdd);
        qMenu->addAction(ui->actionEdit);
        qMenu->addAction(ui->actionRemove);
        qMenu->addAction(ui->actionRemove_All);

        ui->actionOptions->setMenu(qMenu);
        ui->actionOptions->setSoftKeyRole(QAction::PositiveSoftKey);
        addAction(ui->actionOptions);

        ui->actionDone->setSoftKeyRole(QAction::NegativeSoftKey);
        addAction(ui->actionDone);
    }

    //if a touch screen is available, activate buttons, else actions
    if(qApp->navigationMode() == Qt::NavigationModeNone)
    {
        setQPushButtonsVisible(true);
        setQActionsVisible(false);

        ui->actionDone->setVisible(true);
        ui->donePushButton->setVisible(false);
    }
    else
    {
        setQPushButtonsVisible(false);
        setQActionsVisible(true);
    }

    showMaximized();
#else
    setQPushButtonsVisible(true);
    setQActionsVisible(false);
#endif

    //fix window size
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    layout()->setSizeConstraint(QLayout::SetFixedSize);
#else
    layout()->setSizeConstraint(QLayout::SetFixedSize);
#endif

    //initial update
    updateState();

    //disable a default action which text is "Actions", probably realted to
    //context menus. this only appears in symbian
#ifdef Q_OS_SYMBIAN
    ui->listWidget->setContextMenuPolicy(Qt::NoContextMenu);
#endif

#ifdef Q_WS_MAEMO_5
    //the done push button is not needed in maemo
    delete ui->donePushButton;

    //change the layout to make it horizontal
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    if (screenGeometry.width() > screenGeometry.height())
    {
        layout()->removeWidget(ui->addPushButton);
        layout()->removeWidget(ui->editPushButton);
        layout()->removeWidget(ui->removePushButton);
        layout()->removeWidget(ui->removeAllPushButton);

        QVBoxLayout *newRightLayout = new QVBoxLayout();
        newRightLayout->addWidget(ui->addPushButton);
        newRightLayout->addWidget(ui->editPushButton);
        newRightLayout->addWidget(ui->removePushButton);
        newRightLayout->addWidget(ui->removeAllPushButton);

        layout()->removeWidget(ui->nounsFileLabel);
        layout()->removeWidget(ui->listWidget);
        layout()->removeWidget(ui->numberOfNounsLabel);

        QVBoxLayout *newLeftLayout = new QVBoxLayout();
        newLeftLayout->addWidget(ui->nounsFileLabel);
        newLeftLayout->addWidget(ui->listWidget);
        newLeftLayout->addWidget(ui->numberOfNounsLabel);

        QHBoxLayout *newMainLayout = new QHBoxLayout();
        newMainLayout->addLayout(newLeftLayout);
        newMainLayout->addLayout(newRightLayout);

        delete this->layout();
        this->setLayout(newMainLayout);

        ui->addPushButton->setFocus();
    }
#endif
}

EditNounsDialog::~EditNounsDialog()
{
    //delete data members
    delete ui;
}

void EditNounsDialog::updateState()
{
    //update numberOfNounsLabel
    ui->numberOfNounsLabel->setText(
            QString("Number Of Nouns: %2").arg(nouns->length())
            );

    //clear listWidget because items will be added later (if they existed)
    ui->listWidget->clear();

    if(nouns->isEmpty() & erroneousLines->isEmpty())
    {
        //disable buttons and/or actions
#ifdef Q_OS_SYMBIAN
        //if a touch screen is available, disable buttons, else actions
        if(qApp->navigationMode() == Qt::NavigationModeNone)
        {
            ui->editPushButton->setEnabled(false);
            ui->removePushButton->setEnabled(false);
            ui->removeAllPushButton->setEnabled(false);
        }
        else
        {
            ui->actionEdit->setVisible(false);
            ui->actionRemove->setVisible(false);
            ui->actionRemove_All->setVisible(false);
        }
#else
        ui->editPushButton->setEnabled(false);
        ui->removePushButton->setEnabled(false);
        ui->removeAllPushButton->setEnabled(false);
#endif
    }
    else
    {
        //add lines to listWidget
        foreach(QString line, *erroneousLines)
        {
            QListWidgetItem *qListWidgetItem = new QListWidgetItem(line);
            qListWidgetItem->setTextColor(Qt::red);
            ui->listWidget->addItem(qListWidgetItem);
        }

        //sort nouns
        qSort(*nouns);

        //add nouns to listWidget
        foreach(Noun noun, *nouns)
            ui->listWidget->addItem(noun.toString());

        //enable buttons and/or actions
#ifdef Q_OS_SYMBIAN
        //if a touch screen is available, enable buttons, else actions
        if(qApp->navigationMode() == Qt::NavigationModeNone)
        {
            ui->editPushButton->setEnabled(true);
            ui->removePushButton->setEnabled(true);
            ui->removeAllPushButton->setEnabled(true);
        }
        else
        {
            ui->actionEdit->setVisible(true);
            ui->actionRemove->setVisible(true);
            ui->actionRemove_All->setVisible(true);
        }
#else
        ui->editPushButton->setEnabled(true);
        ui->removePushButton->setEnabled(true);
        ui->removeAllPushButton->setEnabled(true);
#endif
    }
}

void EditNounsDialog::setQPushButtonsVisible(bool visible)
{
    ui->addPushButton->setVisible(visible);
    ui->editPushButton->setVisible(visible);
    ui->removePushButton->setVisible(visible);
    ui->removeAllPushButton->setVisible(visible);
    ui->donePushButton->setVisible(visible);
}

void EditNounsDialog::setQActionsVisible(bool visible)
{
    ui->actionOptions->setVisible(visible);
    ui->actionDone->setVisible(visible);
}

void EditNounsDialog::add()
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

void EditNounsDialog::edit()
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

void EditNounsDialog::remove()
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

void EditNounsDialog::removeAll()
{
    if(!nouns->isEmpty() || !erroneousLines->isEmpty())
    {
        if( QMessageBox::question(this,
                                  "Are You Sure?",
                                  "Are you sure you want to remove all nouns "
                                  "and erroneous lines? (this operation can't "
                                  "be undo-ed)",
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::No
                                  ) == QMessageBox::Yes )
            {
                nouns->clear();
                erroneousLines->clear();

                updateState();
            }
    }
}

void EditNounsDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef Q_OS_SYMBIAN
    case Qt::Key_1:
        add();
        break;

    case Qt::Key_2:
        if(!nouns->isEmpty() || !erroneousLines->isEmpty())
            edit();
        break;

    case Qt::Key_3:
    case Qt::Key_Backspace:
        if(!nouns->isEmpty() || !erroneousLines->isEmpty())
            remove();
        break;

    case Qt::Key_4:
        if(!nouns->isEmpty() || !erroneousLines->isEmpty())
            removeAll();
        break;
#else
    case Qt::Key_Delete:
        if(!nouns->isEmpty() || !erroneousLines->isEmpty())
            remove();
        break;
#endif

    default:
        QWidget::keyPressEvent(e);
        break;
    }
}
