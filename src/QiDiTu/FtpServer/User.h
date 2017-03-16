#pragma once

#include <QObject>

namespace QiDiTu {
namespace FtpServer {

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

signals:

public slots:
};

} // namespace FtpServer
} // namespace QiDiTu

#endif // USER_H
