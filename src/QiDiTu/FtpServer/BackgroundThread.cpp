#include "BackgroundThread.h"

#include "FtpServer.h"

#include <exception>
#include <QTcpSocket>

namespace QiDiTu {
namespace FtpServer {

void BackgroundThread::reply(qint32 responseCode, QString message)
{
    socket->write((QString::number(responseCode) + " " + message + "\r\n").toLatin1());
}

BackgroundThread::BackgroundThread(QTcpSocket *socket, QSharedPointer<FtpServer> server)
    : QThread(server.data())
    , socket(socket)
    , server(server)
{

    if(socket == nullptr) {
        throw std::invalid_argument("socket");
    }
    socket->setParent(nullptr);
    socket->moveToThread(this);
}

void BackgroundThread::run()
{
    reply(220, server->welcomeMessage());
    while(true) {
        QByteArray data;
        int index;
        do {
            if(!socket->waitForReadyRead()) {
                //time out
                qDebug() << "time out";
                return;
            }
            data += socket->readAll();
        } while((index = data.indexOf("\r\n")) < 0);
        QByteArray cmd = data.left(index);
        qDebug() << cmd;
        data = data.right(data.length() - (index + 2));
        int spaceIndex = cmd.indexOf(' ');
        if(spaceIndex <= 0) {
            qDebug() << "unknow cmd:" << cmd;
            continue;
        }
        QByteArray command = cmd.left(spaceIndex).toLower();
        QByteArray content = cmd.right(cmd.length() - spaceIndex - 1);
        if(!this->metaObject()->invokeMethod(this, command.data(),
                                         Qt::ConnectionType::DirectConnection,
                                         Q_ARG(QByteArray, content))){
            qDebug() << "error invoke:" << command;
        }
    }
}

void BackgroundThread::user(QByteArray username)
{
    server->userManager();
}

} // namespace FtpServer
} // namespace QiDiTu
