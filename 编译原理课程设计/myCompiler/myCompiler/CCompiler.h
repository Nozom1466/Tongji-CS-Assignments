#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include <QWheelEvent>

#include<qevent.h>
#include<qcolor.h>
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
#include<qdebug.h>
#include<qmessagebox.h>
#include<qfiledialog.h>
#include <string>
#include<fstream>
#include<cstdio>
#include<sstream>
#include<qfile.h>
#include<cstdlib>
#include<stdio.h>
#include<Windows.h>
#include<ctime>
#include<iostream>
#include<fstream>
#include<sstream>
#include <cmath>

#include "Display.h"
#include "ui_CCompiler.h"
#include "ObjectCodeGenerator.h"

#define OBJECT_CODE_FILE "results/Object_Code.txt"
#define DEFAULT_GRAMMAR "Grammar.txt"
#define CODE_FILE "Input_Code.txt"
#define FAILED 0 
#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3
#define ACTION_ERROR 2
#define GOTO_ERROR  3
#define REDUCTION_PROCESS_FILE_OPEN_ERROR 4
#define PARSE_ERROR_FILE_OPEN_ERROR 5
#define TREE_DOT_FILE_OPEN_ERROR 6
#define GRAMMAR_FILE_OPEN_ERROR 2
#define GRAMMAR_ERROR 3
#define GRAMMAR_ERROR_FILE_OPEN_ERROR 4

#define MSG_ERROR 0
#define MSG_SUCCESS 1
#define MSG_WARNING 2
#define MSG_NORMAL 3

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


#pragma execution_character_set("utf-8")


class myCompiler : public QMainWindow
{
    Q_OBJECT

public:
    myCompiler(QWidget* parent = nullptr);
    ~myCompiler();
private:
    bool errorFlag;
    int errorLine;
    bool loadGrammarFlag;
    bool grammarFlag;
    bool lexerFlag;
    bool compileFlag;
    bool treeFlag;
    bool dfaFlag;

    // 展示
    Ui::myCompilerClass* ui; 
    Show* pageShow;

    // 文件
    QString codeFile;
    QString grammarFile;
    
    // 各部分
    GrammaAnalysis* my_lr1Grammar;
    lexer* my_lexer;
    Optimizer* my_optimizer;
    ObjectCodeGenerator* my_objector;

    int readGrammar();
    void loadSourceCode();
    void loadGrammar();
    void restart();
    void compile();
    

    // 提示
    void showErrorLine(int n, QColor color);
    void printTime(int ms);
    void consoleLog(string msg, int msg_type=MSG_NORMAL);
};
