#include "widget.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDataStream>
#include <QTcpSocket>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include <QSettings>

#define BLOCK_SIZE      1024

Widget::Widget(QWidget *parent) : QWidget(parent) {
    // 연결한 서버 정보 입력을 위한 위젯들
    name = new QLineEdit(this);

    QLineEdit* serverAddress = new QLineEdit(this);
    serverAddress->setText("127.0.0.1");
    //    serverAddress->setInputMask("999.999.999.999;_");
    QRegularExpression re("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionValidator validator(re);
    serverAddress->setPlaceholderText("Server IP Address");
    serverAddress->setValidator(&validator);

    QLineEdit* serverPort = new QLineEdit(this);
    serverPort->setText("8000");
    serverPort->setInputMask("00000;_");
    serverPort->setPlaceholderText("Server Port No");

    connectButton = new QPushButton(tr("Log In"), this);
    connect(connectButton, &QPushButton::clicked,
            [=]{
        if(connectButton->text() == tr("Log In")) {
            clientSocket->connectToHost(serverAddress->text( ),
                                                 serverPort->text( ).toInt( ));
            connectToServer( );
            connectButton->setText(tr("Chat in"));
        } else {
            QByteArray bytearray;
            chatProtocolType data;      //구조체를 채움 -> type과 data를 통해 데이터를 만들었었는데,
            data.type = Chat_In;
            qstrcpy(data.data, name->text().toStdString().data());

            QByteArray sendArray;
            QDataStream out(&sendArray, QIODevice::WriteOnly);
            out << data.type;
            out.writeRawData(data.data, 1020);
            clientSocket->write(sendArray);
        }
        connectButton->setDisabled(true);
        inputLine->setEnabled(true);
        sentButton->setEnabled(true);
    } );

    QHBoxLayout *serverLayout = new QHBoxLayout;
    serverLayout->addWidget(name);
    serverLayout->addStretch(1);
    serverLayout->addWidget(serverAddress);
    serverLayout->addWidget(serverPort);
    serverLayout->addWidget(connectButton);

    message = new QTextEdit(this);		// 서버에서 오는 메시지 표시용
    message->setReadOnly(true);

    // 서버로 보낼 메시지를 위한 위젯들
    inputLine = new QLineEdit(this);
    connect(inputLine, SIGNAL(returnPressed( )), SLOT(sendData( )));
    connect(inputLine, SIGNAL(returnPressed( )), inputLine, SLOT(clear( )));
    sentButton = new QPushButton("Send", this);
    connect(sentButton, SIGNAL(clicked( )), SLOT(sendData( )));
    connect(sentButton, SIGNAL(clicked( )), inputLine, SLOT(clear( )));
    inputLine->setEnabled(false);
    sentButton->setEnabled(false);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputLine);
    inputLayout->addWidget(sentButton);

    // 종료 기능
    QPushButton* quitButton = new QPushButton("Quit", this);
    connect(quitButton, SIGNAL(clicked( )), qApp, SLOT(quit( )));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(serverLayout);
    mainLayout->addWidget(message);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    clientSocket = new QTcpSocket(this);			// 클라이언트 소켓 생성
    connect(clientSocket, &QAbstractSocket::errorOccurred,
            [=]{ qDebug( ) << clientSocket->errorString( ); }); //에러 발생시 에러메세지
    connect(clientSocket, SIGNAL(readyRead( )), SLOT(receiveData( )));
    connect(clientSocket, SIGNAL(disconnected( )), SLOT(disconnect( )));
    setWindowTitle(tr("Chat Client"));
    QSettings settings("ChatClient", "Chat Client");
    name->setText(settings.value("ChatClient/ID").toString());
}

Widget::~Widget( )
{
    clientSocket->close( );
    QSettings settings("ChatClient", "Chat Client");
    settings.setValue("ChatClient/ID", name->text());
}

void Widget::closeEvent(QCloseEvent*)   //창이 닫히면 로그아웃
{
    chatProtocolType data;
    data.type = Chat_LogOut;
    qstrcpy(data.data, name->text().toStdString().data());

    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << data.type;
    out.writeRawData(data.data, 1020);
    clientSocket->write(sendArray);

    clientSocket->disconnectFromHost();
    while(clientSocket->waitForDisconnected(1000))
        QThread::usleep(10);
}

void Widget::receiveData( ) //서버쪽에서 오는 데이터를 받음
{
    QTcpSocket *clientSocket = dynamic_cast<QTcpSocket *>(sender( ));
    if (clientSocket->bytesAvailable( ) > BLOCK_SIZE) return;
    QByteArray bytearray = clientSocket->read(BLOCK_SIZE);
    chatProtocolType data;
    QDataStream in(&bytearray, QIODevice::ReadOnly);
    in >> data.type;
    in.readRawData(data.data, 1020);

    qDebug( ) << data.type;
    switch(data.type) {
    case Chat_Talk:
        message->append(QString(data.data));
        break;
        inputLine->setEnabled(true);
        sentButton->setEnabled(true);
        connectButton->setDisabled(true);
    case Chat_KickOut:
        QMessageBox::critical(this, tr("Chatting Client"), \
                              tr("Kick out from Server"));
        inputLine->setEnabled(false);
        sentButton->setEnabled(false);
        connectButton->setEnabled(true);
        break;
    case Chat_Invite:
        QMessageBox::critical(this, tr("Chatting Client"), \
                              tr("Invited from Server"));
        inputLine->setEnabled(true);
        sentButton->setEnabled(true);
        connectButton->setDisabled(true);
        break;
    };
}

void Widget::sendData(  )
{
    QString str = inputLine->text( );
    if(str.length( )) {
        QByteArray bytearray;
        bytearray = str.toUtf8( );
        message->append("<font color=red>나</font> : " + str);

        chatProtocolType data;
        data.type = Chat_Talk;
        qstrcpy(data.data, bytearray.data());

        QByteArray sendArray;
        QDataStream out(&sendArray, QIODevice::WriteOnly);
        out << data.type;
        out.writeRawData(data.data, 1020);
        clientSocket->write(sendArray);
    }
}

void Widget::connectToServer( )
{
    chatProtocolType data;
    data.type = Chat_Login;
    qstrcpy(data.data, name->text().toStdString().data());

    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << data.type;
    out.writeRawData(data.data, 1020);
    clientSocket->write(sendArray);
}

void Widget::disconnect( )
{
    QMessageBox::critical(this, tr("Chatting Client"), \
                          tr("Disconnect from Server"));
    inputLine->setEnabled(false);
    sentButton->setEnabled(false);
//    close( );
}
