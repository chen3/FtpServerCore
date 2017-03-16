#include "QiDiTu/FtpServer/FtpServer.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QiDiTu::FtpServer::FtpServer ftpServer;
    ftpServer.start();

    return a.exec();
}
