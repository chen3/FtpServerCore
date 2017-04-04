#pragma once

#include <QSharedPointer>
#include <QThread>
#include <QTcpServer>

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
    QSharedPointer<FtpServer> server;
    QSharedPointer<User::IUser> currUser;
    QString currentFtpDir{"/"};

    enum class State {
        notLogged, logging, logged
    };
    State state{State::notLogged};

    enum class TransferType {
        Binary, Ascii
    };
    TransferType transferType{TransferType::Binary};

    enum class TransferAction {
        Pasv
    };
    TransferAction transferAction{TransferAction::Pasv};

    QTcpServer transferServer;

public:
    BackgroundThread(QTcpSocket* socket, QSharedPointer<FtpServer> server);

protected:
    QSharedPointer<QTcpSocket> socket;
    virtual void run() override;

    Q_INVOKABLE virtual void user(QByteArray username);
    Q_INVOKABLE virtual void quit();
    Q_INVOKABLE virtual void syst();
    Q_INVOKABLE virtual void pwd();
    Q_INVOKABLE virtual void type(QByteArray data);
    Q_INVOKABLE virtual void size(QByteArray path);
    Q_INVOKABLE virtual void cwd(QByteArray path);
    Q_INVOKABLE virtual void pasv();
    Q_INVOKABLE virtual void list(QByteArray arguments = QByteArray());
    Q_INVOKABLE virtual void noop();
    Q_INVOKABLE virtual void retr(QByteArray name);
    Q_INVOKABLE virtual void stor(QByteArray name);
    Q_INVOKABLE virtual void appe(QByteArray name);
    Q_INVOKABLE virtual void rnfr(QByteArray name);
    Q_INVOKABLE virtual void rnto(QByteArray name);
    Q_INVOKABLE virtual void dele(QByteArray name);
    Q_INVOKABLE virtual void rmd(QByteArray name);
    Q_INVOKABLE virtual void mkd(QByteArray name);

private:
    void reply(qint32 responseCode, QString message);
    QString rnfrName;

};

} // namespace FtpServer
} // namespace QiDiTu
