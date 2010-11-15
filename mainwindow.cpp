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
#include <QTimer>
#include <QString>
#include <QFile>

//implementation-specific data type(s)
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>
#include <QDesktopWidget>
#include "readerrordialog.h"
#include "editnounsdialog.h"

//corresponding header file(s)
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    feedbackAndNounSwitchTimer(new QTimer(this)),
    nouns(new QList<Noun>()),
    nounIndex(0),
    feedbackActive(false),
    nounsFile(new QFile())
{
    //initialization
    ui->setupUi(this);
    feedbackAndNounSwitchTimer->setInterval(800);
    connect(feedbackAndNounSwitchTimer,
            SIGNAL(timeout()),
            SLOT(stopFeedback())
            );
    connect(feedbackAndNounSwitchTimer,
            SIGNAL(timeout()),
            SLOT(displayNewNoun())
            );
    connect(ui->actionEdit_Nouns, SIGNAL(triggered()), SLOT(editNouns()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    nounsFile->setFileName("nouns.txt");

    //center widget in screen
    adjustSize();
    move(QApplication::desktop()->availableGeometry().center() -
         this->rect().center()
         );

    //start the process of reading nouns from the nouns file if it existed
    if(nounsFile->exists())
    {
        //notify the user if the nouns file can't be opened for reading
        if(! nounsFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(0, "Nouns File Open Error",
                                 QString("Cannot open the nouns file '%1' for "
                                         "reading, try again and if the "
                                         "problem persisted please report the "
                                         "bug").arg(nounsFile->fileName())
                );
        }
        else    //read all nouns from the nouns file
        {
            QTextStream in(nounsFile);
            in.setCodec(QTextCodec::codecForName("UTF-8"));
            int lineNumber = 1;

            bool ignoreAll = false;

            while(!in.atEnd())
            {
                QString line = in.readLine();
                if(!line.isEmpty())     //empty lines neither cause errors
                    //nor should be parsed and added
                {
                    if( Noun::isValid(line) )
                    {
                        Noun noun(line);
                        if(nouns->indexOf(noun) == -1)   //if the noun doesn't
                            //already exist in the list
                            nouns->append(noun);     //add it to the list
                    }
                    else
                    {
                        if(ignoreAll == false)
                        {
                            ReadErrorDialog readErrorDialog(lineNumber,
                                                            line,
                                                            nouns,
                                                            0);

                            int result = readErrorDialog.exec();

                            if(result == QDialog::Accepted)
                            {
                                Noun noun = readErrorDialog.getNoun();

                                if(nouns->indexOf(noun) == -1)
                                    nouns->append(noun);
                            }
                            else
                            {
                                if(readErrorDialog.shouldIgnoreAll())
                                    ignoreAll = true;
                            }
                        }
                    }
                }

                lineNumber++;
            }
            nounsFile->close();
        }
    }

    //seed the function qrand()
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //initial GUI state
    updateGui();
}

MainWindow::~MainWindow()
{
    //write nouns into the nouns file
    if(! nounsFile->open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(0,
                             "Nouns File Write Error",
                             QString("Can't open the nouns file '%1' for "
                                     "writing, any modification to nouns will "
                                     "be lost. If the problem persisted please "
                                     "report the bug").arg(
                                             nounsFile->fileName()
                                             )
                             );
    }
    else
    {
        QTextStream out(nounsFile);
        out.setCodec(QTextCodec::codecForName("UTF-8"));

        foreach(Noun noun, *nouns)
            out << noun.toDefiniteArticleAndSingularForm() << "\n";

        nounsFile->close();
    }

    delete ui;
    delete nouns;
}

void MainWindow::updateGui()
{
    if(nouns->isEmpty())
    {
        ui->nounLabel->setText("Add new nouns");

        ui->masculinePushButton->setEnabled(false);
        ui->femininePushButton->setEnabled(false);
        ui->neuterPushButton->setEnabled(false);

        ui->numberOfNounsLabel->setText(QString("Number Of Nouns: 0"));
    }
    else
    {
        displayNewNoun();

        ui->masculinePushButton->setEnabled(true);
        ui->femininePushButton->setEnabled(true);
        ui->neuterPushButton->setEnabled(true);

        //display the number of nouns that are loaded
        ui->numberOfNounsLabel->setText(
                QString("Number Of Nouns: %1").arg(nouns->length())
                );
    }
}

void MainWindow::displayNewNoun()
{
    if( ! nouns->isEmpty() )
    {
        nounIndex = qrand() % nouns->length();
        ui->nounLabel->setText(nouns->at(nounIndex).singularForm);
    }
}

void MainWindow::giveFeedback(Noun::Gender chosenGender)
{
    Noun currentNoun = nouns->at(nounIndex);

    //in case of a mistake, give feedback
    if(currentNoun.gender != chosenGender)
    {
        feedbackActive = true;

        if(currentNoun.gender == Noun::masculine)
        {
            ui->masculinePushButton->setStyleSheet("color: rgb(0, 192, 0);");
            ui->nounLabel->setStyleSheet("color: rgb(0, 192, 0);");
        }
        if(currentNoun.gender == Noun::feminine)
        {
            ui->femininePushButton->setStyleSheet("color: rgb(192, 0, 0);");
            ui->nounLabel->setStyleSheet("color: rgb(192, 0, 0);");
        }
        if(currentNoun.gender == Noun::neuter)
        {
            ui->neuterPushButton->setStyleSheet("color: rgb(0, 0, 192);");
            ui->nounLabel->setStyleSheet("color: rgb(0, 0, 192);");
        }

        feedbackAndNounSwitchTimer->start();
    }
    else
    {
        displayNewNoun();
    }
}

void MainWindow::stopFeedback()
{
    feedbackAndNounSwitchTimer->stop();

    ui->masculinePushButton->setStyleSheet("");
    ui->femininePushButton->setStyleSheet("");
    ui->neuterPushButton->setStyleSheet("");
    ui->nounLabel->setStyleSheet("");

    feedbackActive = false;
}

void MainWindow::on_masculinePushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedback(Noun::masculine);
    }
}

void MainWindow::on_femininePushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedback(Noun::feminine);
    }
}

void MainWindow::on_neuterPushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedback(Noun::neuter);
    }
}

void MainWindow::about()
{
    QMessageBox::information(this, "About GNGT",

                             "German Noun Gender Trainer (GNGT) v0.5 \nCreated "
                             "by Omar Lawand Dalatieh <lawand87@gmail.com>\n\n"

                             "For help, check out the README file.\n\n"

                             "GNGT is licensed under the GNU LGPLv3 license "
                             "(or, at your option, any later version).\n\n"

                             "http://lawand.github.com/gngt/"
                             );
}

void MainWindow::editNouns()
{
    EditNounsDialog editNounsDialog(nouns, this);

    editNounsDialog.exec();

    updateGui();
}
