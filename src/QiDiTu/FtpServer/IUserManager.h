#pragma once

#include <QObject>

namespace QiDiTu {
namespace FtpServer {

class IUserManager : public QObject
{
public:
    using QObject::QObject;
};

}
}
