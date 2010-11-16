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
    explicit Noun();
    ~Noun();

    //conversion
    void fromDefiniteArticleAndSingularForm(
            QString definiteArticleAndSingularForm
            );
    void fromSortingFriendlyDefiniteArticleAndSingularForm(
            QString sortingFriendlyDefiniteArticleAndSingularForm
            );
    QString toDefiniteArticleAndSingularForm();
    QString toSortingFriendlyDefiniteArticleAndSingularForm();

    //validation
    static bool isValid(QString genderToDefiniteArticleString);

    //operators
    bool operator== (Noun otherNoun);
    bool operator<  (Noun otherNoun) const;

//data members
public:
    Gender gender;
    QString singularForm;
    bool enabled;
};

#endif // Noun_H
