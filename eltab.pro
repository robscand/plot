QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eltab
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    spreadsheet.cpp \
    cell.cpp \
    finddialog.cpp \
    delegates/combobox.cpp \
    delegates/spinbox.cpp \
    plot.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    spreadsheet.h \
    cell.h \
    finddialog.h \
    delegates/combobox.h \
    delegates/spinbox.h \
    plot.h \
    qcustomplot.h

TRANSLATIONS += lang_en.ts \
               lang_ru.ts

CODEC       = EUC-JP

RESOURCES += \
    myres.qrc

FORMS += \
    plot.ui


