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
#include <QFile>

//implementation-specific data type(s)
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>
#include <QDesktopWidget>
#include <QTimer>
#include <QString>
#include "editnounsdialog.h"
#include "editlinesdialog.h"

//corresponding header file(s)
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nouns(new QList<Noun>()),
    erroneousLines(new QStringList()),
    nounIndex(0),
    nounsFile(new QFile()),
    feedbackActive(false)
{
    //initialization
    ui->setupUi(this);
    connect(ui->actionEdit_Nouns, SIGNAL(triggered()), SLOT(editNouns()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));

#ifdef Q_OS_SYMBIAN
    nounsFile->setFileName("c:/data/Others/nouns.txt");
#else
    nounsFile->setFileName("nouns.txt");
#endif

    //center widget in screen
    adjustSize();
    move(QApplication::desktop()->availableGeometry().center() -
         this->rect().center()
         );

    //fix window size
    setFixedHeight(size().height());
    setFixedWidth(size().width());

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

            while(!in.atEnd())
            {
                QString line = in.readLine();
                int memorizationStreak = 0;

                if(!line.isEmpty())     //empty lines neither cause errors
                    //nor should be parsed and added
                {
                    QStringList parts = line.split(QRegExp("\\s+"),
                                                   QString::SkipEmptyParts
                                                   );

                    if( QRegExp("^[0-9]$").exactMatch(parts.last()) )
                    {
                        memorizationStreak = parts.last().toInt();
                    }

                    if(parts.length() > 2)
                        parts.removeLast();

                    QString nounPartOfTheLine = parts.join(" ");

                    if( Noun::isValid(nounPartOfTheLine) )
                    {
                        Noun noun(nounPartOfTheLine, memorizationStreak);
                        if(nouns->indexOf(noun) == -1)   //if the noun doesn't
                            //already exist in the list
                            nouns->append(noun);     //add it to the list
                    }
                    else
                    {
                        erroneousLines->append(line);
                    }
                }
            }
            nounsFile->close();
        }
    }

    if(! erroneousLines->isEmpty())
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(0,
                                       "Correct Erroneous Lines?",
                                       "Some lines contained errors and "
                                       "weren't able to be read. Memorization "
                                       "Streaks are reset to 0. Do you want to "
                                       "fix the nouns? (not doing so leads to "
                                       "deleting them)",
                                       QMessageBox::Yes|QMessageBox::No,
                                       QMessageBox::Yes
                                       );
        if(result == QMessageBox::Yes)
        {
            EditLinesDialog editLinesDialog(erroneousLines, nouns);
            editLinesDialog.exec();
            qSort(*nouns);
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

        int largestLength = 0;
        foreach(Noun noun, *nouns)
        {
            if(noun.singularForm.length() > largestLength)
                largestLength = noun.singularForm.length();
        }

        foreach(Noun noun, *nouns)
            out << qSetFieldWidth(largestLength + 5) << left << noun.toString()
                << qSetFieldWidth(1) << noun.memorizationStreak << "\n";

        nounsFile->close();
    }

    delete ui;
    delete nouns;
    delete erroneousLines;
}

void MainWindow::updateGui()
{
    if(nouns->isEmpty())
    {
        ui->nounLabel->setText("Add new nouns");

        ui->masculinePushButton->setEnabled(false);
        ui->femininePushButton->setEnabled(false);
        ui->neuterPushButton->setEnabled(false);

        ui->memorizationStreakLabel->setText("");
    }
    else
    {
        displayNewNoun();

        ui->masculinePushButton->setEnabled(true);
        ui->femininePushButton->setEnabled(true);
        ui->neuterPushButton->setEnabled(true);
    }
}

void MainWindow::displayNewNoun()
{
    if( ! nouns->isEmpty() )
    {
        nounIndex = qrand() % nouns->length();

        ui->memorizationStreakLabel->setText(
                QString("Streak: %1").arg(
                        nouns->at(nounIndex).memorizationStreak
                        )
                );

        ui->nounLabel->setText(nouns->at(nounIndex).singularForm);
    }
}

void MainWindow::giveFeedbackAndUpdateNouns(Noun::Gender chosenGender)
{
    feedbackActive = true;

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

        if(currentNoun.memorizationStreak != 0)
        {
            currentNoun.setMemorizationStreak(0);
            ui->memorizationStreakLabel->setText("Streak: 0");
            ui->memorizationStreakLabel->setStyleSheet(
                    "color: rgb(192, 0, 0);"
                    );

            nouns->replace(nouns->indexOf(currentNoun), currentNoun);
        }
    }
    else
    {
        ui->memorizationStreakLabel->setText(
                QString("Streak: %1").arg(
                        currentNoun.getMemorizationStreak() + 1
                        )
                );
        ui->memorizationStreakLabel->setStyleSheet("color: rgb(0, 192, 0);");

        currentNoun.setMemorizationStreak(
                currentNoun.getMemorizationStreak() + 1
                );

        if(currentNoun.memorizationStreak == 10)
        {
            ui->memorizationStreakLabel->setText(
                    "Noun memorized! \n" +
                    ui->memorizationStreakLabel->text()
                    );

            nouns->removeAt(nouns->indexOf(currentNoun));

            QTimer::singleShot(1500, this, SLOT(stopFeedback()));
            QTimer::singleShot(1500, this, SLOT(updateGui()));
            return;
        }
        else
        {
            nouns->replace(nouns->indexOf(currentNoun), currentNoun);
        }
    }

    QTimer::singleShot(800, this, SLOT(stopFeedback()));
    QTimer::singleShot(800, this, SLOT(updateGui()));
}

void MainWindow::stopFeedback()
{
    feedbackActive = false;

    ui->masculinePushButton->setStyleSheet("");
    ui->femininePushButton->setStyleSheet("");
    ui->neuterPushButton->setStyleSheet("");
    ui->nounLabel->setStyleSheet("");
    ui->memorizationStreakLabel->setStyleSheet("");
}

void MainWindow::on_masculinePushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedbackAndUpdateNouns(Noun::masculine);
    }
}

void MainWindow::on_femininePushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedbackAndUpdateNouns(Noun::feminine);
    }
}

void MainWindow::on_neuterPushButton_clicked()
{
    if(!feedbackActive)
    {
        giveFeedbackAndUpdateNouns(Noun::neuter);
    }
}

void MainWindow::about()
{
    QMessageBox::information(this, "About GNGT",

                             "German Noun Gender Trainer (GNGT) v0.7 \nCreated "
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
