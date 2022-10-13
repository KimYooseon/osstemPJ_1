#include "shopmanagerform.h"
#include "ui_shopmanagerform.h"
#include "shopitem.h"

#include <QFile>
#include <QMenu>
#include <QDebug>

ShopManagerForm::ShopManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopManagerForm)
{
    ui->setupUi(this);
    //QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    //item->setText(0, "10000");
    //item->setText(1, "원주연");
    //item->setText(2, "010-1234-4567");
    //item->setText(3, "대한민국");

    QList<int> sizes;
    sizes << 540 << 400;
    ui->splitter->setSizes(sizes);
    ui->treeWidget->setColumnWidth(0,50);
    ui->treeWidget->setColumnWidth(1,100);
    ui->treeWidget->setColumnWidth(2,150);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;
    menu->addAction(removeAction);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

}

void ShopManagerForm::loadData()
{

    QFile file("shoplist.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");
        if(row.size()) {
            int sid = row[0].toInt();
            //int cid = row[1].toInt();
            //int pid = row[2].toInt();
            int count = row[4].toInt();
            ShopItem* s = new ShopItem(sid, row[1], row[2], row[3], count);
            ui->treeWidget->addTopLevelItem(s);
            shopList.insert(sid, s);

            emit orderAdded(row[1]);
        }
    }
    file.close( );
}


ShopManagerForm::~ShopManagerForm()
{
    delete ui;

    QFile file("shoplist.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : shopList) {
        ShopItem* s = v;
        out << s->SID() << ", ";
        out << s->getCID() << ", ";
        out << s->getPID() << ", ";
        out << s->getDate() << ", ";
        out << s->getCount() << "\n";
    }
    file.close( );
}

//

int ShopManagerForm::makeId( )
{
    if(shopList.size( ) == 0) {
        return 20000;
    } else {
        auto sid = shopList.lastKey();
        return ++sid;
    }
}

void ShopManagerForm::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        shopList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
//        delete item;
        ui->treeWidget->update();
    }
}

void ShopManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}


void ShopManagerForm::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
//    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i==0||i==4)? Qt::MatchCaseSensitive
                   : Qt::MatchCaseSensitive|Qt::MatchContains;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            ShopItem* s = static_cast<ShopItem*>(i);
            int sid = s->SID();
            QString cid = s->getCID();
            QString pid = s->getPID();
            QString date = s->getDate();
            int count = s->getCount();
            ShopItem* item = new ShopItem(sid, cid, pid, date, count);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}


void ShopManagerForm::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if(item != nullptr) {
        int key = item->text(0).toInt();
//        int cid = item->text(1).toInt();
//        int pid = item->text(2).toInt();
//        int count = item->text(4).toInt();

        ShopItem* s = shopList[key];

        //id productname price category
        QString date, cid, pid;
        int count;
        //cid = ui->shopcidLineEdit->text().toInt();
        //pid = ui->shoppidLineEdit->text().toInt();
        date = ui->dateLineEdit->text();
        count = ui->countLineEdit->text().toInt();


        s->setCID(cid);
        s->setPID(pid);
        s->setDate(date);
        s->setCount(count);
        shopList[key] = s;
    }
//    QTreeWidgetItem* item = ui->treeWidget->currentItem();
//    if(item != nullptr) {
//        int key = item->text(0).toInt();
//        int price = item->text(2).toInt();
//        ProductItem* p = productList[key];
//        //id productname price category
//        QString productname, category;
//        productname = ui->productnameLineEdit->text();
//        price = ui->priceLineEdit->text().toInt();
//        category = ui->categoryLineEdit->text();
//        p->setProductName(productname);
//        p->setPrice(price);
//        p->setCategory(category);
//        productList[key] = p;
}

void ShopManagerForm::on_addPushButton_clicked()
{
    QString date, cid, pid;
    int count;
    int sid = makeId( );

    cid = ui->shopcidcomboBox->currentText();
    pid = ui->shoppidcomboBox->currentText();
    date = ui->dateLineEdit->text();
    count = ui->countLineEdit->text().toInt();

    if(date.length()) {
        ShopItem* p = new ShopItem(sid, cid, pid, date, count);
        shopList.insert(sid, p);
        ui->treeWidget->addTopLevelItem(p);
        emit orderAdded(date);

    }
}

void ShopManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->shopsidLineEdit->setText(item->text(0));
    //ui->shopcidLineEdit->setText(item->text(1));
    ui->shoppidcomboBox->setCurrentText(item->text(2));
    ui->shopcidcomboBox->setCurrentText(item->text(1));

    ui->dateLineEdit->setText(item->text(3));
    ui->countLineEdit->setText(item->text(4));

    ui->toolBox->setCurrentIndex(0);
}


void ShopManagerForm::addClient(QString name, int cid)
{
    ui->shopcidcomboBox->addItem(name+"(" + QString::number(cid)+")");

}

void ShopManagerForm::addProduct(QString productname, int pid)
{
    ui->shoppidcomboBox->addItem(productname+"(" + QString::number(pid)+")");
}

void ShopManagerForm::modifyClient(QString name, int cid, int i)
{
    ui->shopcidcomboBox->setItemText(i, name+"(" + QString::number(cid)+")");
}

void ShopManagerForm::removeClient(int i)
{
    ui->shopcidcomboBox->removeItem(i);

}

void ShopManagerForm::modifyProduct(QString productname, int pid, int i)
{
    ui->shoppidcomboBox->setItemText(i, productname+"(" + QString::number(pid)+")");

}

void ShopManagerForm::removeProduct(int i)
{
    ui->shoppidcomboBox->removeItem(i);

}
