#pragma once

#include "User\IUserManager.h"

#include <QSharedPointer>
#include <QTcpServer>

namespace QiDiTu {
namespace FtpServer {
class FtpServer : public QObject
{
    Q_OBJECT

private:
    QSharedPointer<User::IUserManager> userManager;

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

private:
    QTcpServer server;
    void onServerNewConnection();

};
} // namespace FtpServer
} // namespace QiDiTu
