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

#include <QtSingleApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //create and initialize QApplication or QtSingleApplication
#ifdef Q_OS_SYMBIAN
    QApplication application(argc, argv);
#elif defined(Q_WS_MAEMO_5)
    QApplication application(argc, argv);
#else
    QtSingleApplication application(argc, argv);
#endif

    //manage instances
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    //do nothing
#else
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
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    //do nothing
#else
    application.setActivationWindow(&mainWindow);
#endif

    //start the application
    return application.exec();
}
