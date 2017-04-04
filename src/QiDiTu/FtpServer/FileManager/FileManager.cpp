#include "FileManager.h"

#include "QiDiTu/FtpServer/User/IUser.h"

#include <exception>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QLocale>

namespace QiDiTu {
namespace FtpServer {
namespace FileManager {

QString FileManager::mapPath(QSharedPointer<User::IUser> user,
                             const QString &ftpPath) const
{
    if(ftpPath.isEmpty() || ftpPath[0] != '/') {
        throw std::invalid_argument("ftpPath");
    }
    QString path = QDir::cleanPath(ftpPath);
    if(path.split('/').contains("..")) {
        throw std::invalid_argument("ftpPath");
    }
    path = currentPath + user->username() + path;
    return path;
}

bool FileManager::dirExists(QSharedPointer<User::IUser> user,
                            const QString &ftpPath) const
{
    return QDir(mapPath(user, ftpPath)).exists();
}

bool FileManager::mkPath(QSharedPointer<User::IUser> user,
                         const QString &ftpPath) const
{
    return QDir().mkpath(mapPath(user, ftpPath));
}

qint64 FileManager::size(QSharedPointer<User::IUser> user, const QString &ftpPath) const
{
    return QFileInfo(mapPath(user, ftpPath)).size();
}

QStringList FileManager::list(QSharedPointer<User::IUser> user,
                                   const QString &ftpPath) const
{
    if(!dirExists(user, ftpPath)) {
        throw std::invalid_argument("ftpPath");
    }
    return QDir(mapPath(user, ftpPath)).entryList();
}

QString FileManager::getPermission(QSharedPointer<User::IUser> user,
                                   const QString &ftpPath) const
{
    QString permission = "%1%2%3------- 1 ftp ftp %4 %5 %6";
    if(dirExists(user, ftpPath)) {
        permission = permission.arg('d');
    }
    else if(fileExists(user, ftpPath)) {
        permission = permission.arg('-');
    }
    else {
        throw std::invalid_argument("ftpPath");
    }
    permission = permission.arg(isReadable(user, ftpPath) ? 'r' : '-');
    permission = permission.arg(isWritable(user, ftpPath) ? 'w' : '-');
    permission = permission.arg(size(user, ftpPath), 14);
    QFileInfo fileInfo(mapPath(user, ftpPath));
    QLocale locale(QLocale::English);
    QDateTime lm = fileInfo.lastModified();
    if (lm.date().year() != QDate::currentDate().year()) {
        permission = permission.arg(locale.toString(lm.date(), "MMM dd  yyyy"));
    } else {
        permission = permission.arg(locale.toString(lm.date(), "MMM dd") + ' ' + locale.toString(lm.time(), "hh:mm"));
    }
    permission = permission.arg(fileInfo.fileName());
    return permission;
}

bool FileManager::fileExists(QSharedPointer<User::IUser> user,
                             const QString &ftpPath) const
{
    QFileInfo fileInfo(mapPath(user, ftpPath));
    return fileInfo.exists() && fileInfo.isFile();
}

bool FileManager::isReadable(QSharedPointer<User::IUser> user,
                             const QString &ftpPath) const
{
    return QFileInfo(mapPath(user, ftpPath)).isReadable();
}

bool FileManager::isWritable(QSharedPointer<User::IUser> user,
                             const QString &ftpPath) const
{
    return QFileInfo(mapPath(user, ftpPath)).isWritable();
}

} // namespace FileManager
} // namespace FtpServer
} // namespace QiDiTu
