/********************************************************************************
** Form generated from reading UI file 'radioModule.ui'
**
** Created: Sun Jun 29 13:43:15 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADIOMODULE_H
#define UI_RADIOMODULE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "scrolledlabel.h"

QT_BEGIN_NAMESPACE

class Ui_RadioModule
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *lbIcon;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    ScrolledLabel *lbTitle;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lbRadioIcons;
    QLabel *lbExtraInfo;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *RadioModule)
    {
        if (RadioModule->objectName().isEmpty())
            RadioModule->setObjectName(QString::fromUtf8("RadioModule"));
        RadioModule->resize(439, 280);
        verticalLayout = new QVBoxLayout(RadioModule);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 47, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lbIcon = new QLabel(RadioModule);
        lbIcon->setObjectName(QString::fromUtf8("lbIcon"));

        horizontalLayout->addWidget(lbIcon);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 47, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        lbTitle = new ScrolledLabel(RadioModule);
        lbTitle->setObjectName(QString::fromUtf8("lbTitle"));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        font.setPointSize(22);
        lbTitle->setFont(font);
        lbTitle->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbTitle);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lbRadioIcons = new QLabel(RadioModule);
        lbRadioIcons->setObjectName(QString::fromUtf8("lbRadioIcons"));
        lbRadioIcons->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_2->addWidget(lbRadioIcons);


        verticalLayout->addLayout(horizontalLayout_2);

        lbExtraInfo = new QLabel(RadioModule);
        lbExtraInfo->setObjectName(QString::fromUtf8("lbExtraInfo"));

        verticalLayout->addWidget(lbExtraInfo);

        verticalSpacer_3 = new QSpacerItem(20, 47, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(RadioModule);

        QMetaObject::connectSlotsByName(RadioModule);
    } // setupUi

    void retranslateUi(QWidget *RadioModule)
    {
        RadioModule->setWindowTitle(QApplication::translate("RadioModule", "Form", 0, QApplication::UnicodeUTF8));
        lbIcon->setText(QApplication::translate("RadioModule", "SourceIcon", 0, QApplication::UnicodeUTF8));
        lbTitle->setText(QApplication::translate("RadioModule", "RMF FM", 0, QApplication::UnicodeUTF8));
        lbRadioIcons->setText(QApplication::translate("RadioModule", "TRAFFIC NEWS AF-RDS", 0, QApplication::UnicodeUTF8));
        lbExtraInfo->setText(QApplication::translate("RadioModule", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RadioModule: public Ui_RadioModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADIOMODULE_H
