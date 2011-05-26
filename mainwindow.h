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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//base class(es)
#include <QMainWindow>

//template data member(s)
#include <QList>
#include "noun.h"

//data member(s) forward declaration(s)
class QStringList;
class QFile;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

//member functions
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readNounsAndErroneousLines();
    void writeNounsAndErroneousLines();

    void updateGui();
    void displayNewNoun();
    void giveFeedbackAndUpdateNouns(Noun::Gender chosenGender);
    void stopFeedback();

    void on_masculinePushButton_clicked();
    void on_femininePushButton_clicked();
    void on_neuterPushButton_clicked();

    void readme();
    void copying();
    void editNouns();

//data members
private:
    Ui::MainWindow *ui;
    QList<Noun> *nouns;
    QStringList* erroneousLines;
    int nounIndex;
    QFile *nounsFile;
    bool feedbackActive;
};

#endif // MAINWINDOW_H
