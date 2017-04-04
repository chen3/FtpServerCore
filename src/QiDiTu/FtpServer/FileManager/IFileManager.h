#pragma once

#include <QSharedPointer>
#include <QStringList>

namespace QiDiTu {
namespace FtpServer {

namespace User {
class IUser;
} // namespace User

namespace FileManager {

class IFileManager : public QObject
{
public:
    using QObject::QObject;
    virtual QString mapPath(QSharedPointer<User::IUser> user,
                            const QString& ftpPath) const = 0;
    virtual bool dirExists(QSharedPointer<User::IUser> user,
                           const QString& ftpPath) const = 0;
    virtual bool mkPath(QSharedPointer<User::IUser> user,
                        const QString& ftpPath) const = 0;
    virtual qint64 size(QSharedPointer<User::IUser> user,
                        const QString& ftpPath) const = 0;
    virtual QStringList list(QSharedPointer<User::IUser> user,
                             const QString& ftpPath) const = 0;
    virtual QString getPermission(QSharedPointer<User::IUser> user,
                                  const QString& ftpPath) const = 0;
    virtual bool fileExists(QSharedPointer<User::IUser> user,
                            const QString& ftpPath) const = 0;
    virtual bool isReadable(QSharedPointer<User::IUser> user,
                            const QString& ftpPath) const = 0;
    virtual bool isWritable(QSharedPointer<User::IUser> user,
                            const QString& ftpPath) const = 0;

};

} // namespace FileManager

} // namespace FtpServer
} // namespace QiDiTu
