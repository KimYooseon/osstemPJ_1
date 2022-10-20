#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientmanagerform.h"
#include "productmanagerform.h"
#include "shopmanagerform.h"
#include "chatserverform.h"
#include "chatclientform.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ClientManagerForm *clientForm1 = new ClientManagerForm(0);
//    clientForm1->show();
    orderForm = new ShopManagerForm(this);
    //orderForm->setWindowTitle(tr("Order Info"));
    connect(orderForm, SIGNAL(destroyed()),
            orderForm, SLOT(deleteLater()));



    orderForm->loadData();
    orderForm->setWindowTitle(tr("Shop Info"));

    clientForm = new ClientManagerForm(this);
    connect(clientForm, SIGNAL(destroyed()),
            clientForm, SLOT(deleteLater()));
    connect(clientForm, SIGNAL(clientAdded(QString,int)),
            orderForm, SLOT(addClient(QString,int)));

    connect(clientForm, SIGNAL(clientModified(QString,int, int)),
            orderForm, SLOT(modifyClient(QString,int, int)));
    connect(clientForm, SIGNAL(clientRemoved(int)),
            orderForm, SLOT(removeClient(int)));


    clientForm->setWindowTitle(tr("Client Info"));
//    ui->tabWidget->addTab(clientForm, "&Client Info");

    productForm = new ProductManagerForm(this);
    connect(productForm, SIGNAL(destroyed()),
            productForm, SLOT(deleteLater()));
    connect(productForm, SIGNAL(productAdded(QString, int, QString, int)),
            orderForm, SLOT(addProduct(QString, int, QString, int)));

    connect(productForm, SIGNAL(productModified(QString,int, int)),
            orderForm, SLOT(modifyProduct(QString,int, int)));
    connect(productForm, SIGNAL(productRemoved(int)),
            orderForm, SLOT(removeProduct(int)));

    productForm->loadData();
    productForm->setWindowTitle(tr("Product Info"));


    serverForm = new ChatServerForm(this);
    serverForm->setWindowTitle(tr("Chatting Server"));


//    shoForm = new ShopManagerForm(this);
//    connect(orderForm, SIGNAL(destroyed()),
//            orderForm, SLOT(deleteLater()));
//    connect(orderForm, SIGNAL(orderAdded(QString)),
//            orderForm, SLOT(addOrder(QString)));
//    orderForm->loadData();
//    orderForm->setWindowTitle(tr("Shop Info"));



    connect(orderForm, SIGNAL(SendPID(int)), productForm, SLOT(PIDsended(int)));
    connect(productForm, SIGNAL(sendPInfo(QString, int, QString)), orderForm, SLOT(PInfoSended(QString, int, QString)));

    connect(orderForm, SIGNAL(SendCID(int)), clientForm, SLOT(CIDsended(int)));
    connect(clientForm, SIGNAL(sendCInfo(QString, QString, QString)), orderForm, SLOT(CInfoSended(QString, QString, QString)));

    connect(clientForm, SIGNAL(clientAdded(QString, int)), serverForm, SLOT(addClientToServer(QString, int)));




    connect(clientForm, SIGNAL(clientModified(QString, int, int)),
            serverForm, SLOT(modifyClientFromServer(QString, int, int)));
    connect(clientForm, SIGNAL(clientRemoved(int)),
            serverForm, SLOT(removeClientFromServer(int)));



    clientForm->loadData();
    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(clientForm);
    ui->mdiArea->addSubWindow(productForm);
    ui->mdiArea->addSubWindow(orderForm);
    ui->mdiArea->addSubWindow(serverForm);
    ui->mdiArea->setActiveSubWindow(cw);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClient_triggered()
{
    if(clientForm != nullptr) {
        clientForm->setFocus();
    }
}

void MainWindow::on_actionProduct_triggered()
{
    if(productForm != nullptr) {
        productForm->setFocus();
    }
}

void MainWindow::on_actionOrder_triggered()
{
    if(orderForm != nullptr) {
        orderForm->setFocus();
    }
}


void MainWindow::on_actionChat_triggered()
{
    wd = new Widget;
    wd->show();
}

