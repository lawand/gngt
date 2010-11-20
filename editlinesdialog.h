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

#ifndef EDITLINESDIALOG_H
#define EDITLINESDIALOG_H

//base class(es)
#include <QDialog>

//data member(s)
#include <QList>
#include "noun.h"

//data member(s) forward declaration(s)
class QStringList;

namespace Ui {
    class EditLinesDialog;
}

class EditLinesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLinesDialog(QStringList* lines,
                             QList<Noun>* nouns,
                             QWidget *parent = 0);
    ~EditLinesDialog();

    void updateGui();

private:
    Ui::EditLinesDialog *ui;
    QList<Noun>* nouns;
    QStringList* lines;

private slots:
    void on_editPushButton_clicked();
    void on_removePushButton_clicked();
    void on_removeAllPushButton_clicked();
    void on_donePushButton_clicked();
};

#endif // EDITLINESDIALOG_H
