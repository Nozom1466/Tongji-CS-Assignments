#pragma once
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include <QGraphicsView>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include <QWheelEvent>

#include<qlayout.h>
#include<qlabel.h>
#include<qmovie.h>
#include<qtimer.h>
#include <qmessagebox.h>
#include<qtextbrowser.h>
#include<qtablewidget.h>
#include<qpixmap.h>
#include <qdebug.h>
#include <qmath.h>
#include <qscrollbar.h>
#include<qfile.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include <cmath>

#include "ui_Display.h"



//lr1相关文件地址
#define Table_FILE  "results/ACTION_GOTO_Table.csv" //输出表地址
#define REDUCTION_PROCESS_FILE "results/Reduction_Process.txt" //输出归约地址
#define TREE_DOT_FILE  "results/Parse_Tree.dot"   //画图dot文件地址
#define TREE_PNG_FILE   "picture/Parse_Tree.png" //语法树图片地址
#define PARSE_ERROR_FILE "error_log/Parse_Error.txt"
#define DFA_DOT_FILE   "results/Parse_DFA.dot"
#define PARSE_DFA_PNG_FILE "picture/Parse_DFA.png"


//词法分析相关文件地址
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"
#define DFA_PNG_FILE    "picture/DFA.png"


//文法读入相关文件地址
#define GRAMMAR_ERROR_FILE "error_log/Grammar_Error_List.txt"
#define EXTENDED_GRAMMAR_FILE "results/Extended_Grammar.txt"
#define FIRST_SET_FILE "results/First_Set.txt"

#define  QUATERNION_FILE  "results/Quaternion_List.txt"


//目标代码生成相关地址
#define  VAR_TABLE_FILE  "results/Var_Table.txt"
#define FUNC_TABLE_FILE "results/Func_Table.txt"
#define OBJECT_CODE_FILE "results/Object_Code.txt"

//个人修改页面
class Show : public QMainWindow
{
    Q_OBJECT

public:
    Show(QWidget* parent = nullptr);
    ~Show();
    void showToken();//展示词元
    void showDFA();//展示dfa
    void showTable();//展示Action goto表
    void showTree(); //展示语法分析树
    void showProcess(); //展示归约过程
    void showParseDFA();
    void showQuaternion();
    void showFunc();
    void showVar();
    void showObjectCode();
    //void showloading();
private:
    Ui::ShowWindow* ui;                       // 主页面
    QVBoxLayout* layout;
};


//绘图单元类，设置图像画面的相关操作
class myQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit myQGraphicsView(QWidget* parent = nullptr);
    void wheelEvent(QWheelEvent* event);                  // 重写鼠标缩放
    void expand();                                        // 放大
    void shrink();                                        // 缩小
    void refresh();                                       // 刷新

private:
    void zoom(double);                                    // 尺寸变化

signals:

};


