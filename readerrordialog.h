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

#ifndef READERRORDIALOG_H
#define READERRORDIALOG_H

//base class(es)
#include <QDialog>

//template data member(s)
#include <QList>
#include "noun.h"

//data member(s)
#include <QString>

namespace Ui {
    class ReadErrorDialog;
}

class ReadErrorDialog : public QDialog
{
    Q_OBJECT

//member functions
public:
    explicit ReadErrorDialog(int lineNumber,
                             QString line,
                             QList<Noun>* nouns,
                             QWidget *parent = 0);
    ~ReadErrorDialog();

    //public access functions
    bool shouldIgnoreAll();
    QString getLine();

//data members
private:
    Ui::ReadErrorDialog *ui;
    int lineNumber;
    QString line;
    bool ignoreAll;
    QList<Noun> *nouns;

private slots:
    void on_fixPushButton_clicked();
    void on_ignorePushButton_clicked();
    void on_ignoreAllPushButton_clicked();
};

#endif // READERRORDIALOG_H
