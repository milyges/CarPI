/********************************************************************************
** Form generated from reading UI file 'wndmain.ui'
**
** Created: Wed Jun 25 18:57:30 2014
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WndMain
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lRadioText;
    QSpacerItem *horizontalSpacer;
    QLabel *lRadioIcons;
    QHBoxLayout *horizontalLayout;
    QLabel *lSourceIcon;
    QLabel *lBluetoothState;
    QLabel *lTemp;
    QLabel *lTime;
    QStackedWidget *stackedWidget;
    QWidget *pageNavit;
    QVBoxLayout *verticalLayout_3;
    QWidget *pageDials;

    void setupUi(QMainWindow *WndMain)
    {
        if (WndMain->objectName().isEmpty())
            WndMain->setObjectName(QString::fromUtf8("WndMain"));
        WndMain->resize(480, 272);
        WndMain->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(WndMain);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 0, 1, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lRadioText = new QLabel(centralWidget);
        lRadioText->setObjectName(QString::fromUtf8("lRadioText"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        lRadioText->setFont(font);

        horizontalLayout_2->addWidget(lRadioText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lRadioIcons = new QLabel(centralWidget);
        lRadioIcons->setObjectName(QString::fromUtf8("lRadioIcons"));
        QFont font1;
        font1.setPointSize(8);
        lRadioIcons->setFont(font1);
        lRadioIcons->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
        lRadioIcons->setMargin(2);

        horizontalLayout_2->addWidget(lRadioIcons);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_4->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lSourceIcon = new QLabel(centralWidget);
        lSourceIcon->setObjectName(QString::fromUtf8("lSourceIcon"));

        horizontalLayout->addWidget(lSourceIcon);

        lBluetoothState = new QLabel(centralWidget);
        lBluetoothState->setObjectName(QString::fromUtf8("lBluetoothState"));

        horizontalLayout->addWidget(lBluetoothState);

        lTemp = new QLabel(centralWidget);
        lTemp->setObjectName(QString::fromUtf8("lTemp"));
        QFont font2;
        font2.setPointSize(14);
        lTemp->setFont(font2);
        lTemp->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lTemp->setMargin(5);
        lTemp->setIndent(0);

        horizontalLayout->addWidget(lTemp);

        lTime = new QLabel(centralWidget);
        lTime->setObjectName(QString::fromUtf8("lTime"));
        QFont font3;
        font3.setPointSize(20);
        font3.setBold(true);
        font3.setWeight(75);
        lTime->setFont(font3);
        lTime->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lTime);


        horizontalLayout_4->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(horizontalLayout_4);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        pageNavit = new QWidget();
        pageNavit->setObjectName(QString::fromUtf8("pageNavit"));
        verticalLayout_3 = new QVBoxLayout(pageNavit);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        stackedWidget->addWidget(pageNavit);
        pageDials = new QWidget();
        pageDials->setObjectName(QString::fromUtf8("pageDials"));
        stackedWidget->addWidget(pageDials);

        verticalLayout_2->addWidget(stackedWidget);

        WndMain->setCentralWidget(centralWidget);

        retranslateUi(WndMain);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WndMain);
    } // setupUi

    void retranslateUi(QMainWindow *WndMain)
    {
        WndMain->setWindowTitle(QApplication::translate("WndMain", "WndMain", 0, QApplication::UnicodeUTF8));
        lRadioText->setText(QApplication::translate("WndMain", "TextLabel", 0, QApplication::UnicodeUTF8));
        lRadioIcons->setText(QApplication::translate("WndMain", "Auto", 0, QApplication::UnicodeUTF8));
        lSourceIcon->setText(QApplication::translate("WndMain", "AA", 0, QApplication::UnicodeUTF8));
        lBluetoothState->setText(QApplication::translate("WndMain", "BB", 0, QApplication::UnicodeUTF8));
        lTemp->setText(QApplication::translate("WndMain", "6C", 0, QApplication::UnicodeUTF8));
        lTime->setText(QApplication::translate("WndMain", "00:00", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WndMain: public Ui_WndMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WNDMAIN_H
