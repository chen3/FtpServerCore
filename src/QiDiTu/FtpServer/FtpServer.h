#pragma once

#include "IUserManager.h"

#include <QTcpServer>

namespace QiDiTu {
namespace FtpServer {
class FtpServer : public QObject
{
    Q_OBJECT

private:
    IUserManager userManager;

public:
    explicit FtpServer(IUserManager userManager, QObject *parent = nullptr);
    /*!
     * \brief start ftp server
     * \sa stop()
     */
    void start();

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
