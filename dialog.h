#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

namespace Ui {
class Dialog;
}

class QTcpSocket;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    int turn;
    int enemyCard;
    int myCard;
    int draws;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void set_enemyCard_Black();
    void set_enemyCard_White();
    void set_enemyCard_zero();
    void set_enemyCard_one();
    void set_enemyCard_two();
    void set_enemyCard_three();
    void set_enemyCard_four();
    void set_enemyCard_five();
    void set_enemyCard_six();
    void set_enemyCard_seven();
    void set_enemyCard_eight();
    void ServiceNameSlot(const QString x);



private slots:
    void readyRead(); //서버로부터 메시지가 온경우
    void connected(); //연결이 완료되었으면 서버에게 연결되었다고 알림
    void on_connectButton_clicked(); //연결 버튼을 누름

    void on_pushButton_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void cleanBoard();

    void set_cardSets();




private:
    Ui::Dialog *ui;
    QTcpSocket* socket; //나의 소켓
    QGraphicsScene displayedCard[2];
    int cards[9];
    QString eIP;//enemy's ip

};

#endif // DIALOG_H
