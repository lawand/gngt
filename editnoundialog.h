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

#ifndef EDITNOUNDIALOG_H
#define EDITNOUNDIALOG_H

//base class(es)
#include <QDialog>

//template data member(s)
#include <QList>
#include "noun.h"

namespace Ui {
    class EditNounDialog;
}

class EditNounDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNounDialog(QList<Noun>* nouns, QWidget *parent = 0);
    ~EditNounDialog();

    void setText(QString text);
    QString getText();

private slots:
    void on_buttonBox_accepted();

    void on_definiteArticleAndSingularFormLineEdit_textChanged(QString );

    void on_capitalAWithDiaeresisToolButton_clicked();
    void on_capitalOWithDiaeresisToolButton_clicked();
    void on_capitalUWithDiaeresisToolButton_clicked();
    void on_smallSharpSToolButton_clicked();
    void on_smallAWithDiaeresisToolButton_clicked();
    void on_smallOWithDiaeresisToolButton_clicked();
    void on_smallUWithDiaeresisToolButton_clicked();

private:
    Ui::EditNounDialog *ui;
    QList<Noun>* nouns;
};

#endif // EDITNOUNDIALOG_H
