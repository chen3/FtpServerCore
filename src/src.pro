QT -= gui
QT += network
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = FtpServerCore

SOURCES += main.cpp \
    QiDiTu/FtpServer/FtpServer.cpp \
    QiDiTu/FtpServer/User/User.cpp \
    QiDiTu/FtpServer/User/UserManager.cpp \
    QiDiTu/FtpServer/BackgroundThread.cpp \
    QiDiTu/FtpServer/User/AnonymousUser.cpp

HEADERS += \
    QiDiTu/FtpServer/FtpServer.h \
    QiDiTu/FtpServer/User/IUSerManager.h \
    QiDiTu/FtpServer/User/User.h \
    QiDiTu/FtpServer/User/UserManager.h \
    QiDiTu/FtpServer/BackgroundThread.h \
    QiDiTu/FtpServer/User/IUser.h \
    QiDiTu/FtpServer/User/AnonymousUser.h
