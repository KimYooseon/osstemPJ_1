#ifndef CLIENTMANAGERFORM_H
#define CLIENTMANAGERFORM_H

#include <QWidget>
#include <QHash>

class ClientItem;
class QMenu;
class QTreeWidgetItem;

namespace Ui {
class ClientManagerForm;
}

class ClientManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagerForm(QWidget *parent = nullptr);
    ~ClientManagerForm();
    void loadData();

private slots:
    /* QTreeWidget을 위한 슬롯 */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void showContextMenu(const QPoint &);
    void removeItem();              /* QAction을 위한 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    void CIDsended(int);
signals:
    void clientAdded(QString, int);
    void clientModified(QString, int, int);
    void clientRemoved(int);
    void sendCInfo(QString, QString, QString);
private:
    int makeId();

    QMap<int, ClientItem*> clientList;
    Ui::ClientManagerForm *ui;
    QMenu* menu;
};

#endif // CLIENTMANAGERFORM_H
