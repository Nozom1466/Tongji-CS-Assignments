/********************************************************************************
** Form generated from reading UI file 'CCompiler.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCOMPILER_H
#define UI_CCOMPILER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_myCompilerClass
{
public:
    QAction *action_Vartable;
    QAction *action_FuncTable;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QGroupBox *groupBox_3;
    QPushButton *action_DFA;
    QPushButton *action_Token;
    QGroupBox *groupBox_4;
    QPushButton *action_Process;
    QPushButton *action_Table;
    QPushButton *actionLR_1_DFA;
    QPushButton *action_Tree;
    QGroupBox *groupBox_5;
    QPushButton *action_Quaternion;
    QPushButton *action_Func;
    QPushButton *action_Var;
    QGroupBox *groupBox_2;
    QTextEdit *textEdit;
    QGroupBox *groupBox_6;
    QPushButton *action_Code;
    QPushButton *action_Grammar;
    QGroupBox *groupBox_7;
    QPushButton *actionMIPS_code;
    QPushButton *action_Run;
    QGroupBox *groupBox_8;
    QTextBrowser *textBrowser;
    QPushButton *action_Restart;

    void setupUi(QMainWindow *myCompilerClass)
    {
        if (myCompilerClass->objectName().isEmpty())
            myCompilerClass->setObjectName(QString::fromUtf8("myCompilerClass"));
        myCompilerClass->resize(748, 507);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/myCompiler/picture/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        myCompilerClass->setWindowIcon(icon);
        action_Vartable = new QAction(myCompilerClass);
        action_Vartable->setObjectName(QString::fromUtf8("action_Vartable"));
        action_FuncTable = new QAction(myCompilerClass);
        action_FuncTable->setObjectName(QString::fromUtf8("action_FuncTable"));
        centralWidget = new QWidget(myCompilerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"  background-color: #19232D;\n"
"  border: 0px solid #455364;\n"
"  padding: 0px;\n"
"  color: #E0E1E3;\n"
"  selection-background-color: #346792;\n"
"  selection-color: #E0E1E3;\n"
"}\n"
"\n"
"QWidget:disabled {\n"
"  background-color: #19232D;\n"
"  color: #9DA9B5;\n"
"  selection-background-color: #26486B;\n"
"  selection-color: #9DA9B5;\n"
"}\n"
"\n"
"QWidget::item:selected {\n"
"  background-color: #346792;\n"
"}\n"
"\n"
"QWidget::item:hover:!selected {\n"
"  background-color: #1A72BB;\n"
"}"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, -10, 751, 521));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"color: rgb(170, 170, 255);\n"
"background-color: rgb(211, 249, 216);\n"
"  font-weight: bold;\n"
"  border: 1px solid #455364;\n"
"  border-radius: 4px;\n"
"  padding: 2px;\n"
"  margin-top: 6px;\n"
"  margin-bottom: 4px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"  rgb(255, 255, 255)\n"
"  subcontrol-origin: margin;\n"
"  subcontrol-position: top left;\n"
"  left: 4px;\n"
"  padding-left: 2px;\n"
"  padding-right: 4px;\n"
"  padding-top: -4px;\n"
"}\n"
"\n"
"QGroupBox::indicator {\n"
"  margin-left: 2px;\n"
"  margin-top: 2px;\n"
"  padding: 0;\n"
"  height: 14px;\n"
"  width: 14px;\n"
"}\n"
""));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 110, 191, 71));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        action_DFA = new QPushButton(groupBox_3);
        action_DFA->setObjectName(QString::fromUtf8("action_DFA"));
        action_DFA->setGeometry(QRect(10, 20, 81, 31));
        action_DFA->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Token = new QPushButton(groupBox_3);
        action_Token->setObjectName(QString::fromUtf8("action_Token"));
        action_Token->setGeometry(QRect(100, 20, 81, 31));
        action_Token->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(20, 190, 191, 111));
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        action_Process = new QPushButton(groupBox_4);
        action_Process->setObjectName(QString::fromUtf8("action_Process"));
        action_Process->setGeometry(QRect(100, 20, 81, 31));
        action_Process->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Table = new QPushButton(groupBox_4);
        action_Table->setObjectName(QString::fromUtf8("action_Table"));
        action_Table->setGeometry(QRect(10, 20, 81, 31));
        action_Table->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        actionLR_1_DFA = new QPushButton(groupBox_4);
        actionLR_1_DFA->setObjectName(QString::fromUtf8("actionLR_1_DFA"));
        actionLR_1_DFA->setGeometry(QRect(10, 60, 81, 31));
        actionLR_1_DFA->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Tree = new QPushButton(groupBox_4);
        action_Tree->setObjectName(QString::fromUtf8("action_Tree"));
        action_Tree->setGeometry(QRect(100, 60, 81, 31));
        action_Tree->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(20, 310, 191, 111));
        groupBox_5->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        action_Quaternion = new QPushButton(groupBox_5);
        action_Quaternion->setObjectName(QString::fromUtf8("action_Quaternion"));
        action_Quaternion->setGeometry(QRect(10, 20, 171, 31));
        action_Quaternion->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Func = new QPushButton(groupBox_5);
        action_Func->setObjectName(QString::fromUtf8("action_Func"));
        action_Func->setGeometry(QRect(10, 60, 81, 31));
        action_Func->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Var = new QPushButton(groupBox_5);
        action_Var->setObjectName(QString::fromUtf8("action_Var"));
        action_Var->setGeometry(QRect(100, 60, 81, 31));
        action_Var->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(220, 30, 251, 471));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        textEdit = new QTextEdit(groupBox_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 20, 231, 431));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        textEdit->setFont(font);
        textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"  background-color: #FFFFFF;\n"
"  color: #19232D;\n"
"  border-radius: 4px;\n"
"  border: 1px solid #C9CDD0;\n"
"}\n"
"\n"
"QTextEdit:focus {\n"
"  border: 1px solid #73C7FF;\n"
"}\n"
"\n"
"QTextEdit:selected {\n"
"  background: #9FCBFF;\n"
"  color: #C9CDD0;\n"
"}"));
        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(20, 30, 191, 71));
        groupBox_6->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        action_Code = new QPushButton(groupBox_6);
        action_Code->setObjectName(QString::fromUtf8("action_Code"));
        action_Code->setGeometry(QRect(10, 20, 81, 31));
        action_Code->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"	cursor: pointer;\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Grammar = new QPushButton(groupBox_6);
        action_Grammar->setObjectName(QString::fromUtf8("action_Grammar"));
        action_Grammar->setGeometry(QRect(100, 20, 81, 31));
        action_Grammar->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        groupBox_7 = new QGroupBox(groupBox);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(20, 430, 191, 71));
        groupBox_7->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        actionMIPS_code = new QPushButton(groupBox_7);
        actionMIPS_code->setObjectName(QString::fromUtf8("actionMIPS_code"));
        actionMIPS_code->setGeometry(QRect(10, 20, 171, 31));
        actionMIPS_code->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        action_Run = new QPushButton(groupBox);
        action_Run->setObjectName(QString::fromUtf8("action_Run"));
        action_Run->setGeometry(QRect(620, 460, 111, 31));
        action_Run->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        groupBox_8 = new QGroupBox(groupBox);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(480, 30, 251, 421));
        groupBox_8->setStyleSheet(QString::fromUtf8("QGroupBox \n"
"{\n"
"	border-color: rgb(55, 178, 77);\n"
"	color: rgb(43, 138, 62);\n"
"	font-size: 10px;\n"
"}\n"
"\n"
"QGroupBox::title \n"
"{\n"
"    left: 5px;\n"
"    top: -5px;\n"
"}"));
        textBrowser = new QTextBrowser(groupBox_8);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 20, 231, 381));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        textBrowser->setFont(font1);
        textBrowser->setStyleSheet(QString::fromUtf8("QTextBrowser {\n"
"	background-color: rgb(255, 255, 255);\n"
"  \n"
"  color: #19232D;\n"
"  border-radius: 4px;\n"
"  border: 1px solid #C9CDD0;\n"
"}"));
        action_Restart = new QPushButton(groupBox);
        action_Restart->setObjectName(QString::fromUtf8("action_Restart"));
        action_Restart->setGeometry(QRect(480, 460, 111, 31));
        action_Restart->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(81, 207, 102);\n"
"	padding: 3px 6px 3px 6px;\n"
"	font-weight: 700;\n"
"	font-size: 10px;\n"
"	border-radius: 8px;\n"
"	color: rgb(235, 251, 238);\n"
"}\n"
"\n"
"QPushButton:hover\n"
"{\n"
"    background-color:rgb(140, 233, 154);\n"
"    cursor: pointer;\n"
"}"));
        myCompilerClass->setCentralWidget(centralWidget);

        retranslateUi(myCompilerClass);

        QMetaObject::connectSlotsByName(myCompilerClass);
    } // setupUi

    void retranslateUi(QMainWindow *myCompilerClass)
    {
        myCompilerClass->setWindowTitle(QApplication::translate("myCompilerClass", "LR(1) \347\261\273 C \346\226\207\346\263\225\347\274\226\350\257\221\345\231\250", nullptr));
        action_Vartable->setText(QApplication::translate("myCompilerClass", "\345\217\230\351\207\217\350\241\250", nullptr));
        action_FuncTable->setText(QApplication::translate("myCompilerClass", "\345\207\275\346\225\260\350\241\250", nullptr));
        groupBox->setTitle(QString());
        groupBox_3->setTitle(QApplication::translate("myCompilerClass", "\350\257\215\346\263\225\345\210\206\346\236\220", nullptr));
        action_DFA->setText(QApplication::translate("myCompilerClass", "DFA\345\261\225\347\244\272", nullptr));
        action_Token->setText(QApplication::translate("myCompilerClass", "Token\345\261\225\347\244\272", nullptr));
        groupBox_4->setTitle(QApplication::translate("myCompilerClass", "\350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        action_Process->setText(QApplication::translate("myCompilerClass", "\350\247\204\347\272\246\350\277\207\347\250\213", nullptr));
        action_Table->setText(QApplication::translate("myCompilerClass", "Action/Goto\350\241\250", nullptr));
        actionLR_1_DFA->setText(QApplication::translate("myCompilerClass", "\351\241\271\347\233\256\351\233\206DFA", nullptr));
        action_Tree->setText(QApplication::translate("myCompilerClass", "\350\257\255\346\263\225\346\240\221", nullptr));
        groupBox_5->setTitle(QApplication::translate("myCompilerClass", "\350\257\255\344\271\211\345\210\206\346\236\220", nullptr));
        action_Quaternion->setText(QApplication::translate("myCompilerClass", "\345\233\233\345\205\203\345\274\217\347\224\237\346\210\220", nullptr));
        action_Func->setText(QApplication::translate("myCompilerClass", "\345\207\275\346\225\260\350\241\250", nullptr));
        action_Var->setText(QApplication::translate("myCompilerClass", "\345\217\230\351\207\217\350\241\250", nullptr));
        groupBox_2->setTitle(QApplication::translate("myCompilerClass", "\346\272\220\344\273\243\347\240\201", nullptr));
        groupBox_6->setTitle(QApplication::translate("myCompilerClass", "\346\226\207\344\273\266\345\257\274\345\205\245", nullptr));
        action_Code->setText(QApplication::translate("myCompilerClass", "\345\257\274\345\205\245\346\272\220\347\240\201", nullptr));
        action_Grammar->setText(QApplication::translate("myCompilerClass", "\345\257\274\345\205\245\346\226\207\346\263\225", nullptr));
        groupBox_7->setTitle(QApplication::translate("myCompilerClass", "\347\233\256\346\240\207\344\273\243\347\240\201\347\224\237\346\210\220", nullptr));
        actionMIPS_code->setText(QApplication::translate("myCompilerClass", "\346\261\207\347\274\226\344\273\243\347\240\201\347\224\237\346\210\220", nullptr));
        action_Run->setText(QApplication::translate("myCompilerClass", "\345\274\200\345\247\213\347\274\226\350\257\221", nullptr));
        groupBox_8->setTitle(QApplication::translate("myCompilerClass", "\347\273\210\347\253\257", nullptr));
        action_Restart->setText(QApplication::translate("myCompilerClass", "\351\207\215\347\275\256\350\276\223\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class myCompilerClass: public Ui_myCompilerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCOMPILER_H
