#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <dialog.h> //add the include .h file of your form

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

signals:
    void ServiceNameSignal( const QString);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void openAbout(); //make a new function to call the window

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    Dialog     *winAbout; //make a new declaration for the new window


private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_1_clicked();
};

#endif // MAINWINDOW_H
