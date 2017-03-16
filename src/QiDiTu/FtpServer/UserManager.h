#pragma once

#include <QObject>

namespace QiDiTu {

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = 0);

signals:

public slots:
};

} // namespace QiDiTu

#endif // USERMANAGER_H
