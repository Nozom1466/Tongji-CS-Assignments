#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("utf-8") 
#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<cstring>
#include<string>
using namespace std;

// 文件
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"

// 信号量
#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3

// 数组大小
#define  KEY_WORD_SIZE  24
#define  MONOCULAR_OPERATOR_SIZE  13
#define  BINOCULAR_OPERATOR_SIZE  14
#define  BUFFER_SIZE 300

//终结符
enum {
    my_Key_Word = 1,
    my_SignWord,
    my_Integer,
    my_FloatPoint,
    my_MonocularOperator,    // 单目运算符
    my_BinocularOperator,    // 双目运算符
    my_Delimiter,            // 界符
    my_WrongWord,            // 错误
    my_Blank,                // 空格
    my_Separator,            // 分隔符
    my_BracketsLeft,         // 左括号
    my_BracketsRight,        // 右括号
    my_BracketsLeftBig,      // 左大括号
    my_BracketsRightBig,     // 右大括号
    my_End,                  // 结束符
    my_Str,                  // 字符串
    my_Char,                 // 字符
    my_Brackets_Left_Square, // 左方括号
    my_Brackets_Right_Square,// 右方括号
    my_Point_Arrow,          // 箭头
    my_Region,               // 域符
    my_Region_Destroyer,     // 析构符
    my_Point,                // 点符
    my_Colon,                // 冒号
    my_Question_Mark         // 问号
};

//关键字
const string Key_Word[KEY_WORD_SIZE] = { 
    "break","case","char","class","continue","do","default","double","define", "else","float","for","if",
    "int","include","long","return","switch","typedef","void","unsigned","while","iostream" 
};

// 界符
enum {
    Delimiter = ';',
    Separator = ',',
    Brackets_Left = '(',
    Brackets_Right = ')',
    Brackets_Left_Big = '{',
    Brackets_Right_Big = '}',
    End = '#',
    Point = '.',
    Brackets_Left_Square = '[',
    Brackets_Right_Square = ']',
    Colon = ':',
    Question_Mark = '?'
};
const char Point_Arrow[] = "->";
const char Region[] = "::";
const char Region_Destroyer[] = "::~";

//单目运算符
const string Monocular_Operator[MONOCULAR_OPERATOR_SIZE] = {
    "+","-","*","/","!","%","~","&","|","^", "=" ,">","<"
};

//双目运算符
const string Binocular_Operator[BINOCULAR_OPERATOR_SIZE] = {
    "++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/=","<<",">>"
};

class GrammaAnalysis;

struct token {
    string word;
    int type;
    int line;
    token(string word, int type, int line);
};


class Buffer {
public:
    char* buffer;
    int count;
    Buffer() {
        count = 0;
        buffer = new char[BUFFER_SIZE];
    }
    ~Buffer() {
        delete buffer;
    }
};


class myCompiler;
class lexer
{
    friend myCompiler;
    friend GrammaAnalysis;

public:
    Buffer double_buffer[2];
    Buffer final_buffer;

    int current_buffer = 0;
    int comment_flag = 0;

    FILE* fcode;
    FILE* ftoken;
    FILE* ferror;
    int error_line;
    vector<token>token_vec;
    int lexicalAnalysis(string filename);
    void deleComments(int line_count);  // 删除注释
    void deleSpaces(int line_count);    // 删除空格

    bool DFA(int line_count);//状态机 调用结构体的buffer变量 返回的int 转交给kindJudge函数
    bool tokenJudge(char* str, int line_count);//kindJudge函数 判断传入的 str 类型 并将结果转交给 print函数完成输出

    token lexer::elemGenerate(int kind, char* str, int line_count);

    lexer() {};
    ~lexer() {};

};

int toState(char c);// 判断当前DFA要转向的状态
bool spaceRemovable(char c);//判断空格能否删除


//符号类型判断
int isDelimiter(char c);            //界符
int isDelimiter(string c);          //界符(重载，判断字符数组)
int isSignWord(string str);         // 标识符 
int isKeyWord(string str);          // 保留字  关键字 
int isBinocularOperator(string str);//判断双目运算符
int isMonocularOperator(string str);//判断单目运算符

int isSeparator(char c);            //,
int isBracketsLeft(char c);         //(
int isBracketsRight(char c);        //)
int isBracketsLeftBig(char c);      //{
int isBracketsRightBig(char c);     //}
int isPoint(char c);                //.
int isBracketsLeftSquare(char c);   //[
int isBracketsRightSquare(char c);  //]
int isPointArrow(string str);       //->
int isRegion(string str);           //::
int isRegionDestroyer(string str);  //::~
int isColon(char c);                //:
int isEnd(char c);                  //#
int isQuestion_Mark(char c);        //?

int isStr(string str);              //字符串
int isChar(string str);             //字串是字符
int isInt(string str);              //整型
int isFloat(string str);            //float 型 +-xx.xx e +-xx.xx
int hasDot(string str);             //float型 +-xx.xx
int isBlank(string str);            //判断空格


