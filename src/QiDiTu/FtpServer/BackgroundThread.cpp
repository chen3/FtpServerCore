#include "BackgroundThread.h"

#include "FtpServer.h"
#include "User/IUser.h"
#include "User/IUSerManager.h"

#include <exception>
#include <QTcpSocket>

namespace QiDiTu {
namespace FtpServer {

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
    QByteArray data;
    int index;

    while(socket->isOpen()) {
        do {
            if(!socket->waitForReadyRead()) {
                qDebug() << "time out";
                return;
            }
            data += socket->readAll();
        } while((index = data.indexOf("\r\n")) < 0);

        QByteArray cmd = data.left(index);
        data = data.right(data.length() - (index + 2));
        qDebug() << cmd;

        int spaceIndex = cmd.indexOf(' ');
        if(spaceIndex == 0) {
            qDebug() << "unknow cmd:" << cmd;
            continue;
        }
        bool success = false;
        if(spaceIndex < 0) {
            success = this->metaObject()->invokeMethod(this, cmd.toLower().data(),
                                             Qt::ConnectionType::DirectConnection);
        }
        else {
            QByteArray command = cmd.left(spaceIndex).toLower();
            QByteArray content = cmd.right(cmd.length() - spaceIndex - 1);
            success = this->metaObject()->invokeMethod(this, command.data(),
                                                    Qt::ConnectionType::DirectConnection,
                                                    Q_ARG(QByteArray, content));
        }
        if(!success) {
            reply(502, cmd);
            continue;
        }
    }
    qDebug() << "close";
}

void BackgroundThread::reply(qint32 responseCode, QString message)
{
    socket->write(QByteArray::number(responseCode) + " " + message.toLatin1() + "\r\n");
}

void BackgroundThread::user(QByteArray username)
{
    QSharedPointer<User::IUser> user = server->userManager()->getUser(username);
    if(user.isNull()) {
        reply(530, "not user exist");
        return;
    }
    if(user->isNeedPassword()) {
        reply(331, "need password");
        state = State::logging;
    }
    else {
        reply(230, "user logged in");
        state = State::logged;
    }
}

void BackgroundThread::quit()
{
    socket->close();
}

} // namespace FtpServer
} // namespace QiDiTu
