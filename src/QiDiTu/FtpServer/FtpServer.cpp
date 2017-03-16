#include "FtpServer.h"

#include <QDebug>
#include <QHostAddress>

namespace QiDiTu {
namespace FtpServer {

FtpServer::FtpServer(QObject *parent)
    : QObject(parent)
{
    connect(&server,    &QTcpServer::newConnection,
            this,       &FtpServer::onServerNewConnection);
}

void FtpServer::start()
{
    server.listen(QHostAddress::Any, 21);
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
