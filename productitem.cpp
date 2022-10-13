#include "productitem.h"

ProductItem::ProductItem(int id, QString productname, int price, QString category)
{
    setText(0, QString::number(id));
    setText(1, productname);
    setText(2, QString::number(price));
    setText(3, category);
}

QString ProductItem::getProductName() const
{
    return text(1);
}

void ProductItem::setProductName(QString& productname)
{
    setText(1, productname);
}

int ProductItem::getPrice() const
{
    return text(2).toInt();
}
void ProductItem::setPrice(int& price)
{
    setText(2, QString::number(price));
}

QString ProductItem::getCategory() const
{
    return text(3);
}
void ProductItem::setCategory(QString& category)
{
    setText(3, category);
}

int ProductItem::id() const
{
    return text(0).toInt();
}
