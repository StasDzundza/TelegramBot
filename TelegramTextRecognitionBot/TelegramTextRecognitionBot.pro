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
        main.cpp \
    bot.cpp \
    message.cpp \
    user.cpp \
    telegramtypesfactory.cpp \
    update.cpp \
    translater.cpp \
    textreader.cpp \
    filedownloader.cpp \
    document.cpp \
    file.cpp \
    tesseractocr.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bot.h \
    message.h \
    user.h \
    telegramtypesfactory.h \
    update.h \
    translater.h \
    textreader.h \
    filedownloader.h \
    document.h \
    file.h \
    tesseractocr.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/tesseract-include/ -lliblept168
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/tesseract-include/ -lliblept168
else:unix: LIBS += -L$$PWD/tesseract-include/ -lliblept168

INCLUDEPATH += $$PWD/tesseract-include/leptonica
DEPENDPATH += $$PWD/tesseract-include/leptonica

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/tesseract-include/ -llibtesseract302
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/tesseract-include/ -llibtesseract302
else:unix: LIBS += -L$$PWD/tesseract-include/ -llibtesseract302

INCLUDEPATH += $$PWD/tesseract-include/tesseract
DEPENDPATH += $$PWD/tesseract-include/tesseract
