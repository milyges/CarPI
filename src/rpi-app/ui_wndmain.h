/********************************************************************************
** Form generated from reading UI file 'wndmain.ui'
**
** Created: Wed Jun 25 22:19:46 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WNDMAIN_H
#define UI_WNDMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "scrolledlabel.h"

QT_BEGIN_NAMESPACE

class Ui_WndMain
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frameTitle;
    QHBoxLayout *horizontalLayout;
    QLabel *lbSourceIcon;
    ScrolledLabel *lbRadioText;
    QSpacerItem *horizontalSpacer;
    QLabel *lbBluetoothIcon;
    QLabel *lbTime;
    QStackedWidget *mainWidget;

    void setupUi(QMainWindow *WndMain)
    {
        if (WndMain->objectName().isEmpty())
            WndMain->setObjectName(QString::fromUtf8("WndMain"));
        WndMain->resize(480, 272);
        centralwidget = new QWidget(WndMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frameTitle = new QFrame(centralwidget);
        frameTitle->setObjectName(QString::fromUtf8("frameTitle"));
        frameTitle->setFrameShape(QFrame::StyledPanel);
        frameTitle->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameTitle);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbSourceIcon = new QLabel(frameTitle);
        lbSourceIcon->setObjectName(QString::fromUtf8("lbSourceIcon"));

        horizontalLayout->addWidget(lbSourceIcon);

        lbRadioText = new ScrolledLabel(frameTitle);
        lbRadioText->setObjectName(QString::fromUtf8("lbRadioText"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        font.setPointSize(16);
        lbRadioText->setFont(font);

        horizontalLayout->addWidget(lbRadioText);

        horizontalSpacer = new QSpacerItem(179, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lbBluetoothIcon = new QLabel(frameTitle);
        lbBluetoothIcon->setObjectName(QString::fromUtf8("lbBluetoothIcon"));

        horizontalLayout->addWidget(lbBluetoothIcon);

        lbTime = new QLabel(frameTitle);
        lbTime->setObjectName(QString::fromUtf8("lbTime"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setWeight(75);
        lbTime->setFont(font1);

        horizontalLayout->addWidget(lbTime);


        verticalLayout->addWidget(frameTitle);

        mainWidget = new QStackedWidget(centralwidget);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));

        verticalLayout->addWidget(mainWidget);

        WndMain->setCentralWidget(centralwidget);

        retranslateUi(WndMain);

        QMetaObject::connectSlotsByName(WndMain);
    } // setupUi

    void retranslateUi(QMainWindow *WndMain)
    {
        WndMain->setWindowTitle(QApplication::translate("WndMain", "MainWindow", 0, QApplication::UnicodeUTF8));
        lbSourceIcon->setText(QApplication::translate("WndMain", "SRC", 0, QApplication::UnicodeUTF8));
        lbRadioText->setText(QApplication::translate("WndMain", " > ESKA < P2", 0, QApplication::UnicodeUTF8));
        lbBluetoothIcon->setText(QApplication::translate("WndMain", "BT", 0, QApplication::UnicodeUTF8));
        lbTime->setText(QApplication::translate("WndMain", "10:07", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WndMain: public Ui_WndMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WNDMAIN_H
