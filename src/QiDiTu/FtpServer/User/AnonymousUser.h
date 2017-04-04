#pragma once

#include "IUser.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

class AnonymousUser : public IUser
{
public:
    using IUser::IUser;
    virtual QString username() const override;
    virtual bool isNeedPassword() const override;
    virtual bool isPassword(const QString &password) const override;

};

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
