#pragma once

#include <QSharedPointer>
#include <QThread>

class QTcpSocket;

namespace QiDiTu {
namespace FtpServer {

class FtpServer;
namespace User {
class IUser;
}

class BackgroundThread final : public QThread
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    void reply(qint32 responseCode, QString message);
    QSharedPointer<FtpServer> server;
    enum class State{
        notLogged, logging, logged
    };
    State state{State::notLogged};
    QSharedPointer<User::IUser> user;

public:
    BackgroundThread(QTcpSocket* socket, QSharedPointer<FtpServer> server);

protected:
    virtual void run() override;

private:
    Q_INVOKABLE void user(QByteArray username);
    Q_INVOKABLE void quit();

};

} // namespace FtpServer
} // namespace QiDiTu
