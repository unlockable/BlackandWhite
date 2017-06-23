/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView_1;
    QPushButton *connectButton;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(714, 673);
        pushButton_1 = new QPushButton(Dialog);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(110, 500, 71, 121));
        pushButton_2 = new QPushButton(Dialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(180, 500, 71, 121));
        pushButton_3 = new QPushButton(Dialog);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(250, 500, 71, 121));
        pushButton_4 = new QPushButton(Dialog);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(320, 500, 71, 121));
        pushButton_5 = new QPushButton(Dialog);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(390, 500, 71, 121));
        pushButton_6 = new QPushButton(Dialog);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(460, 500, 71, 121));
        pushButton_7 = new QPushButton(Dialog);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(530, 500, 71, 121));
        pushButton_8 = new QPushButton(Dialog);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(600, 500, 71, 121));
        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 500, 71, 121));
        graphicsView = new QGraphicsView(Dialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(90, 150, 181, 271));
        graphicsView_1 = new QGraphicsView(Dialog);
        graphicsView_1->setObjectName(QString::fromUtf8("graphicsView_1"));
        graphicsView_1->setGeometry(QRect(420, 150, 181, 271));
        connectButton = new QPushButton(Dialog);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(10, 640, 80, 21));
        lineEdit = new QLineEdit(Dialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(90, 30, 511, 71));
        lineEdit_2 = new QLineEdit(Dialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(460, 630, 211, 31));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_1->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        pushButton_7->setText(QString());
        pushButton_8->setText(QString());
        pushButton->setText(QString());
        connectButton->setText(QApplication::translate("Dialog", "connect", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("Dialog", "Your points : 0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
