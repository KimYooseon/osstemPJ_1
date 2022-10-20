#include "shopitem.h"

ShopItem::ShopItem(int sid, QString cid, QString pid, QString date, int count, int price, int totalprice, QString phonenumber, QString address)
{
    setText(0, QString::number(sid));
    setText(1, cid);
    setText(2, pid);
    setText(3, date);
    setText(4, QString::number(count));
    setText(5, QString::number(price));
    setText(6, QString::number(price*count));
    setText(7, phonenumber);
    setText(8, address);
}

QString ShopItem::getCID() const
{
    return text(1);
}

void ShopItem::setCID(QString& cid)
{
    setText(1, cid);
}

QString ShopItem::getPID() const
{
    return text(2);
}

void ShopItem::setPID(QString& pid)
{
    setText(2, pid);
}


QString ShopItem::getDate() const
{
    return text(3);
}
void ShopItem::setDate(QString& date)
{
    setText(3, date);
}

int ShopItem::getCount() const
{
    return text(4).toInt();
}

void ShopItem::setCount(int& count)
{
    setText(4, QString::number(count));
}

int ShopItem::SID() const
{
    return text(0).toInt();
}


int ShopItem::getPrice() const
{
    return text(5).toInt();
}

QString ShopItem::getPhoneNum() const{
    return text(7);
}
QString ShopItem::getAddress() const{
    return text(8);
}
