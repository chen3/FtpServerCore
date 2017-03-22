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
    QSharedPointer<FtpServer> server;
    QSharedPointer<User::IUser> currUser;

    enum class State {
        notLogged, logging, logged
    };
    State state{State::notLogged};

    enum class TransferType {
        Binary, Ascii
    };
    TransferType transferType{TransferType::Binary};

public:
    BackgroundThread(QTcpSocket* socket, QSharedPointer<FtpServer> server);

protected:
    virtual void run() override;

private:
    void reply(qint32 responseCode, QString message);

    Q_INVOKABLE void user(QByteArray username);
    Q_INVOKABLE void quit();
    Q_INVOKABLE void syst();
    Q_INVOKABLE void pwd();
    Q_INVOKABLE void type(QByteArray data);

};

} // namespace FtpServer
} // namespace QiDiTu
