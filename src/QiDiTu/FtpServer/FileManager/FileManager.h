#pragma once

#include "IFileManager.h"

#include <QDir>

namespace QiDiTu {
namespace FtpServer {
namespace FileManager {

class FileManager : public IFileManager
{
private:
    const QString currentPath{QDir::currentPath() + '/'};

public:
    using IFileManager::IFileManager;
    virtual QString mapPath(QSharedPointer<User::IUser> user,
                            const QString &ftpPath) const override;
    virtual bool dirExists(QSharedPointer<User::IUser> user,
                           const QString &ftpPath) const override;
    virtual bool mkPath(QSharedPointer<User::IUser> user,
                        const QString& ftpPath) const override;
    virtual qint64 size(QSharedPointer<User::IUser> user,
                        const QString& ftpPath) const override;
    virtual QStringList list(QSharedPointer<User::IUser> user,
                             const QString& ftpPath) const override;
    virtual QString getPermission(QSharedPointer<User::IUser> user,
                                  const QString& ftpPath) const override;
    virtual bool fileExists(QSharedPointer<User::IUser> user,
                            const QString& ftpPath) const override;
    virtual bool isReadable(QSharedPointer<User::IUser> user,
                        const QString& ftpPath) const override;
    virtual bool isWritable(QSharedPointer<User::IUser> user,
                         const QString& ftpPath) const override;

};

} // namespace FileManager
} // namespace FtpServer
} // namespace QiDiTu
