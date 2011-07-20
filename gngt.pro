symbian: {
    HEADERS += qtsingleapplication/src/QtSingleApplication
}
!symbian: {
    include(qtsingleapplication/src/qtsingleapplication.pri)
}

VERSION = 1.1.2

linux {
    TARGET = gngt
}
maemo5 {
    TARGET = gngt
}
symbian {
    TARGET = GNGT
}
windows {
    TARGET = GNGT
}

TRANSLATIONS += gngt_cs.ts
CODECFORTR = UTF-8

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

#symbian
    symbian {
        ICON = gngt.svg
    }

#meamo 5
    maemo5 {
        target.path = /opt/gngt/bin

        QMAKE_EXTRA_TARGETS += provide_icon
        icon.files = gngt.png
        icon.path = /usr/share/icons
        icon.depends = provide_icon

        desktopfile.path = /usr/share/applications/hildon
        desktopfile.files = qtc_packaging/debian_fremantle/gngt.desktop

        INSTALLS += desktopfile icon target
    }
    OTHER_FILES += \
        qtc_packaging/debian_fremantle/rules \
        qtc_packaging/debian_fremantle/copyright \
        qtc_packaging/debian_fremantle/control \
        qtc_packaging/debian_fremantle/compat

