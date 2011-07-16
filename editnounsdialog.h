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

#ifndef EDITNOUNSDIALOG_H
#define EDITNOUNSDIALOG_H

//base class(es)
#include <QDialog>

//template data member(s)
#include <QList>
#include "noun.h"

//data member(s) forward declaration(s)
#ifdef Q_OS_SYMBIAN
class QMenu;
class QAction;
class QKeyEvent;
#endif

namespace Ui {
    class EditNounsDialog;
}

class EditNounsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNounsDialog(
        QStringList* erroneousLines,
        QList<Noun>* nouns,
        QString nounsFileFileName,
        QWidget *parent = 0
        );
    ~EditNounsDialog();

    void updateState();
    void setQPushButtonsVisible(bool visible);
    void setQActionsVisible(bool visible);
    void setCurrentNoun(Noun noun);

private slots:
    void add();
    void edit();
    void remove();
    void removeAll();

private:
    Ui::EditNounsDialog *ui;
    QList<Noun>* nouns;
    QStringList* erroneousLines;
#ifdef Q_OS_SYMBIAN
    QMenu* qMenu;
    QAction *actionOptions;
#endif

protected:
    virtual void keyPressEvent(QKeyEvent *e);
};

#endif // EDITNOUNSDIALOG_H
