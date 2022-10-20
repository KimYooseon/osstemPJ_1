#ifndef PRODUCTMANAGERFORM_H
#define PRODUCTMANAGERFORM_H

#include <QWidget>
#include <QHash>

class ProductItem;
class QMenu;
class QTreeWidgetItem;

namespace Ui {
class ProductManagerForm;

}

class ProductManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductManagerForm(QWidget *parent = nullptr);
    ~ProductManagerForm();
    void loadData();

private slots:
    /* QTreeWidget을 위한 슬롯 */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void showContextMenu(const QPoint &);
    void removeItem();              /* QAction을 위한 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    void PIDsended(int);

signals:
    void productAdded(QString, int, QString, int);
    void productModified(QString, int, int);
    void productRemoved(int);
    void sendPInfo(QString, int, QString);


private:
    int makeId();

    QMap<int, ProductItem*> productList;
    Ui::ProductManagerForm *ui;
    QMenu* menu;
};

#endif // PRODUCTMANAGERFORM_H
