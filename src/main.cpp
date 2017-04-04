#include "QiDiTu/FtpServer/FtpServer.h"
#include "QiDiTu/FtpServer/User/AnonymousUser.h"
#include "QiDiTu/FtpServer/User/UserManager.h"
#include "QiDiTu/FtpServer/FileManager/FileManager.h"

#include <QCoreApplication>
#include <QSharedPointer>

using QiDiTu::FtpServer::FtpServer;
using QiDiTu::FtpServer::User::AnonymousUser;
using QiDiTu::FtpServer::User::UserManager;
using QiDiTu::FtpServer::FileManager::FileManager;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto anonymousUser = QSharedPointer<AnonymousUser>::create();
    auto userManager = QSharedPointer<UserManager>::create(anonymousUser);
    auto fileManager = QSharedPointer<FileManager>::create();
    auto server = QSharedPointer<FtpServer>::create(userManager, fileManager);
    server->start(QHostAddress::Any, 5566);

    return a.exec();
}
