#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDataStream>

class QTextEdit;
class QLineEdit;
class QTcpSocket;
class QPushButton;
class QFile;
class QProgressDialog;

typedef enum {
    Chat_Login,             // 로그인(서버 접속)   --> 초대를 위한 정보 저장
    Chat_In,                // 채팅방 입장
    Chat_Talk,              // 채팅
    Chat_Out,             // 채팅방 퇴장         --> 초대 가능
    Chat_LogOut,            // 로그 아웃(서버 단절) --> 초대 불가능
    Chat_Invite,            // 초대
    Chat_KickOut,           // 강퇴
    Chat_FileTrans_Start,   // 파일 전송 시작(파일명) --> 파일 오픈
    Chat_FileTransfer,      // 파일 데이터 전송      --> 데이터를 파일에 저장
    Chat_FileTrans_End,     // 파일 전송 완료        --> 파일 닫기
} Chat_Status;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void receiveData( );			// 서버에서 데이터가 올 때
    void sendData( );               // 서버로 데이터를 보낼 때
    void disconnect( );
    void sendFile( );
    void sendProtocol(Chat_Status, char*, int = 1020);
    void goOnSend(qint64);

private:
    void closeEvent(QCloseEvent*) override;

    QLineEdit *name;                // ID(이름)을 입력하는 창
    QTextEdit *message;             // 서버에서 오는 메세지 표시용
    QLineEdit *inputLine;           // 서버로 보내는 메시지 입력용
    QPushButton *connectButton;     // 서버 로그인 등 접속 처리
    QPushButton *sentButton;        // 메시지 전송
    QPushButton* fileButton;        // 파일 전송
    QTcpSocket *clientSocket;		// 클라이언트용 소켓
    QProgressDialog* progressDialog;    // 파일 진행 확인
    QFile* file;
    qint64 loadSize;
    qint64 byteToWrite;
    qint64 totalSize;

    bool isSent;
};
#endif // WIDGET_H
