#include "AnonymousUser.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

QString AnonymousUser::username() const
{
    return QStringLiteral("anonymous");
}

bool AnonymousUser::isNeedPassword() const
{
    return false;
}

bool AnonymousUser::isPassword(const QString &password) const
{
    Q_UNUSED(password);
    return true;
}

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
