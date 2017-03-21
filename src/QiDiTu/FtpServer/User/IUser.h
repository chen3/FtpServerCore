#pragma once

#include <QObject>

namespace QiDiTu {
namespace FtpServer {
namespace User {

class IUser : public QObject
{
public:
    using QObject::QObject;

    virtual QString username() const = 0;
    virtual bool isNeedPassword() const = 0;

};

}
}
}
