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

//implementation-specific data type(s)
#include <QStringList>

//corresponding header file(s)
#include "noun.h"

Noun::Noun(QString definiteArticleAndSingularForm, int memorizationStreak)
{
    if(Noun::isValid(definiteArticleAndSingularForm))
    {
        QStringList parts = definiteArticleAndSingularForm.split(
                QRegExp("\\s+"),
                QString::SkipEmptyParts
                );

        QString article = parts.value(0);
        article = article.toLower();
        if(article[2] == 'r')
            gender = Noun::masculine;
        if(article[2] == 'e')
            gender = Noun::feminine;
        if(article[2] == 's')
            gender = Noun::neuter;

        singularForm = parts.value(1);
        singularForm = singularForm.toLower();
        singularForm[0] = singularForm[0].toUpper();
    }
    else
    {
        this->gender = Noun::neuter;
        this->singularForm = "Error";
    }

    this->memorizationStreak = memorizationStreak;
}

Noun::~Noun()
{
}

QString Noun::toString()
{
    QString defeniteArticle;
    if(gender == Noun::masculine)
        defeniteArticle = "der";
    if(gender == Noun::feminine)
        defeniteArticle = "die";
    if(gender == Noun::neuter)
        defeniteArticle = "das";

    return QString("%1 %2").arg(defeniteArticle).arg(singularForm);
}

bool Noun::isValid(QString definiteArticleAndSingularForm)
{
    QStringList parts = definiteArticleAndSingularForm.split(
            QRegExp("\\s+"),
            QString::SkipEmptyParts
            );

    if(parts.size() != 2)
        return false;

    if(! QRegExp("^der|die|das$").exactMatch(parts.at(0).toLower()))
        return false;

    if(! QRegExp(
            "^[a-zA-Z\x00C4\x00D6\x00DC\x00DF\x00E4\x00F6\x00FC]{1,23}$"
            ).exactMatch(parts.at(1))
        )
        return false;

    //at this point, return true
    return true;
}

bool Noun::operator== (Noun otherNoun)
{
    return QString::compare(singularForm,
                            otherNoun.singularForm,
                            Qt::CaseInsensitive
                            ) == 0;
}

bool Noun::operator<  (Noun otherNoun) const
{
    return this->singularForm < otherNoun.singularForm;
}
