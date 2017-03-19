#pragma once

#include <QSharedPointer>
#include <QTcpServer>

namespace QiDiTu {
namespace FtpServer {

namespace User {
class IUserManager;
}

class FtpServer : public QObject , public QEnableSharedFromThis<FtpServer>
{
    Q_OBJECT

private:
    QSharedPointer<User::IUserManager> _userManager;
    QTcpServer server;
    void onServerNewConnection();

public:
    explicit FtpServer(QSharedPointer<User::IUserManager> userManager,
                       QObject *parent = nullptr);

    /*!
     * \brief start ftp server
     * \sa stop()
     */
    void start(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

    bool isRuning() const;

    /*!
     * \brief stop ftp server
     * \sa start()
     */
    void stop();

    QSharedPointer<User::IUserManager> userManager();

    QString welcomeMessage();

};

} // namespace FtpServer
} // namespace QiDiTu
