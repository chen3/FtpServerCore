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
    Q_PROPERTY(QString welcomeMessage MEMBER _welcomeMessage)
    Q_PROPERTY(QString name MEMBER _name)
    Q_PROPERTY(QString quitMessage MEMBER _quit)

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

private:
    QString _welcomeMessage{QStringLiteral("Welcome.")};
    QString _name{QStringLiteral("QtFtp")};
    QString _quit{QStringLiteral("Goodbye")};

};

} // namespace FtpServer
} // namespace QiDiTu
