#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

class chatServer : public QTcpServer
{
    Q_OBJECT
signals:
    void oddSignal();
    void evenSignal();
    void zeroSignal();
    void oneSignal();
    void twoSignal();
    void threeSignal();
    void fourSignal();
    void fiveSignal();
    void sixSignal();
    void sevenSignal();
    void eightSignal();
    void trunEndSignal();

public:
    chatServer(QObject* parent=0);

private slots:
    void readyRead(); //클라이언트로부터 메세지가 전달됨을 감지하는 함수
    void disconnected();  //클라이언트가 접속을 끊을 경우를 감지하는 함수
    void sendUserList();  //새로 들어온 유저의 닉네임을 모든 클라이언트에게 뿌려주는 함수


protected:
    void incomingConnection(int socketfd);  //처음 유저가 접속을 시도할때 발생하는 함수

private:
    int userAmount; //유저 닉네임에 번호를 매김
    QSet<QTcpSocket*> clients;  //클라이언트 소켓저장
    QMap<QTcpSocket*,QString> users; //유저 이름 저장
};

#endif // CHATSERVER_H
