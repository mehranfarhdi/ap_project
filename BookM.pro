
QT       += core sql gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookManagement
TEMPLATE = app


SOURCES += main.cpp\
    book.cpp \
    qdb.cpp \

HEADERS  += \
    book.h \
    qdb.h \

FORMS    += \
    book.ui

DISTFILES += \
    db.s3db.sql

RESOURCES += \
    elements.qrc
