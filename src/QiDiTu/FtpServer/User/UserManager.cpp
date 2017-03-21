#include "UserManager.h"

#include "User.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {
UserManager::UserManager(QObject* parent)
    : IUserManager(parent)
{

}

UserManager::UserManager(QSharedPointer<IUser> user, QObject* parent)
    : UserManager(parent)
{
    addUser(user);
}

QSharedPointer<IUser> UserManager::getUser(const QString &username)
{
    return users.value(username);
}

void UserManager::addUser(QSharedPointer<IUser> user)
{
    users.insert(user->username(), user);
}

bool UserManager::removeUser(const QString &username)
{
    return users.remove(username) == 1;
}

}
}
} // namespace QiDiTu
