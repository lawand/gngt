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
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopWidget>
#include <QTimer>
#include <QTextCodec>

//corresponding header file(s)
#include "editnoundialog.h"
#include "ui_editnoundialog.h"

EditNounDialog::EditNounDialog(QList<Noun>* nouns, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNounDialog)
{
    //initialization
    ui->setupUi(this);
    this->nouns = nouns;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    //fix window size
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    layout()->setSizeConstraint(QLayout::SetFixedSize);
#else
    layout()->setSizeConstraint(QLayout::SetFixedSize);
#endif

    //manage width of dialog
#ifdef Q_OS_SYMBIAN
    //smaller on small screens
    if(QApplication::desktop()->screenGeometry().width() <= 240)
    {
        QFont smallerFont = this->font();
        smallerFont.setPointSize(this->font().pointSize() - 3);
        ui->capitalAWithDiaeresisToolButton->setFont(smallerFont);
        ui->capitalOWithDiaeresisToolButton->setFont(smallerFont);
        ui->capitalUWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallSharpSToolButton->setFont(smallerFont);
        ui->smallAWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallOWithDiaeresisToolButton->setFont(smallerFont);
        ui->smallUWithDiaeresisToolButton->setFont(smallerFont);
    }

    //larger on large screens
    if(qApp->navigationMode() == Qt::NavigationModeNone)
    {
        //delay this so that the dialog can first get correct geometry values
        QTimer::singleShot(500, this, SLOT(setupWidth()));
    }
#endif

    //translation-related function call
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
}

EditNounDialog::~EditNounDialog()
{
    //delete data members
    delete ui;
}

void EditNounDialog::setText(QString text)
{
    ui->definiteArticleAndBothFormsLineEdit->setText(text);
}

QString EditNounDialog::getText()
{
    return ui->definiteArticleAndBothFormsLineEdit->text();
}

void EditNounDialog::on_buttonBox_accepted()
{
    //handle the case of an invalid noun string
    if(! Noun::isValid(ui->definiteArticleAndBothFormsLineEdit->text()))
    {
        QMessageBox::information(
                    this,
                    tr("Noun Format Error"),
                    tr(
                        "Incorrect format. \n"
                        "The correct format is "
                        "like this: 'das Auto Autos'. \n"
                        "The plural form is optional. \n"
                        "Both forms can each contain at "
                        "most 23 characters."
                        )
                    );

        return;
    }

    //handle the case of a duplicate noun
    Noun noun(ui->definiteArticleAndBothFormsLineEdit->text());
    if(nouns->indexOf(noun) != -1)
    {
        QMessageBox::information(
                    this,
                    tr("Duplicate Noun"),
                    tr("This noun already exists")
                    );

        return;
    }

    //at this point, accept can be called
    accept();
}

void EditNounDialog::on_definiteArticleAndBothFormsLineEdit_textChanged(
        QString
        )
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            ! ui->definiteArticleAndBothFormsLineEdit->text().isEmpty()
            );
}

void EditNounDialog::on_capitalAWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xC4));
}

void EditNounDialog::on_capitalOWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xD6));
}

void EditNounDialog::on_capitalUWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xDC));
}

void EditNounDialog::on_smallSharpSToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xDF));
}

void EditNounDialog::on_smallAWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xE4));
}

void EditNounDialog::on_smallOWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xF6));
}

void EditNounDialog::on_smallUWithDiaeresisToolButton_clicked()
{
    ui->definiteArticleAndBothFormsLineEdit->insert(QChar(0xFC));
}

#ifdef Q_OS_SYMBIAN
void EditNounDialog::setupWidth()
{
    QRect geometry = this->geometry();
    geometry.setWidth(360);
    setGeometry(geometry);
}
#endif
