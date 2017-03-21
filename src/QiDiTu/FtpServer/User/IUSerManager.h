#pragma once

#include <QObject>
#include <QSharedPointer>

namespace QiDiTu {
namespace FtpServer {
namespace User {

class IUser;

class IUserManager : public QObject
{
public:
    using QObject::QObject;
    virtual QSharedPointer<IUser> getUser(const QString& username) = 0;
    virtual void addUser(QSharedPointer<IUser> user) = 0;
    virtual bool removeUser(const QString& username) = 0;
};

}
}
}
