#pragma once

#include "IUser.h"

namespace QiDiTu {
namespace FtpServer {
namespace User {

class User : public IUser
{
    Q_OBJECT
private:
    QString _username;
    QString _password;
    bool _isNeedPassword;

public:
    explicit User(QObject *parent = 0);

    virtual QString username() const override;
    virtual bool isNeedPassword() const override;

};

} // namespace User
} // namespace FtpServer
} // namespace QiDiTu
