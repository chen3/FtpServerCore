#pragma once

#include "IUser.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

class User : public IUser
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

};

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
