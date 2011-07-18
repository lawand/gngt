/****************************************************************************
**
** Copyright 2010, 2011 Omar Lawand Dalatieh.
**
** This file is part of GNGT.
**
** GNGT is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GNGT is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with GNGT. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

//implementation-specific data type(s)
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>
#include <QDesktopWidget>
#include <QTimer>
#include <QDir>
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QSettings>
#include "editnounsdialog.h"

//corresponding header file(s)
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString language, QWidget *parent) :
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
    connect(ui->actionReadme, SIGNAL(triggered()), SLOT(readme()));
    connect(ui->actionCopying, SIGNAL(triggered()), SLOT(copying()));
    connect(ui->actionQtSingleApplication_Copying, SIGNAL(triggered()),
            SLOT(qtsingleapplicationCopying()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    this->language = language;

    //enable auto-rotation on maemo linux
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif

    //set nouns file name
#if defined(Q_OS_SYMBIAN)
    nounsFile->setFileName("c:/data/Others/nouns.txt");
#elif defined(Q_WS_MAEMO_5)
    nounsFile->setFileName("/home/user/MyDocs/Others/nouns.txt");
#elif defined(Q_OS_LINUX)
    nounsFile->setFileName(QDir::home().absolutePath() + "/.gngt/nouns.txt");
#elif defined(Q_OS_WIN32)
    nounsFile->setFileName("nouns.txt");
#endif

    //center widget in screen and fix window size
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    //do nothing
#else
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

    //add language actions into the language menu and set them up
    setupLanguageActions();

    //translation-related function call
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
}

MainWindow::~MainWindow()
{
    //write nouns into the nouns file
    writeNounsAndErroneousLines();

    //write preferred language into settings
    QSettings settings;
    settings.setValue("config/language", language);

    //delete data members
    delete ui;
    delete nouns;
    delete erroneousLines;
    delete nounsFile;
}

void MainWindow::setupLanguageActions()
{
    QAction* actionEnglish = new QAction(this);
    actionEnglish->setText(tr("&English"));
    ui->menuLanguage->addAction(actionEnglish);
    connect(actionEnglish, SIGNAL(triggered()), SLOT(setLanguageEnglish()));

    QAction* actionCzech = new QAction(this);
    actionCzech->setText(tr("&Czech"));
    ui->menuLanguage->addAction(actionCzech);
    connect(actionCzech, SIGNAL(triggered()), SLOT(setLanguageCzech()));

    if(language == "en")
    {
        actionEnglish->setEnabled(false);
    }
    if(language == "cs")
    {
        actionCzech->setEnabled(false);
    }
}

void MainWindow::setLanguageEnglish()
{
    language = "en";

    QMessageBox qMessageBox;
    qMessageBox.setWindowTitle(tr("Restart Required"));
    qMessageBox.setIcon(QMessageBox::Information);
    qMessageBox.setText(
                tr("The language will change the next time you start GNGT.")
                );
    qMessageBox.exec();
}

void MainWindow::setLanguageCzech()
{
    language = "cs";

    QMessageBox qMessageBox;
    qMessageBox.setWindowTitle(tr("Restart Required"));
    qMessageBox.setIcon(QMessageBox::Information);
    qMessageBox.setText(
                tr("The language will change the next time you start GNGT.")
                );
    qMessageBox.exec();
}

void MainWindow::readNounsAndErroneousLines()
{
    //check for the nouns file
    if(! nounsFile->exists())
    {
        return;
    }

    //if the nouns file can't be opened for reading, notify the user
    if(! nounsFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *qMessageBox = new QMessageBox(this);
        qMessageBox->setWindowTitle(
                    tr("File Open Error")
                    );

        //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
        qMessageBox->setText(
                QString(
                        tr("Cannot open the nouns file '%1' for reading.")
                        ).arg(nounsFile->fileName())
                );
#else
        if(QApplication::desktop()->screenGeometry().width() <= 240)
            //on devices with 240x320 screen resolution
        {
            qMessageBox->setText(
                    tr("Cannot open the nouns file.")
                    );

            qMessageBox->setInformativeText(
                    QString(
                            tr("Cannot open the nouns file '%1' for reading.")
                            ).arg(nounsFile->fileName())
                    );
        }
        else
            //on other Symbian devices
        {
            qMessageBox->setText(
                    QString(
                            tr("Cannot open the nouns file '%1' for reading.")
                            ).arg(nounsFile->fileName())
                    );
        }
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
        QStringList lineParts = line.split(
                    QRegExp("\\s+"),
                    QString::SkipEmptyParts
                    );

        if(!lineParts.isEmpty())     //empty or white-space-only lines
            //neither cause errors
            //nor should be parsed and added
        {
            int memorizationStreak;
            if( !QRegExp("^[0-9]$").exactMatch(lineParts.last()) )
            {
                memorizationStreak = 0;
            }
            else
            {
                memorizationStreak = lineParts.last().toInt();
                lineParts.removeLast();
            }

            QString partialLine = lineParts.join(" ");

            if( !Noun::isValid(partialLine) )
            {
                erroneousLines->append(line);
            }
            else
            {
                Noun noun(partialLine, memorizationStreak);
                if(nouns->indexOf(noun) == -1)   //if the noun doesn't
                    //already exist in the list
                    nouns->append(noun);     //add it to the list
            }
        }
    }
    nounsFile->close();

    //give warning about erroneous lines
    if(erroneousLines->isEmpty() == false)
    {
        QMessageBox *qMessageBox = new QMessageBox(this);
        qMessageBox->setWindowTitle(
                    tr("Read Error")
                    );

        //set the text of the QMessageBox
        qMessageBox->setText(
                tr("Some lines contained errors and weren't able to be read. \n"
                "Correct these errors using the 'Edit Nouns' option.")
                );

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
    }
}

void MainWindow::writeNounsAndErroneousLines()
{
#ifdef Q_OS_SYMBIAN
    //create "C:/data/Others/" if it didn't exist
    QDir data("c:/data/");
    if(data.exists("Others") == false)
        data.mkdir("Others");
#endif

#ifdef Q_OS_LINUX
    //create "~/.gngt/" if it didn't exist
    QDir home(QDir::home().absolutePath());
    if(home.exists(".gngt") == false)
        home.mkdir(".gngt");
#endif

#ifdef Q_WS_MAEMO_5
    //create "/home/user/MyDocs/Others/" if it didn't exist
    QDir data("/home/user/MyDocs/");
    if(data.exists("Others") == false)
        data.mkdir("Others");
#endif

    //if the nouns file can't be opened for writing, notify the user
    if(! nounsFile->open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox qMessageBox(this);
        qMessageBox.setWindowTitle(
                    tr("File Open Error")
                    );

        //set the text of the QMessageBox
#ifndef Q_OS_SYMBIAN
        qMessageBox.setText(
                QString(
                        tr("Cannot open the nouns file '%1' for writing.")
                        ).arg(nounsFile->fileName())
                );
#else
        if(QApplication::desktop()->screenGeometry().width() <= 240)
            //on devices with 240x320 screen resolution
        {
            qMessageBox.setText(
                    tr("Cannot open the nouns file.")
                    );

            qMessageBox.setInformativeText(
                    QString(
                            tr("Cannot open the nouns file '%1' for writing.")
                            ).arg(nounsFile->fileName())
                    );
        }
        else
            //on other Symbian devices
        {
            qMessageBox.setText(
                    QString(
                            tr("Cannot open the nouns file '%1' for writing.")
                            ).arg(nounsFile->fileName())
                    );
        }
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
        if(noun.toString().length() > largestLength)
            largestLength = noun.toString().length();
    }

    //write nouns
    foreach(Noun noun, *nouns)
        out << qSetFieldWidth(largestLength + 1) << left << noun.toString()
            << qSetFieldWidth(1) << noun.memorizationStreak << "\n";

    out << "\n";

    //write erroneous lines
    foreach(QString line, *erroneousLines)
        out << line << "\n";

    nounsFile->close();
}

void MainWindow::updateGui()
{
    if(nouns->isEmpty())
    {
        ui->singularFormLabel->setText(
                    tr("Add new nouns")
                    );
        ui->pluralFormLabel->setText("");
        ui->memorizationStreakLabel->setText("");

        ui->masculinePushButton->setEnabled(false);
        ui->femininePushButton->setEnabled(false);
        ui->neuterPushButton->setEnabled(false);
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

        ui->singularFormLabel->setText(nouns->at(nounIndex).singularForm);
        if(nouns->at(nounIndex).pluralForm.isEmpty())
        {
            ui->pluralFormLabel->setText("");
        }
        else
        {
            ui->pluralFormLabel->setText(
                    "(" +
                    nouns->at(nounIndex).pluralForm +
                    ")"
                    );
        }

        ui->memorizationStreakLabel->setText(
                QString(tr("Streak: %1")).arg(
                        nouns->at(nounIndex).memorizationStreak
                        )
                );
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
            ui->masculinePushButton->setStyleSheet("color: green;");
            ui->singularFormLabel->setStyleSheet("color: green;");
        }
        if(currentNoun.gender == Noun::feminine)
        {
            ui->femininePushButton->setStyleSheet("color: red;");
            ui->singularFormLabel->setStyleSheet("color: red;");
        }
        if(currentNoun.gender == Noun::neuter)
        {
            ui->neuterPushButton->setStyleSheet("color: blue;");
            ui->singularFormLabel->setStyleSheet("color: blue;");
        }

        if(currentNoun.memorizationStreak != 0)
        {
            currentNoun.memorizationStreak = 0;
            ui->memorizationStreakLabel->setText(
                        tr("Streak: 0")
                        );
            ui->memorizationStreakLabel->setStyleSheet(
                    "color: red;"
                    );

            nouns->replace(nouns->indexOf(currentNoun), currentNoun);
        }
    }
    else
    {
        ui->memorizationStreakLabel->setText(
                QString(tr("Streak: %1")).arg(
                        currentNoun.memorizationStreak + 1
                        )
                );
        ui->memorizationStreakLabel->setStyleSheet("color: green;");

        currentNoun.memorizationStreak++;

        if(currentNoun.memorizationStreak == 10)
        {
            ui->memorizationStreakLabel->setText(
                    tr("Noun memorized! \n") +
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
    ui->singularFormLabel->setStyleSheet("");
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

void MainWindow::readme()
{
    QDialog dialog(this);

    dialog.setWindowTitle("Readme");

    QFile readmeFile(":/help/readme.txt");
    readmeFile.open(QFile::ReadOnly);
    QString readmeString = readmeFile.readAll();

    QTextBrowser textBrowser;
    textBrowser.setText(readmeString);

    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Ok);
    buttonBox.setCenterButtons(true);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setLayout(new QVBoxLayout());
    dialog.layout()->addWidget(&textBrowser);
    dialog.layout()->addWidget(&buttonBox);

    dialog.exec();
}

void MainWindow::copying()
{
    QDialog dialog(this);

    dialog.setWindowTitle("Copying");

    QFile copyingFile(":/help/copying.txt");
    copyingFile.open(QFile::ReadOnly);
    QString copyingString = copyingFile.readAll();

    QTextBrowser textBrowser;
    textBrowser.setText(copyingString);

    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Ok);
    buttonBox.setCenterButtons(true);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setLayout(new QVBoxLayout());
    dialog.layout()->addWidget(&textBrowser);
    dialog.layout()->addWidget(&buttonBox);

    dialog.exec();
}

void MainWindow::qtsingleapplicationCopying()
{
    QDialog dialog(this);

    dialog.setWindowTitle("QtSingleApplication/Copying");

    QFile qtsingleapplicationCopyingFile(
                ":/help/qtsingleapplication/COPYING.txt"
                );
    qtsingleapplicationCopyingFile.open(QFile::ReadOnly);
    QString qtsingleapplicationCopyingString =
            qtsingleapplicationCopyingFile.readAll();

    QTextBrowser textBrowser;
    textBrowser.setText(qtsingleapplicationCopyingString);

    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Ok);
    buttonBox.setCenterButtons(true);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    dialog.setLayout(new QVBoxLayout());
    dialog.layout()->addWidget(&textBrowser);
    dialog.layout()->addWidget(&buttonBox);

    dialog.exec();
}

void MainWindow::editNouns()
{
    EditNounsDialog editNounsDialog(
                erroneousLines,
                nouns,
                nounsFile->fileName(),
                this
                );

    if(!nouns->isEmpty())
    {
        editNounsDialog.setCurrentNoun(nouns->at(nounIndex));
    }

    editNounsDialog.exec();

    updateGui();

    writeNounsAndErroneousLines();
}
