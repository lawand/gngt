include(qtsingleapplication/qtsingleapplication.pri)

TARGET = GNGT
TEMPLATE = app
SOURCES += \
    noun.cpp \
    main.cpp \
    mainwindow.cpp \
    editnoundialog.cpp \
    editnounsdialog.cpp \
    readnounerrordialog.cpp \
    readstreakerrordialog.cpp
HEADERS += \
    noun.h \
    mainwindow.h \
    editnoundialog.h \
    editnounsdialog.h \
    readnounerrordialog.h \
    readstreakerrordialog.h
FORMS += \
    mainwindow.ui \
    editnoundialog.ui \
    editnounsdialog.ui \
    readnounerrordialog.ui \
    readstreakerrordialog.ui

RESOURCES += \
    main.qrc

RC_FILE = main.rc
