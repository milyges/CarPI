/********************************************************************************
** Form generated from reading UI file 'wndvolume.ui'
**
** Created: Wed Jun 25 21:39:38 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WNDVOLUME_H
#define UI_WNDVOLUME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_WndVolume
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QProgressBar *pbVolume;

    void setupUi(QDialog *WndVolume)
    {
        if (WndVolume->objectName().isEmpty())
            WndVolume->setObjectName(QString::fromUtf8("WndVolume"));
        WndVolume->resize(346, 50);
        horizontalLayout = new QHBoxLayout(WndVolume);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(WndVolume);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/speaker.png")));

        horizontalLayout->addWidget(label);

        pbVolume = new QProgressBar(WndVolume);
        pbVolume->setObjectName(QString::fromUtf8("pbVolume"));
        pbVolume->setMaximum(32);

        horizontalLayout->addWidget(pbVolume);


        retranslateUi(WndVolume);

        QMetaObject::connectSlotsByName(WndVolume);
    } // setupUi

    void retranslateUi(QDialog *WndVolume)
    {
        WndVolume->setWindowTitle(QApplication::translate("WndVolume", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        pbVolume->setFormat(QApplication::translate("WndVolume", "%v", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WndVolume: public Ui_WndVolume {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WNDVOLUME_H
