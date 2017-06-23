#include "dialog.h"
#include "chatserver.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // Dialog w;
    w.show();



    return a.exec();
}
