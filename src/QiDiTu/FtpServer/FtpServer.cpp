#include "FtpServer.h"

#include <QDebug>
#include <QHostAddress>

namespace QiDiTu {
namespace FtpServer {

FtpServer::FtpServer(QSharedPointer<User::IUserManager> userManager, QObject *parent)
    : QObject(parent)
    , userManager(userManager)
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

void FtpServer::onServerNewConnection()
{
    qDebug() << "new connect";
}

} // namespace FtpServer
} // namespace QiDiTu
