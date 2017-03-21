#include "QiDiTu/FtpServer/FtpServer.h"
#include "QiDiTu/FtpServer/User/AnonymousUser.h"
#include "QiDiTu/FtpServer/User/UserManager.h"

#include <QCoreApplication>
#include <QSharedPointer>

using namespace QiDiTu::FtpServer;
using namespace QiDiTu::FtpServer::User;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto anonymousUser = QSharedPointer<AnonymousUser>::create();
    auto userManager = QSharedPointer<UserManager>::create(anonymousUser);
    auto server = QSharedPointer<FtpServer>::create(userManager);
    server->start(QHostAddress::Any, 5566);

    return a.exec();
}
