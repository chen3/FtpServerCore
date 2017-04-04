#pragma once

#include <QSharedPointer>
#include <QTcpServer>

namespace QiDiTu {
namespace FtpServer {

namespace User {
class IUserManager;
}
namespace FileManager {
class IFileManager;
}

class FtpServer : public QObject , public QEnableSharedFromThis<FtpServer>
{
    Q_OBJECT
    Q_PROPERTY(QString welcomeMessage MEMBER _welcomeMessage)
    Q_PROPERTY(QString name MEMBER _name)
    Q_PROPERTY(QString quitMessage MEMBER _quit)

private:
    QSharedPointer<User::IUserManager> _userManager;
    QSharedPointer<FileManager::IFileManager> _fileManager;
    QTcpServer server;

public:
    explicit FtpServer(QSharedPointer<User::IUserManager> userManager,
                       QSharedPointer<FileManager::IFileManager> fileManager,
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

    QSharedPointer<FileManager::IFileManager> fileManager();

private:
    QString _welcomeMessage{QStringLiteral("Welcome.")};
    QString _name{QStringLiteral("UNIX emulated by QtFtp")};
    QString _quit{QStringLiteral("Goodbye")};
    void onServerNewConnection();

};

} // namespace FtpServer
} // namespace QiDiTu
