QT -= gui
QT += network
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = FtpServerCore

SOURCES += main.cpp \
    QiDiTu/FtpServer/FtpServer.cpp \
    QiDiTu/FtpServer/User.cpp \
    QiDiTu/FtpServer/UserManager.cpp

HEADERS += \
    QiDiTu/FtpServer/FtpServer.h \
    QiDiTu/FtpServer/User.h \
    QiDiTu/FtpServer/UserManager.h \
    QiDiTu/FtpServer/IUserManager.h
