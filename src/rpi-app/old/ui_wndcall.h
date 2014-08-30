/********************************************************************************
** Form generated from reading UI file 'wndcall.ui'
**
** Created: Wed Jun 25 18:57:30 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WNDCALL_H
#define UI_WNDCALL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WndCall
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lModeText;
    QLabel *lCallDuration;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lStartIcon;
    QLabel *lStartText;
    QSpacerItem *horizontalSpacer;
    QLabel *lStopIcon;
    QLabel *lStopText;

    void setupUi(QDialog *WndCall)
    {
        if (WndCall->objectName().isEmpty())
            WndCall->setObjectName(QString::fromUtf8("WndCall"));
        WndCall->resize(326, 84);
        verticalLayout = new QVBoxLayout(WndCall);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lModeText = new QLabel(WndCall);
        lModeText->setObjectName(QString::fromUtf8("lModeText"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lModeText->sizePolicy().hasHeightForWidth());
        lModeText->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(14);
        lModeText->setFont(font);
        lModeText->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lModeText);

        lCallDuration = new QLabel(WndCall);
        lCallDuration->setObjectName(QString::fromUtf8("lCallDuration"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lCallDuration->sizePolicy().hasHeightForWidth());
        lCallDuration->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setPointSize(12);
        lCallDuration->setFont(font1);
        lCallDuration->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);

        horizontalLayout->addWidget(lCallDuration);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lStartIcon = new QLabel(WndCall);
        lStartIcon->setObjectName(QString::fromUtf8("lStartIcon"));
        lStartIcon->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/call-start.png")));

        horizontalLayout_2->addWidget(lStartIcon);

        lStartText = new QLabel(WndCall);
        lStartText->setObjectName(QString::fromUtf8("lStartText"));
        lStartText->setFont(font1);

        horizontalLayout_2->addWidget(lStartText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lStopIcon = new QLabel(WndCall);
        lStopIcon->setObjectName(QString::fromUtf8("lStopIcon"));
        lStopIcon->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/call-stop.png")));

        horizontalLayout_2->addWidget(lStopIcon);

        lStopText = new QLabel(WndCall);
        lStopText->setObjectName(QString::fromUtf8("lStopText"));
        lStopText->setFont(font1);

        horizontalLayout_2->addWidget(lStopText);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(WndCall);

        QMetaObject::connectSlotsByName(WndCall);
    } // setupUi

    void retranslateUi(QDialog *WndCall)
    {
        WndCall->setWindowTitle(QApplication::translate("WndCall", "Dialog", 0, QApplication::UnicodeUTF8));
        lModeText->setText(QApplication::translate("WndCall", "Po\305\202\304\205czenie przychodz\304\205ce", 0, QApplication::UnicodeUTF8));
        lCallDuration->setText(QApplication::translate("WndCall", "00:00", 0, QApplication::UnicodeUTF8));
        lStartIcon->setText(QString());
        lStartText->setText(QApplication::translate("WndCall", "Volume+", 0, QApplication::UnicodeUTF8));
        lStopIcon->setText(QString());
        lStopText->setText(QApplication::translate("WndCall", "Volume-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WndCall: public Ui_WndCall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WNDCALL_H
