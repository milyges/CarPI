/********************************************************************************
** Form generated from reading UI file 'wndmenu.ui'
**
** Created: Wed Jun 25 18:57:30 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WNDMENU_H
#define UI_WNDMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WndMenu
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QDialog *WndMenu)
    {
        if (WndMenu->objectName().isEmpty())
            WndMenu->setObjectName(QString::fromUtf8("WndMenu"));
        WndMenu->resize(309, 119);
        WndMenu->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 14pt \"Monospace\";\n"
"}\n"
"\n"
"QLabel[checked=\"true\"] {	\n"
"	background-color: #ff8c00;\n"
"	color: #ddd;\n"
"    border-radius: 4px;\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(WndMenu);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(WndMenu);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 275, 101));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(WndMenu);

        QMetaObject::connectSlotsByName(WndMenu);
    } // setupUi

    void retranslateUi(QDialog *WndMenu)
    {
        WndMenu->setWindowTitle(QApplication::translate("WndMenu", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WndMenu: public Ui_WndMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WNDMENU_H
