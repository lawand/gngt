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

#ifndef Noun_H
#define Noun_H

//data member(s)
#include <QString>

class Noun
{

//class enumerations
public:
    //this denotes the gender of the noun
    enum Gender
    {
        masculine,
        feminine,
        neuter
    };

//member functions
public:
    explicit Noun(
        QString definiteArticleAndBothForms = "",
        int memorizationStreak = 0
        );
    ~Noun();

    //conversion
    QString toString();

    //validation
    static bool isValid(QString definiteArticleAndBothForms);

    //operators
    bool operator== (Noun otherNoun);
    bool operator<  (Noun otherNoun) const;

//data members
public:
    Gender gender;
    QString singularForm;
    QString pluralForm;
    int memorizationStreak;
};

#endif // Noun_H
