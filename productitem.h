#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QTreeWidgetItem>

class ProductItem: public QTreeWidgetItem
{
public:
    explicit ProductItem(int id = 0, QString = "", int price = 0, QString = " ");
    int id() const;

    QString getProductName() const;
    void setProductName(QString&);

    int getPrice() const;
    void setPrice(int&);

    QString getCategory() const;
    void setCategory(QString&);
    bool operator==(const ProductItem &other) const;
};

#endif // PRODUCTITEM_H
