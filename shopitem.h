#ifndef SHOPITEM_H
#define SHOPITEM_H

#include "clientitem.h"
#include "productitem.h"
#include <QTreeWidgetItem>
class ShopItem:public QTreeWidgetItem
{

public:
    explicit ShopItem(int sid = 0, QString cid= 0, QString pid = 0, QString date = " ", int count = 0, int price=0, int totalprice = 0, QString phonenumber="", QString address="");

    int SID() const;

    QString getCID() const;
    void setCID(QString&);

    QString getPID() const;
    void setPID(QString&);

    QString getDate() const;
    void setDate(QString&);

    int getCount() const;
    void setCount(int&);

    int getPrice() const;

    QString getPhoneNum() const;
    void setPhoneNum(QString&);

    QString getAddress() const;

    bool operator==(const ShopItem &other) const;

};

#endif // SHOPITEM_H
