#-------------------------------------------------
#
# Project created by QtCreator 2013-02-08T22:29:11
#
#-------------------------------------------------

QT       += core gui

TARGET = RobotCONTROL
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    serialport.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    leorexception.h \
    leorMessage.h

FORMS    += mainwindow.ui

win32: LIBS += -LC:/lib/boost/lib/ \
  -lAdvapi32\
  -lws2_32 \
  -llibboost_system-mgw44-mt-1_53 \
  -llibboost_thread-mgw44-mt-1_53


DEFINES += _WIN32_WINNT=0x0501 BOOST_THREAD_USE_LIB
INCLUDEPATH += C:/lib/boost/include/boost-1_53
DEPENDPATH += C:/lib/boost/include/boost-1_53

#PRE_TARGETDEPS += C:/lib/boost/lib/libboost_system-mgw46-mt-1_50.lib
