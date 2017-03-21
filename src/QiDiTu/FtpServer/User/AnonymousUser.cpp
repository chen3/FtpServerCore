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

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
