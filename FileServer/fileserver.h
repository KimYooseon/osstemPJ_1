#ifndef FILESERVER_H
#define FILESERVER_H

#include <QWidget>

class QTcpServer;
class QTcpSocket;
class QFile;
class QProgressDialog;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QLabel* infoLabel;
    QTcpServer *server;
    QTcpSocket *receivedSocket;
    QFile *newFile;
    QProgressDialog* progressDialog;

    QByteArray inBlock;
    QString filename;
    qint64 totalSize;
    qint64 byteReceived;

private slots:
    void acceptConnection();
    void readClient();
    void clickButton();
};
#endif // FILESERVER_H
