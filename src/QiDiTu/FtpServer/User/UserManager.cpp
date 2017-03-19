#include "UserManager.h"

#include "User.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {
UserManager::UserManager(QObject *parent)
    : IUserManager(parent)
{

}

QSharedPointer<IUser> UserManager::getUser(const QString &username)
{
    return QSharedPointer<User>();
}

}
}
} // namespace QiDiTu
