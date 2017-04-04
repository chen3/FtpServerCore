#include "FtpServer.h"

#include "BackgroundThread.h"
#include "User/IUserManager.h"
#include "FileManager/IFileManager.h"

#include <QDebug>
#include <QHostAddress>

namespace QiDiTu {
namespace FtpServer {

FtpServer::FtpServer(QSharedPointer<User::IUserManager> userManager,
                     QSharedPointer<FileManager::IFileManager> fileManager,
                     QObject *parent)
    : QObject(parent)
    , _userManager(userManager)
    , _fileManager(fileManager)
{
    connect(&server,    &QTcpServer::newConnection,
            this,       &FtpServer::onServerNewConnection);
}

void FtpServer::start(const QHostAddress &address, quint16 port)
{
    qDebug() << "start listen" << address << port;
    server.listen(address, port);
}

bool FtpServer::isRuning() const
{
    return server.isListening();
}

void FtpServer::stop()
{
    server.close();
}

QSharedPointer<User::IUserManager> FtpServer::userManager()
{
    return _userManager;
}

QSharedPointer<FileManager::IFileManager> FtpServer::fileManager()
{
    return _fileManager;
}

void FtpServer::onServerNewConnection()
{
    qDebug() << "new connection";
    BackgroundThread* thread = new BackgroundThread(server.nextPendingConnection(),
                                                    this->sharedFromThis());
    thread->start();
}

} // namespace FtpServer
} // namespace QiDiTu
