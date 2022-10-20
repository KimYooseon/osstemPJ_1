#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDataStream>

class QTextEdit;
class QLineEdit;
class QTcpSocket;
class QPushButton;

typedef struct {        //채팅 프로토콜 설계
    int type;           //채팅의 목적
    char data[1020];    //전송되는 데이터
} chatProtocolType;

typedef enum {
    Chat_Login,     //로그인        ->초대를 위한 정보 저장
    Chat_In,        //채팅방 입장
    Chat_Talk,      //채팅          ->초대 불가능
    Chat_Close,     //채팅방 퇴장    ->초대 가능
    Chat_LogOut,    //로그아웃(서버단절)
    Chat_Invite,    //초대
    Chat_KickOut,   //강퇴
    Chat_FileTrans_Start,   //파일 전송 시작(파일명) -->파일 오픈
    Chat_FileTranfer,       //파일 데이터 전송      --> 데이터를 파일에 저장
    Chat_FileTrans_End,     //파일 전송 완료        -->파일 닫기
} Chat_Status;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void connectToServer( );
    void receiveData( );			// 서버에서 데이터가 올 때
    void sendData( );			// 서버로 데이터를 보낼 때
    void disconnect( );

private:
    void closeEvent(QCloseEvent*) override;

    QLineEdit *name;        //ID(이름)을 입력하는 창
    QTextEdit *message;		// 서버에서 오는 메세지 표시용
    QLineEdit *inputLine;		// 서버로 보내는 메시지 입력용
    QPushButton* sentButton;
    QPushButton* connectButton;
    QTcpSocket *clientSocket;		// 클라이언트용 소켓
};
#endif // WIDGET_H
