#pragma once

#include "IUSerManager.h"

#include <QSharedPointer>
#include <QHash>

namespace QiDiTu {
namespace FtpServer {
namespace User {

class IUser;

class UserManager : public IUserManager
{
    Q_OBJECT
private:
    QHash<QString, QSharedPointer<IUser>> users;

public:
    explicit UserManager(QObject* parent = nullptr);
    UserManager(QSharedPointer<IUser> user, QObject* parent = nullptr);
    virtual QSharedPointer<IUser> getUser(const QString& username) override;
    virtual void addUser(QSharedPointer<IUser> user) override;
    virtual bool removeUser(const QString &username) override;

};

}
}
} // namespace QiDiTu
