/********************************************************************************
** Form generated from reading UI file 'Display.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowWindow
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *ShowWindow)
    {
        if (ShowWindow->objectName().isEmpty())
            ShowWindow->setObjectName(QString::fromUtf8("ShowWindow"));
        ShowWindow->resize(800, 600);
        centralwidget = new QWidget(ShowWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ShowWindow->setCentralWidget(centralwidget);

        retranslateUi(ShowWindow);

        QMetaObject::connectSlotsByName(ShowWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ShowWindow)
    {
        ShowWindow->setWindowTitle(QApplication::translate("ShowWindow", "\345\261\225\347\244\272\347\225\214\351\235\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowWindow: public Ui_ShowWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
