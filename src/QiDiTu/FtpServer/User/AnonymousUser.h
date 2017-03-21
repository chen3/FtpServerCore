#pragma once

#include "IUser.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

class AnonymousUser : public IUser
{
public:
    using IUser::IUser;
    virtual QString username() const;
    virtual bool isNeedPassword() const;
};

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
