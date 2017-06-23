#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::openAbout()
{
    QString IP = ui->lineEdit_2->text();
    winAbout = new Dialog(this);
    winAbout->show();
    connect(this, SIGNAL(ServiceNameSignal(const QString)), winAbout,SLOT(ServiceNameSlot(const QString)));
    ServiceNameSignal(IP);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_1_clicked()
{
    openAbout();
}

void MainWindow::on_pushButton_2_clicked()
{
    exit(1);
}
