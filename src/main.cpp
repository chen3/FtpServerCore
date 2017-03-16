#include "QiDiTu/FtpServer/FtpServer.h"
#include "QiDiTu/FtpServer/User/UserManager.h"

#include <QCoreApplication>
#include <QSharedPointer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QiDiTu::FtpServer::FtpServer ftpServer(QSharedPointer<QiDiTu::FtpServer::User::UserManager>::create());
    ftpServer.start(QHostAddress::Any, 5566);

    return a.exec();
}
