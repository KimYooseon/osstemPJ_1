#include "productmanagerform.h"
#include "ui_productmanagerform.h"
#include "productitem.h"

#include <QFile>
#include <QMenu>

ProductManagerForm::ProductManagerForm(QWidget *parent) :
    QWidget(parent), ui(new Ui::ProductManagerForm)
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
    ui->treeWidget->setColumnWidth(1,130);
    ui->treeWidget->setColumnWidth(2,100);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

}

void ProductManagerForm::loadData()
{

    QFile file("productlist.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");
        if(row.size()) {
            int id = row[0].toInt();
            int price = row[2].toInt();
            ProductItem* p = new ProductItem(id, row[1], price, row[3]);
            ui->treeWidget->addTopLevelItem(p);
            productList.insert(id, p);

            emit productAdded(row[1], row[2].toInt(), row[3], row[0].toInt());
        }
    }
    file.close( );
}


ProductManagerForm::~ProductManagerForm()
{
    delete ui;

    QFile file("productlist.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : productList) {
        ProductItem* p = v;
        out << p->id() << ", " << p->getProductName() << ", ";
        out << p->getPrice() << ", ";
        out << p->getCategory() << "\n";
    }
    file.close( );
}

//

int ProductManagerForm::makeId( )
{
    if(productList.size( ) == 0) {
        return 10000;
    } else {
        auto id = productList.lastKey();
        return ++id;
    }
}

void ProductManagerForm::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int i = ui->treeWidget->currentIndex().row();
        productList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
//        delete item;
        ui->treeWidget->update();
        emit productRemoved(i);
    }
}

void ProductManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}


void ProductManagerForm::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
//    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    //MatchCaseSensitive: 대소문자 구별, MatchContains: 포함하는지 확인
    //auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
    //               : Qt::MatchCaseSensitive;
    auto flag = (i==0 || i==2)? Qt::MatchCaseSensitive|Qt::MatchContains
                       : Qt::MatchCaseSensitive;

    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            ProductItem* p = static_cast<ProductItem*>(i);
            int id = p->id();
            QString productname = p->getProductName();
            int price = p->getPrice();
            QString category = p->getCategory();
            ProductItem* item = new ProductItem(id, productname, price, category);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}


void ProductManagerForm::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int i = ui->treeWidget->currentIndex().row();
        int key = item->text(0).toInt();
        int price = item->text(2).toInt();
        ProductItem* p = productList[key];
        //id productname price category
        QString productname, category;
        productname = ui->productnameLineEdit->text();
        price = ui->priceLineEdit->text().toInt();
        category = ui->categoryLineEdit->text();
        p->setProductName(productname);
        p->setPrice(price);
        p->setCategory(category);
        productList[key] = p;

        emit productModified(productname, key, i);
    }
}

void ProductManagerForm::on_addPushButton_clicked()
{
    QString productname, category;
    int price;
    int id = makeId( );

    if(ui->productnameLineEdit->text() == "" || ui->priceLineEdit->text() == "" || ui->categoryLineEdit->text() == "")
    {
        return;
    }
    productname = ui->productnameLineEdit->text();
    price = ui->priceLineEdit->text().toInt();
    category = ui->categoryLineEdit->text();
    if(productname.length()) {
        ProductItem* p = new ProductItem(id, productname, price, category);
        productList.insert(id, p);
        ui->treeWidget->addTopLevelItem(p);
        emit productAdded(productname, price, category, id);
    }
}

void ProductManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->idLineEdit->setText(item->text(0));
    ui->productnameLineEdit->setText(item->text(1));
    ui->priceLineEdit->setText(item->text(2));
    ui->categoryLineEdit->setText(item->text(3));
    ui->toolBox->setCurrentIndex(0);
}

//void ProductManagerForm::getpname(int i){
//    i = ui->searchComboBox->currentIndex();
//    auto flag = (i==0 || i==2)? Qt::MatchCaseSensitive|Qt::MatchContains
//                       : Qt::MatchCaseSensitive;

//    {
//        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

//        foreach(auto i, items) {
//            ProductItem* p = static_cast<ProductItem*>(i);
//            int id = p->id();
//            QString productname = p->getProductName();
//            int price = p->getPrice();
//            QString category = p->getCategory();
//            ProductItem* item = new ProductItem(id, productname, price, category);
//            ui->searchTreeWidget->addTopLevelItem(item);
//        }
//    }

//}

void ProductManagerForm::PIDsended(int id){
    ProductItem *p = productList[id];
    QString pname = p->getProductName();
    int price = p->getPrice();
    QString category = p->getCategory();

    emit sendPInfo(pname, price, category);
}

