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

bool User::isPassword(const QString &password) const
{
    return _password == password;
}

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
