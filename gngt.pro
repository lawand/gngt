include(qtsingleapplication/qtsingleapplication.pri)

TARGET = GNGT
TEMPLATE = app
SOURCES += \
    noun.cpp \
    main.cpp \
    mainwindow.cpp \
    editnoundialog.cpp \
    editnounsdialog.cpp \
    readnounerrordialog.cpp
HEADERS += \
    noun.h \
    mainwindow.h \
    editnoundialog.h \
    editnounsdialog.h \
    readnounerrordialog.h
FORMS += \
    mainwindow.ui \
    editnoundialog.ui \
    editnounsdialog.ui \
    readnounerrordialog.ui

RESOURCES += \
    main.qrc

RC_FILE = main.rc
