#ifndef SHOPITEM_H
#define SHOPITEM_H

#include "clientitem.h"
#include "productitem.h"
#include <QTreeWidgetItem>
class ShopItem:public QTreeWidgetItem
{

public:
    explicit ShopItem(int sid = 0, QString cid= 0, QString pid = 0, QString = " ", int count = 0);

    int SID() const;

    QString getCID() const;
    void setCID(QString&);

    QString getPID() const;
    void setPID(QString&);

    QString getDate() const;
    void setDate(QString&);

    int getCount() const;
    void setCount(int&);

    QString getPhoneNum() const;
    void setPhoneNum(QString&);
    bool operator==(const ShopItem &other) const;

};

#endif // SHOPITEM_H
