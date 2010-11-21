include(qtsingleapplication/qtsingleapplication.pri)

TARGET = GNGT
TEMPLATE = app
SOURCES += \
    noun.cpp \
    main.cpp \
    mainwindow.cpp \
    editnoundialog.cpp \
    editnounsdialog.cpp
HEADERS += \
    noun.h \
    mainwindow.h \
    editnoundialog.h \
    editnounsdialog.h
FORMS += \
    mainwindow.ui \
    editnoundialog.ui \
    editnounsdialog.ui

RESOURCES += \
    main.qrc

RC_FILE = main.rc
