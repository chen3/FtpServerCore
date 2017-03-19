#pragma once

#include <QSharedPointer>
#include <QThread>

class QTcpSocket;

namespace QiDiTu {
namespace FtpServer {

class FtpServer;

class BackgroundThread : public QThread
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    void reply(qint32 responseCode, QString message);
    QSharedPointer<FtpServer> server;

public:
    BackgroundThread(QTcpSocket* socket, QSharedPointer<FtpServer> server);

protected:
    virtual void run() override;

private:
    Q_INVOKABLE void user(QByteArray username);

};

} // namespace FtpServer
} // namespace QiDiTu
