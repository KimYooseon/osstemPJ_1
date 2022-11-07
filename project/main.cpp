#include "mainwindow.h"

#include <QApplication>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if(!db.open()) return false;

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS person(id INTEGER Primary Key,"
               "firstname VARCHAR(20) NOT NULL, lastname VARCHAR(20));");
    query.exec("INSERT INTO person VALUES(101, 'Yongsu', 'Kang');");
    query.exec("INSERT INTO person(firstname, lastname) VALUES('Soomi', 'Kim');");
    query.exec("INSERT INTO person(firstname, lastname) VALUES('Hanmi', 'Lee'), ('YoungHin', 'Suh'), ('YoungHwa', 'Ryu');");

    return true;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection()) return 1;

    QSqlQueryModel queryModel;
    queryModel.setQuery("select * from person");
    queryModel.setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel.setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    queryModel.setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));

    QTableView *tableview = new QTableView;
    tableview->setModel(&queryModel);
    tableview->setWindowTitle(QObject::tr("Query Model"));
    tableview->hideColumn(0);
    tableview->show();

    //MainWindow w;
    //w.show();
    return a.exec();
}
/*
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if(!db.open())return false;

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS person(id INTEGER Primary Key,"
               "firstname VARCHAR(20) NOT NULL, lastname VARCHAR(20));");
    query.exec("INSERT INTO person VALUES(101, 'Yongsu', 'Kang');");
    query.exec("INSERT INTO person(firstname, lastname) VALUES('Soomi', 'Kim')");
    query.exec("INSERT INTO person(firstname, lastname) VALUES"\
               "('Hanmi', 'Lee'), ('YoungJin', 'Suh'), ('YoungHwa', 'Ryu');");

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createConnection()) return 1;

    QSqlQueryModel queryModel;
    queryModel.setQuery("select * from person");
    queryModel.setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel.setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    queryModel.setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));

    QTableView *tableview = new QTableView;
    tableview->setModel(&queryModel);
    tableview->setWindowTitle(QObject::tr("Query Model"));
    tableview->hideColumn(0);
    tableview->show();

    return a.exec();
}
*/
