#pragma once

#include "IUSerManager.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {
class UserManager : public IUserManager
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = 0);
    QSharedPointer<IUser> getUser(const QString& username) override;
};
}
}
} // namespace QiDiTu
