QT += gui
QT += network
QT += core

CONFIG += c++14 console
CONFIG -= app_bundle

DEFINES += _CRT_SECURE_NO_WARNINGS

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/sources/Utils/filewriter.cpp \
        src\sources\main.cpp \
    src\sources\bot.cpp \
    src\sources\TelegramTypes\message.cpp \
    src\sources\TelegramTypes\user.cpp \
    src\sources\Utils\telegramtypesfactory.cpp \
    src\sources\TelegramTypes\update.cpp \
    src\sources\Utils\translater.cpp \
    src\sources\Utils\textreader.cpp \
    src\sources\Utils\filedownloader.cpp \
    src\sources\TelegramTypes\document.cpp \
    src\sources\TelegramTypes\file.cpp \
    src\sources\Utils\tesseractocr.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include\bot.h \
    src/headers/Utils/filewriter.h \
    src\headers\TelegramTypes\message.h \
    src\headers\TelegramTypes\user.h \
    src\headers\Utils\telegramtypesfactory.h \
    src\headers\TelegramTypes\update.h \
    src\headers\Utils\translater.h \
    src\headers\Utils\textreader.h \
    src\headers\Utils\filedownloader.h \
    src\headers\TelegramTypes\document.h \
    src\headers\TelegramTypes\file.h \
    include\tesseractocr.h

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src/headers
INCLUDEPATH += $$PWD/src/headers/TelegramTypes
INCLUDEPATH += $$PWD/src/headers/Utils

INCLUDEPATH += $$PWD/TesseractOCR/tesseract-include/tesseract
LIBS += -L"$$PWD/TesseractOCR/tesseract-include/" -llibtesseract302

INCLUDEPATH += $$PWD/TesseractOCR/tesseract-include/leptonica
LIBS += -L"$$PWD/TesseractOCR/tesseract-include/" -lliblept168
