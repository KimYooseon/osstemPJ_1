#ifndef SHOPMANAGERFORM_H
#define SHOPMANAGERFORM_H

#include <QWidget>
#include <QHash>

class ShopItem;
class QMenu;
class QTreeWidgetItem;

namespace Ui {
class ShopManagerForm;
}

class ShopManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ShopManagerForm(QWidget *parent = nullptr);
    ~ShopManagerForm();
    void loadData();
public slots:
    void addClient(QString, int);
    void addProduct(QString, int, QString, int);
    void modifyClient(QString, int, int);
    void removeClient(int);
    void modifyProduct(QString, int, int);
    void removeProduct(int);



private slots:
    /* QTreeWidget을 위한 슬롯 */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void showContextMenu(const QPoint &);
    void removeItem();              /* QAction을 위한 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    //void on_customer_Clicked();

    void on_shoppidcomboBox_textActivated(const QString &arg1);
    void PInfoSended(QString, int, QString);

    void on_shopcidcomboBox_textActivated(const QString &arg1);
    void CInfoSended(QString, QString, QString);


signals:
    void orderAdded(QString);
    void sendpname(int);
    void SendPID(int);
    void SendCID(int);
private:
    int makeId();

    QMap<int, ShopItem*> shopList;
    Ui::ShopManagerForm *ui;
    QMenu* menu;
};

#endif // SHOPMANAGERFORM_H
