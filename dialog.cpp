#include "dialog.h"
#include "ui_dialog.h"
#include <QRegExp>
#include <QTcpSocket>
#include "chatserver.h"
#include <QRegExp>
#include <QTextCodec>
#include <QString>
#include <QThread>


QString eIP;

int points = 0;
bool myTurn = false;
bool enemyTurnEnd = false;


class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    for(int i = 0; i < 9; i++)
        cards[i] = i;
    set_cardSets();
    turn = 0;
    enemyCard = -1;
    myCard = -1;
    draws = 0;

    chatServer* server=new chatServer();
    bool success=server->listen(QHostAddress::Any,4200);
    if(!success)
    {
        printf("Could not listen on port 4200.\n");
    }

    connect(server, SIGNAL(evenSignal()), this, SLOT(set_enemyCard_Black()));
    connect(server, SIGNAL(oddSignal()), this, SLOT(set_enemyCard_White()));
    connect(server, SIGNAL(zeroSignal()), this, SLOT(set_enemyCard_zero()));
    connect(server, SIGNAL(oneSignal()), this, SLOT(set_enemyCard_one()));
    connect(server, SIGNAL(twoSignal()), this, SLOT(set_enemyCard_two()));
    connect(server, SIGNAL(threeSignal()), this, SLOT(set_enemyCard_three()));
    connect(server, SIGNAL(fourSignal()), this, SLOT(set_enemyCard_four()));
    connect(server, SIGNAL(fiveSignal()), this, SLOT(set_enemyCard_five()));
    connect(server, SIGNAL(sixSignal()), this, SLOT(set_enemyCard_six()));
    connect(server, SIGNAL(sevenSignal()), this, SLOT(set_enemyCard_seven()));
    connect(server, SIGNAL(eightSignal()), this, SLOT(set_enemyCard_eight()));
    connect(server, SIGNAL(trunEndSignal()), this, SLOT(cleanBoard()));

    printf("Ready\n");

}

Dialog::~Dialog()
{
    delete ui;
}

chatServer::chatServer(QObject* parent) : QTcpServer(parent)
{
    userAmount=-1;
}

void chatServer::incomingConnection(int socketfd)
{
    QTcpSocket* client=new QTcpSocket(this); //클라리언트 소켓 새성
    client->setSocketDescriptor(socketfd); //소켓드스크립터 설정
    clients.insert(client); //클라이언트 집합에 새로 들어온 클라이언트 추가


    printf("New Client from:%s\n",client->peerAddress().toString().toLocal8Bit().data());

    connect(client,SIGNAL(readyRead()),this,SLOT(readyRead()));

    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));

}

void chatServer::readyRead()
{
    QTcpSocket* client=(QTcpSocket*)sender(); //메시지가 들어온 클라이언트를 감지하는 함수 sender
    while(client->canReadLine())
    {
        QString line=QString::fromUtf8(client->readLine());
        printf("Read Line:%s\n",line.toLocal8Bit().data());

        QRegExp meRegex("^/me:(.*)$");

        myTurn = true; // set my turn true
        

        if(line== (QString)"0\n" || line== (QString)"2\n" || line== (QString)"4\n" || line== (QString)"6\n"
                || line== (QString)"8\n"){
            emit evenSignal();

            if(line == (QString)"0\n")
                emit zeroSignal();

            if(line == (QString)"2\n")
                emit twoSignal();

            if(line == (QString)"4\n")
                emit fourSignal();

            if(line == (QString)"6\n")
                emit sixSignal();

            if(line == (QString)"8\n")
                emit eightSignal();


        }

        else if(line== (QString)"1\n" || line== (QString)"3\n" || line== (QString)"5\n" || line== (QString)"7\n"
                   || line== (QString)"9\n"){
               emit oddSignal();


                if(line == (QString)"1\n")
                    emit oneSignal();

                if(line == (QString)"3\n")
                    emit threeSignal();

                if(line == (QString)"5\n")
                    emit fiveSignal();

                if(line == (QString)"7\n")
                    emit sevenSignal();

        }

        if(line == QString("turnEnd\n")){
            Sleeper::msleep(2000);
            emit trunEndSignal();
        }

        if(meRegex.indexIn(line)!=-1) //새로 들어온 유저면
        {
            QString user=QString("user%1").arg(++userAmount);
            users.insert(client,user);
            foreach(QTcpSocket* client,clients)
            {
                QString temp=QString("server:%1 is connneted\n").arg(user);
                client->write(temp.toUtf8());
                sendUserList();
            }

        }
        else if(users.contains(client)) //이미 있는 유저가 메시지를 보낸경우
        {
            QString message=line;
            QString user=users[client];
            printf("User:%s\n",user.toLocal8Bit().data());
            printf("Message:%s\n",message.toLocal8Bit().data());

            foreach(QTcpSocket* otherClient,clients)
                otherClient->write(QString("%1").arg(message).toUtf8());
        }
        else
        {
            qWarning()<<"Got bad message from client:"<<client->peerAddress().toString()<<line;
        }
    }
}

void chatServer::disconnected()
{
    QTcpSocket* client=(QTcpSocket*)sender();
    printf("Client disconnected:%s\n",client->peerAddress().toString().toLocal8Bit().data());

    clients.remove(client);

    QString user=users[client];
    users.remove(client);

    sendUserList();
    foreach(QTcpSocket* client,clients)
    {
        client->write(QString("server:"+user+"exit the server.\n").toUtf8());
    }

}


void chatServer::sendUserList()
{
    QStringList userList;
    foreach(QString user,users.values())
        userList<<user;

    foreach(QTcpSocket* client,clients)
        client->write(QString("users:"+ userList.join(",")+"\n").toUtf8());
}




//--------------------------pushButton ClickEvent-----------------------
void Dialog::on_pushButton_clicked()
{
    //ui->pushButton->setText(test);

    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_0.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton->hide();

    turn++;
    myCard = 0;
    if(enemyTurnEnd == true){
        socket->write(QString("0\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }


    myTurn = false;
    socket->write(QString("0\n").toUtf8());
}

void Dialog::on_pushButton_1_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_1.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_1->hide();

    turn++;
    myCard = 1;

    if(enemyTurnEnd == true){

        socket->write(QString("1\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }

    myTurn = false;
    socket->write(QString("1\n").toUtf8());
}


void Dialog::on_pushButton_2_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_2.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));


        displayedCard[0].addPixmap(pix);
        ui->graphicsView->setScene(&displayedCard[0]);\
        ui->graphicsView->show();
        ui->pushButton_2->hide();

        turn++;
        myCard = 2;
        if(enemyTurnEnd == true){

            socket->write(QString("2\n").toUtf8());
            socket->write(QString("turnEnd\n").toUtf8());

            cleanBoard();

            return;

        }

        myTurn = false;
        socket->write(QString("2\n").toUtf8());
}

void Dialog::on_pushButton_3_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_3.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_3->hide();

    turn++;
    myCard = 3;
    if(enemyTurnEnd == true){
        socket->write(QString("3\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;      
    }
    myTurn = false;
    socket->write(QString("3\n").toUtf8());

}


void Dialog::on_pushButton_4_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_4.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_4->hide();

    turn++;
    myCard = 4;
    if(enemyTurnEnd == true){
        socket->write(QString("4\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;
    }

    myTurn = false;
    socket->write(QString("4\n").toUtf8());
}

void Dialog::on_pushButton_5_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_5.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_5->hide();

    turn++;
    myCard = 5;
    if(enemyTurnEnd == true){

        socket->write(QString("5\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }

    myTurn = false;
    socket->write(QString("5\n").toUtf8());
}

void Dialog::on_pushButton_6_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_6.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_6->hide();

    turn++;
    myCard = 6;
    if(enemyTurnEnd == true){

        socket->write(QString("6\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }

    myTurn = false;
    socket->write(QString("6\n").toUtf8());
}

void Dialog::on_pushButton_7_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_7.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_7->hide();

    turn++;
    myCard = 7;
    if(enemyTurnEnd == true){

        socket->write(QString("7\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }


    myTurn = false;
    socket->write(QString("7\n").toUtf8());
}

void Dialog::on_pushButton_8_clicked()
{
    if(myTurn == false){
        ui->lineEdit->setText("Not Your turn wait..");
        return;
    }
    QPixmap pix("images/num_8.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
    ui->pushButton_8->hide();

    turn++;
    myCard = 8;
    if(enemyTurnEnd == true){

        socket->write(QString("8\n").toUtf8());
        socket->write(QString("turnEnd\n").toUtf8());

        cleanBoard();

        return;

    }


    myTurn = false;
    socket->write(QString("8\n").toUtf8());
   // pix.scaled(QSize(ui->graphicsView->width(), ui->graphicsView->height()),  Qt::KeepAspectRatio);
}

void Dialog::set_cardSets()
{
    // set card icons
        QPixmap pixmap;
        QIcon ButtonIcon(pixmap);

        pixmap.load("images/num_0.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);

        ui->pushButton->setIcon(ButtonIcon);
        ui->pushButton->setIconSize(pixmap.rect().size());
        ui->pushButton->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_1.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_1->setIcon(ButtonIcon);
        ui->pushButton_1->setIconSize(pixmap.rect().size());
        ui->pushButton_1->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_2.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_2->setIcon(ButtonIcon);
        ui->pushButton_2->setIconSize(pixmap.rect().size());
        ui->pushButton_2->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_3.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_3->setIcon(ButtonIcon);
        ui->pushButton_3->setIconSize(pixmap.rect().size());
        ui->pushButton_3->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_4.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_4->setIcon(ButtonIcon);
        ui->pushButton_4->setIconSize(pixmap.rect().size());
        ui->pushButton_4->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_5.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_5->setIcon(ButtonIcon);
        ui->pushButton_5->setIconSize(pixmap.rect().size());
        ui->pushButton_5->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_6.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_6->setIcon(ButtonIcon);
        ui->pushButton_6->setIconSize(pixmap.rect().size());
        ui->pushButton_6->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_7.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_7->setIcon(ButtonIcon);
        ui->pushButton_7->setIconSize(pixmap.rect().size());
        ui->pushButton_7->setFixedSize(pixmap.rect().size());

        pixmap.load("images/num_8.jpg");
        pixmap = pixmap.scaled(QSize(ui->pushButton->width()-2, ui->pushButton->height()-2));
        ButtonIcon.addPixmap(pixmap);
        ui->pushButton_8->setIcon(ButtonIcon);
        ui->pushButton_8->setIconSize(pixmap.rect().size());
        ui->pushButton_8->setFixedSize(pixmap.rect().size());
}


void Dialog::readyRead()
{
    while(socket->canReadLine())
    {
        QString line=QString::fromUtf8(socket->readLine());
        line=line.left(line.length()-1);
        printf("%s is keyword\n",line.toLocal8Bit().data());       

    }
}

void Dialog::connected()
{
    //socket->write(QString("/me:"+ui->messageLineEdit->text()+"\n").toUtf8());
    socket->write(QString("/me: connection is successful \n").toUtf8());
}

void Dialog::on_connectButton_clicked()
{
    socket=new QTcpSocket(this);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));

    socket->connectToHost(eIP.toLocal8Bit().data(), 4200);
    myTurn = true;
}
void Dialog::ServiceNameSlot(const QString x)
{
    eIP = x;
}

void Dialog::set_enemyCard_Black(){
    QPixmap pix("images/blackcard.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[1].addPixmap(pix);
    ui->graphicsView_1->setScene(&displayedCard[1]);\
    ui->graphicsView_1->show();
}

void Dialog::set_enemyCard_White(){
    QPixmap pix("images/whitecard.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[1].addPixmap(pix);
    ui->graphicsView_1->setScene(&displayedCard[1]);\
    ui->graphicsView_1->show();
}

void Dialog::cleanBoard(){

    enemyTurnEnd =false;

    printf("your card : %d\n enemycard: %d\n",myCard, enemyCard);
    if(myCard > enemyCard){
        points++;
        QString temp = "Turn : "+ QString::number(turn);

        temp += "  You win in this turn";
        ui->lineEdit->setText(temp);
        ui->lineEdit_2->setText("Your points: "+QString::number(points));

        myTurn = true;
    }
    
    else if(myCard < enemyCard){
        QString temp = "Turn : "+ QString::number(turn);
        temp += "  You lose in this turn";
        ui->lineEdit->setText(temp);
        ui->lineEdit_2->setText("Your points: "+QString::number(points));

        myTurn = false;
    }
    
    else{
        ui->lineEdit->setText("draw in this turn");
        ui->lineEdit_2->setText("Your points: "+QString::number(points));

        myTurn = true;
    }

    if(turn == 9){
        QString temp = "Game Ended!!! ";
        if((turn - draws) / 2 < points){
            temp += "You are Winner!!\n ";
        }
        else
            temp += "You lose!!!!!\n";

        ui->lineEdit->setText(temp);
        return;

    }

    QPixmap pix("images/whiteboard.jpg");
    pix = pix.scaled(QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2));
    displayedCard[1].addPixmap(pix);
    ui->graphicsView_1->setScene(&displayedCard[1]);\
    ui->graphicsView_1->show();
    displayedCard[0].addPixmap(pix);
    ui->graphicsView->setScene(&displayedCard[0]);\
    ui->graphicsView->show();
}

void Dialog::set_enemyCard_zero(){
    enemyCard = 0;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}

void Dialog::set_enemyCard_one(){
    enemyCard = 1;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}

void Dialog::set_enemyCard_two(){
    enemyCard = 2;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}

void Dialog::set_enemyCard_three(){
    enemyCard = 3;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}
void Dialog::set_enemyCard_four(){
    enemyCard = 4;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}

void Dialog::set_enemyCard_five(){
    enemyCard = 5;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}
void Dialog::set_enemyCard_six(){
    enemyCard = 6;
    enemyTurnEnd = true;
    printf("enemy Card:%d is set\n", enemyCard);
}

void Dialog::set_enemyCard_seven(){
    enemyCard = 7;
    enemyTurnEnd = true;
}
void Dialog::set_enemyCard_eight(){
    enemyCard = 8;
    enemyTurnEnd = true;
}

