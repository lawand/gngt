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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//base class(es)
#include <QMainWindow>

//template data member(s)
#include <QList>
#include "noun.h"

//data member(s) forward declaration(s)
class QTimer;
class QString;
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

    void updateGui();

private slots:
    void displayNewNoun();
    void giveFeedbackAndUpdateNouns(Noun::Gender chosenGender);
    void stopFeedback();

    void on_masculinePushButton_clicked();
    void on_femininePushButton_clicked();
    void on_neuterPushButton_clicked();

    void about();
    void editNouns();

//data members
private:
    Ui::MainWindow *ui;
    QTimer *feedbackAndNounSwitchTimer;
    QList<Noun> *nouns;
    int nounIndex;
    bool feedbackActive;
    QFile *nounsFile;
};

#endif // MAINWINDOW_H
