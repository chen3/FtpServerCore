#include "QiDiTu/FtpServer/FtpServer.h"
#include "QiDiTu/FtpServer/User/UserManager.h"

#include <QCoreApplication>
#include <QSharedPointer>

using namespace QiDiTu::FtpServer;
using namespace QiDiTu::FtpServer::User;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto server = QSharedPointer<FtpServer>::create(QSharedPointer<UserManager>::create());
    server->start(QHostAddress::Any, 5566);

    return a.exec();
}
