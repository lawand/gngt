include(qtsingleapplication/qtsingleapplication.pri)

TARGET = GNGT
TEMPLATE = app
SOURCES += \
    noun.cpp \
    main.cpp \
    mainwindow.cpp \
    editnoundialog.cpp \
    editnounsdialog.cpp \
    editlinesdialog.cpp
HEADERS += \
    noun.h \
    mainwindow.h \
    editnoundialog.h \
    editnounsdialog.h \
    editlinesdialog.h
FORMS += \
    mainwindow.ui \
    editnoundialog.ui \
    editnounsdialog.ui \
    editlinesdialog.ui

RESOURCES += \
    main.qrc

RC_FILE = main.rc
