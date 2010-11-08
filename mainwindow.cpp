/****************************************************************************
**
** Copyright 2010 Omar Lawand Dalatieh.
** Contact: see the README file
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

//implementation-specific data type(s)
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>

//corresponding header file(s)
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    feedbackAndNounSwitchTimer(new QTimer(this)),
    nouns(new QList<Noun>()),
    nounsFileName(new QString("nouns.txt")),
    nounIndex(0)
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

    //create and initialize nounsFile which will be used for
    //reading nouns from the nouns file
    QFile nounsFile(*nounsFileName);

    //if the nouns file exists, read nouns from it
    if(nounsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&nounsFile);
        in.setCodec(QTextCodec::codecForName("UTF-8"));
        int lineNumber = 1;

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
                    QMessageBox::warning(0, "Line Read Error",
                                         QString("Cannot read line number: %1 "
                                                 "of the nouns file "
                                                 "'nouns.txt'. \nThe line "
                                                 "contains: '%2', Whereas "
                                                 "it should contain a "
                                                 "definite article, a space "
                                                 "and the singular form of "
                                                 "the noun. \nExample: "
                                                 "'das Buch' "
                                                 "(without the quotes)").arg(
                                                         lineNumber).arg(
                                                                 line)
                                         );
                }
            }

            lineNumber++;
        }
    }
    else
    {
        QMessageBox::warning(
            0,
            "Nouns File Open Error",
            QString("Cannot open the nouns file %1").arg(*nounsFileName)
            );
    }
    nounsFile.close();

    //seed the function qrand()
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //initial GUI state
    updateGui();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete nouns;
    delete nounsFileName;
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
}

void MainWindow::on_masculinePushButton_clicked()
{
    giveFeedback(Noun::masculine);
}

void MainWindow::on_femininePushButton_clicked()
{
    giveFeedback(Noun::feminine);
}

void MainWindow::on_neuterPushButton_clicked()
{
    giveFeedback(Noun::neuter);
}

void MainWindow::on_aboutPushButton_clicked()
{
    QMessageBox::information(this,
                             "About GNGT",

                             "German Noun Gender Trainer (GNGT) v0.2 \nCreated "
                             "by Omar Lawand Dalatieh <lawand87@gmail.com>\n\n"

                             "For help, check out the README file.\n\n"

                             "GNGT is free software and is licensed under "
                             "the GNU LGPLv3 license (or, at your option, any "
                             "later version).\n\n"

                             "http://lawand.github.com/gngt/"
                             );
}
