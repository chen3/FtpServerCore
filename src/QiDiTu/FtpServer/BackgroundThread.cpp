#include "BackgroundThread.h"

#include "FtpServer.h"
#include "FileManager/IFileManager.h"
#include "User/IUser.h"
#include "User/IUSerManager.h"

#include <exception>
#include <QDataStream>
#include <QDir>
#include <QFileInfo>
#include <QTcpSocket>

namespace QiDiTu {
namespace FtpServer {

BackgroundThread::BackgroundThread(QTcpSocket *socket, QSharedPointer<FtpServer> server)
    : QThread(server.data())
    , server(server)
{
    if(socket == nullptr) {
        throw std::invalid_argument("socket");
    }
    socket->setParent(nullptr);
    socket->moveToThread(this);
    transferServer.moveToThread(this);
    transferServer.setMaxPendingConnections(1);
    this->socket = QSharedPointer<QTcpSocket>(socket);
}

void BackgroundThread::run()
{
    reply(220, server->property("welcomeMessage").toString());
    QByteArray data;
    int index;

    while(socket->isOpen()) {
        do {
            if(!socket->waitForReadyRead()) {
                reply(412, "time out");
                return;
            }
            data += socket->readAll();
        } while((index = data.indexOf("\r\n")) < 0);

        QByteArray cmd = data.left(index);
        data = data.right(data.length() - (index + 2));
        qDebug() << cmd;

        int spaceIndex = cmd.indexOf(' ');
        if(spaceIndex == 0) {
            qDebug() << "unknow cmd:" << cmd;
            continue;
        }
        bool success = false;
        if(spaceIndex < 0) {
            success = this->metaObject()->invokeMethod(this, cmd.toLower().data(),
                                             Qt::ConnectionType::DirectConnection);
        }
        else {
            QByteArray command = cmd.left(spaceIndex).toLower();
            QByteArray content = cmd.right(cmd.length() - spaceIndex - 1);
            success = this->metaObject()->invokeMethod(this, command.data(),
                                                    Qt::ConnectionType::DirectConnection,
                                                    Q_ARG(QByteArray, content));
        }
        if(!success) {
            reply(502, cmd);
            continue;
        }
    }
    qDebug() << "disconnected";
}

void BackgroundThread::reply(qint32 responseCode, QString message)
{
    QByteArray msg;
    if(message.isEmpty()) {
        msg = QByteArray::number(responseCode) + "\r\n";
    }
    else {
        msg = QByteArray::number(responseCode) + " " + message.toUtf8() + "\r\n";
    }
    qDebug() << "reply:" << msg;
    socket->write(msg);
}

void BackgroundThread::user(QByteArray username)
{
    QSharedPointer<User::IUser> user = server->userManager()->getUser(username);
    if(user.isNull()) {
        reply(530, "not user exist");
        return;
    }
    if(user->isNeedPassword()) {
        reply(331, "need password");
        state = State::logging;
    }
    else {
        reply(230, "user logged on");
        currUser = user;
        state = State::logged;
    }
}

void BackgroundThread::quit()
{
    reply(221, server->property("quitMessage").toString());
    socket->close();
}

void BackgroundThread::syst()
{
    reply(215, server->property("name").toString());
}

void BackgroundThread::pwd()
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->dirExists(currUser, "/")) {
        if(!fileManager->mkPath(currUser, "/")) {
            reply(550, "create fail");
            return;
        }
    }
    reply(257, QString(R"("%1" directory created)").arg(currentFtpDir));
}

void BackgroundThread::type(QByteArray data)
{
    if(data.length() > 1) {
        reply(502, data);
        return;
    }
    switch (data.toLower().at(0)) {
        case 'i': {
            transferType = TransferType::Binary;
        } break;
        case 'a': {
            transferType = TransferType::Ascii;
        } break;
        default: {
            reply(502, data);
        } break;
    }
    reply(200, "Type set to " + data);
}

void BackgroundThread::size(QByteArray path)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    path = path.trimmed();
    if(path.isEmpty()) {
        reply(550, "File not found");
        return;
    }
    QString dir = path;
    if(dir.at(0) != '/') {
        dir = currentFtpDir + dir;
    }

    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();

    qint64 size = fileManager->size(currUser, dir);
    if(size <= 0) {
        reply(550, "File not found");
    }
    else {
        reply(213, QString::number(size));
    }
}

void BackgroundThread::cwd(QByteArray path)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QString dir = path;
    if(dir.at(dir.length() - 1) != '/') {
        dir += '/';
    }
    if(dir.at(0) != '/') {
        dir = currentFtpDir + dir;
    }
    dir = QDir::cleanPath(dir);
    if(!dir.endsWith('/')) {
        dir += '/';
    }
    if(dir.startsWith("/..")) {
        reply(550, "dir out of range");
        return;
    }

    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->dirExists(currUser, dir)) {
        reply(550, "dir not exists");
    }
    else {
        currentFtpDir = dir;
        reply(250, QString(R"(CWD successful. "%1" is current directory.)").arg(dir));
    }
}

void BackgroundThread::pasv()
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    transferAction = TransferAction::Pasv;
    QString ip = socket->peerAddress().toString();
    static const QString ipv6Prefix = QStringLiteral("::ffff:");
    if(ip.startsWith(ipv6Prefix)) {
        ip = ip.mid(ipv6Prefix.length());
    }
    ip.replace('.', ',');
    transferServer.listen();
    quint16 port = transferServer.serverPort();
    quint8 port1 = port >> 8;
    quint8 port2 = port;
    ip += ',' + QString::number(port1) + ',' + QString::number(port2);
    reply(227, QString("Entering Passive Mode (%1)").arg(ip));
}

void BackgroundThread::list(QByteArray arguments)
{
    Q_UNUSED(arguments);
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    if(!transferServer.hasPendingConnections()) {
        if(!transferServer.waitForNewConnection()) {
            transferServer.close();
            return;
        }
    }
    QTcpSocket* tcpSocket = transferServer.nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
    transferServer.close();
    reply(150, QString("Opening data channel for directory listing of \"%1\"").arg(currentFtpDir));
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    for(QString name : fileManager->list(currUser, currentFtpDir)) {
        if(name == "." || name == "..") {
            continue;
        }
        QString permission = fileManager->getPermission(currUser, currentFtpDir + name);
        qDebug() << permission;
        tcpSocket->write(permission.toUtf8() + "\r\n");
    }
    tcpSocket->waitForBytesWritten(-1);
    tcpSocket->disconnectFromHost();
    reply(226, QString("Successfully transferred \"%1\"").arg(currentFtpDir));

}

void BackgroundThread::noop()
{
    reply(200, "");
}

void BackgroundThread::retr(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QString filePath = name;
    if(!filePath.startsWith('/')) {
        filePath = currentFtpDir + filePath;
    }
    qDebug() << "retr:" << filePath;
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isReadable(currUser, filePath)) {
        reply(550, QString("%1 can't read").arg(filePath));
        return;
    }
    if(!transferServer.hasPendingConnections()) {
        if(!transferServer.waitForNewConnection()) {
            transferServer.close();
            return;
        }
    }
    QTcpSocket* tcpSocket = transferServer.nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
    transferServer.close();
    reply(150, QString("Opening data channel for directory listing of \"%1\"").arg(currentFtpDir));
    QFile file(fileManager->mapPath(currUser, filePath));
    if(!file.open(QIODevice::OpenModeFlag::ReadOnly)) {
        reply(550, QString("%1 can't read").arg(filePath));
        tcpSocket->disconnectFromHost();
        return;
    }
    tcpSocket->write(file.readAll());
    file.close();
    tcpSocket->waitForBytesWritten(-1);
    tcpSocket->disconnectFromHost();
    reply(226, QString("Successfully transferred \"%1\"").arg(filePath));
}

void BackgroundThread::stor(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QString filePath = name;
    if(!filePath.startsWith('/')) {
        filePath = currentFtpDir + filePath;
    }
    qDebug() << "stor:" << filePath;
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    if(!transferServer.hasPendingConnections()) {
        if(!transferServer.waitForNewConnection()) {
            transferServer.close();
            return;
        }
    }

    QTcpSocket* tcpSocket = transferServer.nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
    transferServer.close();
    reply(150, QString("Opening data channel for directory listing of \"%1\"").arg(currentFtpDir));
    QFile file(fileManager->mapPath(currUser, filePath));
    if(!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
        reply(550, QString("%1 can't write").arg(filePath));
        tcpSocket->disconnectFromHost();
        return;
    }
    tcpSocket->waitForDisconnected(-1);
    QDataStream stream(&file);
    stream << tcpSocket->readAll();
    file.close();
    reply(226, QString("Successfully transferred \"%1\"").arg(filePath));
}

void BackgroundThread::appe(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QString filePath = name;
    if(!filePath.startsWith('/')) {
        filePath = currentFtpDir + filePath;
    }
    qDebug() << "stor:" << filePath;
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    if(!transferServer.hasPendingConnections()) {
        if(!transferServer.waitForNewConnection()) {
            transferServer.close();
            return;
        }
    }

    QTcpSocket* tcpSocket = transferServer.nextPendingConnection();
    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
    transferServer.close();
    reply(150, QString("Opening data channel for directory listing of \"%1\"").arg(currentFtpDir));
    QFile file(fileManager->mapPath(currUser, filePath));
    if(!file.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Append)) {
        reply(550, QString("%1 can't write").arg(filePath));
        tcpSocket->disconnectFromHost();
        return;
    }
    tcpSocket->waitForDisconnected(-1);
    QDataStream stream(&file);
    stream << tcpSocket->readAll();
    file.close();
    reply(226, QString("Successfully transferred \"%1\"").arg(filePath));
}

void BackgroundThread::rnfr(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    QString filePath = name;
    if(!filePath.startsWith('/')) {
        filePath = currentFtpDir + filePath;
    }
    rnfrName = filePath;
    reply(350, "wait RNTO");
}

void BackgroundThread::rnto(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    QFile file(fileManager->mapPath(currUser, rnfrName));
    rnfrName.clear();
    if(file.rename(name)) {
        reply(250, "rename successful");
    }
    else {
        reply(502, file.errorString());
    }
}

void BackgroundThread::dele(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    QString filePath = name;
    if(!filePath.startsWith('/')) {
        filePath = currentFtpDir + filePath;
    }
    QFile file(fileManager->mapPath(currUser, filePath));
    if(file.remove()) {
        reply(250, QString("delete %1 successful").arg(filePath));
    }
    else {
        reply(550, file.errorString());
    }
}

void BackgroundThread::rmd(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    QDir dir(fileManager->mapPath(currUser, currentFtpDir));
    if(dir.rmdir(name)) {
        reply(250, QString("delete %1 successful").arg(QString(name)));
    }
    else {
        reply(550, QString("delete %1 fail").arg(QString(name)));
    }
}

void BackgroundThread::mkd(QByteArray name)
{
    if(state != State::logged) {
        reply(530, "not logging");
        return;
    }
    QSharedPointer<FileManager::IFileManager> fileManager = server->fileManager();
    if(!fileManager->isWritable(currUser, currentFtpDir)) {
        reply(550, QString("%1 can't write").arg(currentFtpDir));
        return;
    }
    QDir dir(fileManager->mapPath(currUser, currentFtpDir));
    if(dir.mkdir(name)) {
        reply(250, QString("mkdir %1 successful").arg(QString(name)));
    }
    else {
        reply(550, QString("mkdir %1 fail").arg(QString(name)));
    }
}

} // namespace FtpServer
} // namespace QiDiTu
