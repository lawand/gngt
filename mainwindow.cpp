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
#include <QTextEdit>
#include <QDir>
#include "editnounsdialog.h"

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

    //set nouns file name
#ifdef Q_OS_SYMBIAN
    nounsFile->setFileName("c:/data/Others/nouns.txt");
#else
    nounsFile->setFileName("nouns.txt");
#endif

    //center widget in screen and fix window size
#ifndef Q_OS_SYMBIAN
    adjustSize();
    move(QApplication::desktop()->availableGeometry().center() -
         this->rect().center()
         );

    setFixedHeight(size().height());
    setFixedWidth(size().width());
#endif

    //seed the function qrand()
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //disable a default action which text is "Actions", probably realted to
    //context menus. this only appears in symbian
#ifdef Q_OS_SYMBIAN
    ui->masculinePushButton->setContextMenuPolicy(Qt::NoContextMenu);
    ui->femininePushButton->setContextMenuPolicy(Qt::NoContextMenu);
    ui->neuterPushButton->setContextMenuPolicy(Qt::NoContextMenu);
#endif

    //read nouns from nouns file
    readNounsAndErroneousLines();

    //initial GUI state
    updateGui();
}

MainWindow::~MainWindow()
{
    //write nouns into the nouns file
    writeNounsAndErroneousLines();

    //delete data members
    delete ui;
    delete nouns;
    delete erroneousLines;
}

void MainWindow::readNounsAndErroneousLines()
{
    //load a default list of nouns if the nouns file doesn't exist
    if(! nounsFile->exists())
    {
        nouns->append(Noun("das Auto"));
        nouns->append(Noun("der Bahnhof"));
        nouns->append(Noun("die Bank"));
        nouns->append(Noun("der Baum"));
        nouns->append(Noun("das Buch"));
        nouns->append(Noun("die Frau"));
        nouns->append(Noun("das Geld"));
        nouns->append(Noun("die Gesundheit"));
        nouns->append(Noun("der Mann"));
        nouns->append(Noun("die Natur"));
        nouns->append(Noun("der Sohn"));
        nouns->append(Noun("die Sonne"));
        nouns->append(Noun("der Tisch"));
        nouns->append(Noun("der Vater"));
        nouns->append(Noun("das Wasser"));
        nouns->append(Noun("das Wort"));

        return;
    }

    //if the nouns file can't be opened for reading, notify the user
    if(! nounsFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *qMessageBox = new QMessageBox(this);
        qMessageBox->setWindowTitle("File Open Error");

        //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
        qMessageBox->setText(
                QString("Cannot open the nouns file '%1' for reading. \n"
                        "Try again and if the problem persists, please report "
                        "the bug.").arg(nounsFile->fileName())
                );
#else
        //must use setInformativeText as well as setText on symbian because
        //setInformativeText allows large text to have a scrollbar
        qMessageBox->setText("Cannot open the nouns file.");
        qMessageBox->setInformativeText(
                QString("Cannot open the nouns file '%1' for reading. \n\n"
                        "Try again and if the problem persists, please report "
                        "the bug.").arg(nounsFile->fileName())
                );
#endif

        //set QMessageBox icon
        qMessageBox->setIcon(QMessageBox::Warning);

        //execute the QMessageBox
#ifdef Q_OS_SYMBIAN
        //delay displaying the QMessageBox on Symbian until after the
        //widget is shown, because of a bug
        QTimer::singleShot(0, qMessageBox, SLOT(exec()));
#else
        qMessageBox->exec();
#endif

        return;
    }

    //read all nouns from the nouns file
    QTextStream in(nounsFile);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while(!in.atEnd())
    {
        QString line = in.readLine();
        int memorizationStreak = 0;

        QStringList parts = line.split(QRegExp("\\s+"),
                                       QString::SkipEmptyParts
                                       );

        if(!parts.isEmpty())     //empty or white-space-only lines
            //neither cause errors
            //nor should be parsed and added
        {

            if( QRegExp("^[0-9]$").exactMatch(parts.last()) )
            {
                memorizationStreak = parts.last().toInt();
                parts.removeLast();
            }

            QString partialLine = parts.join(" ");

            if( Noun::isValid(partialLine) )
            {
                Noun noun(partialLine, memorizationStreak);
                if(nouns->indexOf(noun) == -1)   //if the noun doesn't
                    //already exist in the list
                    nouns->append(noun);     //add it to the list
            }
            else
            {
                erroneousLines->append(partialLine);
            }
        }
    }
    nounsFile->close();

    //give warning about erroneous lines
    if(erroneousLines->isEmpty() == false)
    {
        QMessageBox *qMessageBox = new QMessageBox(this);
        qMessageBox->setWindowTitle("Read Error");

        //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
        qMessageBox->setText(
                "Some lines contained errors and weren't able to be read. \n"
                "This is due to these lines not being formatted properly. \n"
                "Correct these errors using the 'Edit Nouns' option. \n"
                "<Memorization Streaks are reset to 0>"
                );
#else
        //must use setInformativeText as well as setText on symbian because
        //setInformativeText allows large text to have a scrollbar
        qMessageBox->setText(
                "Some lines contained errors and weren't able to be read."
                );
        qMessageBox->setInformativeText(
                "This is due to these lines not being formatted properly. \n\n"
                "Correct these errors using the 'Edit Nouns' option. \n\n"
                "<Memorization Streaks are reset to 0>"
                );
#endif

        //set QMessageBox icon
        qMessageBox->setIcon(QMessageBox::Information);

        //execute the QMessageBox
#ifdef Q_OS_SYMBIAN
        //delay displaying the QMessageBox on Symbian until after the
        //widget is shown, because of a bug
        QTimer::singleShot(0, qMessageBox, SLOT(exec()));
#else
        qMessageBox->exec();
#endif
    }
}

void MainWindow::writeNounsAndErroneousLines()
{
    //create "C:/data/Others/" if it didn't exist
#ifdef Q_OS_SYMBIAN
    QDir data("c:/data/");
    if(data.exists("Others") == false)
        data.mkdir("Others");
#endif

//if the nouns file can't be opened for writing, notify the user
    if(! nounsFile->open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox qMessageBox(this);
        qMessageBox.setWindowTitle("File Open Error");

        //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
        qMessageBox.setText(
                QString("Cannot open the nouns file '%1' for writing. \n"
                        "Any modifications to nouns will be lost. \n"
                        "If the problem persists, please report the bug.").arg(
                                nounsFile->fileName()
                                )
                );
#else
        //must use setInformativeText as well as setText on symbian because
        //setInformativeText allows large text to have a scrollbar
        qMessageBox.setText("Cannot open the nouns file.");
        qMessageBox.setInformativeText(
                QString("Cannot open the nouns file '%1' for writing. \n\n"
                        "Any modifications to nouns will be lost. \n\n"
                        "If the problem persists, please report the bug.").arg(
                                nounsFile->fileName()
                                )
                );
#endif
        //set QMessageBox icon
        qMessageBox.setIcon(QMessageBox::Warning);

        //execute the QMessageBox
        qMessageBox.exec();

        return;
    }

    //write nouns into the nouns file

    QTextStream out(nounsFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    int largestLength = 0;
    foreach(Noun noun, *nouns)
    {
        if(noun.singularForm.length() > largestLength)
            largestLength = noun.singularForm.length();
    }

    //write nouns
    foreach(Noun noun, *nouns)
        out << qSetFieldWidth(largestLength + 5) << left << noun.toString()
            << qSetFieldWidth(1) << noun.memorizationStreak << "\n";

    out << "\n";

    //write erroneous lines
    foreach(QString line, *erroneousLines)
        out << line << " " << 0 << "\n";

    nounsFile->close();
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
            currentNoun.memorizationStreak = 0;
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
                        currentNoun.memorizationStreak + 1
                        )
                );
        ui->memorizationStreakLabel->setStyleSheet("color: rgb(0, 192, 0);");

        currentNoun.memorizationStreak++;

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
    QMessageBox qMessageBox(this);
    qMessageBox.setWindowTitle("About");

    //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
    qMessageBox.setText(
            "German Noun Gender Trainer (GNGT) v0.7 \n"
            "Created by Omar Lawand Dalatieh <lawand87@gmail.com>\n\n"

            "For help, check out the README file.\n\n"

            "GNGT is licensed under the GNU LGPLv3 license \n"
            "(or, at your option, any later version).\n\n"

            "http://lawand.github.com/gngt/"
            );
#else
    //must use setInformativeText as well as setText on symbian because
    //setInformativeText allows large text to have a scrollbar
    qMessageBox.setText("German Noun Gender Trainer (GNGT) v0.7");
    qMessageBox.setInformativeText(
            "Created by Omar Lawand Dalatieh "
            "<lawand87@gmail.com>\n\n"

            "For help, check out the README file.\n\n"

            "GNGT is licensed under the GNU LGPLv3 license "
            "(or, at your option, any later version).\n\n"

            "http://lawand.github.com/gngt/"
            );
#endif

    //set QMessageBox icon
    QIcon applicationIcon(":/icons/applicationIcon.png");
    qMessageBox.setIconPixmap(applicationIcon.pixmap(QSize(32, 32)));

    //execute the QMessageBox
    qMessageBox.exec();
}

void MainWindow::editNouns()
{
    EditNounsDialog editNounsDialog(erroneousLines, nouns, this);

    editNounsDialog.exec();

    updateGui();

    writeNounsAndErroneousLines();
}
