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

#include <QtSingleApplication>
#include <QIcon>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //create and initialize QApplication or QtSingleApplication
#ifndef Q_OS_SYMBIAN
    QtSingleApplication application(argc, argv);
#else
    QApplication application(argc, argv);
#endif

    //manage instances
#ifndef Q_OS_SYMBIAN
    if(application.isRunning())
    {
        return !application.sendMessage("Are you running?");
    }
#endif

    //set icon for all windows
    application.setWindowIcon(QIcon(":/icons/applicationIcon.svg"));

    //create an instance of MainWindow
    MainWindow mainWindow;

    //show that instance
#ifndef Q_OS_SYMBIAN
    mainWindow.show();
#else
    mainWindow.showMaximized();
#endif

    //setActivationWindow (for QtSingleApplication)
#ifndef Q_OS_SYMBIAN
    application.setActivationWindow(&mainWindow);
#endif

    //start the application
    return application.exec();
}
