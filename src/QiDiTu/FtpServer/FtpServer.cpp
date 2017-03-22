#include "FtpServer.h"

#include "BackgroundThread.h"
#include "User\IUserManager.h"

#include <QDebug>
#include <QHostAddress>

namespace QiDiTu {
namespace FtpServer {

FtpServer::FtpServer(QSharedPointer<User::IUserManager> userManager, QObject *parent)
    : QObject(parent)
    , _userManager(userManager)
{
    connect(&server,    &QTcpServer::newConnection,
            this,       &FtpServer::onServerNewConnection);
}

void FtpServer::start(const QHostAddress &address, quint16 port)
{
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

void FtpServer::onServerNewConnection()
{
    qDebug() << "new connection";
    BackgroundThread* thread = new BackgroundThread(server.nextPendingConnection(),
                                                    sharedFromThis());
    thread->start();
}

} // namespace FtpServer
} // namespace QiDiTu
