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
#include <QTextCodec>
#include <QLocale>
#include <QSettings>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //translation-related function call
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    //create and initialize QApplication or QtSingleApplication
#ifdef Q_OS_SYMBIAN
    QApplication application(argc, argv);
#elif defined(Q_WS_MAEMO_5)
    QApplication application(argc, argv);
#else
    QtSingleApplication application(argc, argv);
#endif

    //for QSettings
    application.setOrganizationName("lawand");
    application.setApplicationName("gngt");

    //set application language
        //default language is the system locale
            QString defaultLanguage = QLocale::system().name();
            defaultLanguage.chop(3);

        //determine language, either from settings or as the system locale
            QSettings settings;
            QString language = settings.value(
                        "config/language", defaultLanguage
                        ).toString();

        //load the language
            QTranslator gngtTranslator;
            gngtTranslator.load(
                        QString(":/languages/gngt_%1").arg(language)
                                );
            application.installTranslator(&gngtTranslator);

            QTranslator qtTranslator;
            qtTranslator.load(
                        QString(":/languages/qt_%1").arg(language)
                        );
            application.installTranslator(&qtTranslator);

    //manage instances
#ifdef Q_OS_SYMBIAN
    //do nothing
#elif defined(Q_WS_MAEMO_5)
    //do nothing
#else
    if(application.isRunning())
    {
        return !application.sendMessage(
                    QObject::tr("Are you running?")
                    );
    }
#endif

    //set icon for all windows
    application.setWindowIcon(QIcon(":/icons/gngt.svg"));

    //create an instance of MainWindow
    MainWindow mainWindow(language);

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
