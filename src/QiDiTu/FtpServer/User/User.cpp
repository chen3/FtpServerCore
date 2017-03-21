#include "User.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

User::User(QObject *parent)
    : IUser(parent)
{

}

QString User::username() const
{
    return _username;
}

bool User::isNeedPassword() const
{
    return _isNeedPassword;
}

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
