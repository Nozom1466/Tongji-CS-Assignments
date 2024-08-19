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

// �ļ�
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"

// �ź���
#define SUCCESS 1
#define CODE_FILE_OPEN_ERROR 2
#define LEXICAL_ANALYSIS_ERROR 3

// �����С
#define  KEY_WORD_SIZE  24
#define  MONOCULAR_OPERATOR_SIZE  13
#define  BINOCULAR_OPERATOR_SIZE  14
#define  BUFFER_SIZE 300

//�ս��
enum {
    my_Key_Word = 1,
    my_SignWord,
    my_Integer,
    my_FloatPoint,
    my_MonocularOperator,    // ��Ŀ�����
    my_BinocularOperator,    // ˫Ŀ�����
    my_Delimiter,            // ���
    my_WrongWord,            // ����
    my_Blank,                // �ո�
    my_Separator,            // �ָ���
    my_BracketsLeft,         // ������
    my_BracketsRight,        // ������
    my_BracketsLeftBig,      // �������
    my_BracketsRightBig,     // �Ҵ�����
    my_End,                  // ������
    my_Str,                  // �ַ���
    my_Char,                 // �ַ�
    my_Brackets_Left_Square, // ������
    my_Brackets_Right_Square,// �ҷ�����
    my_Point_Arrow,          // ��ͷ
    my_Region,               // ���
    my_Region_Destroyer,     // ������
    my_Point,                // ���
    my_Colon,                // ð��
    my_Question_Mark         // �ʺ�
};

//�ؼ���
const string Key_Word[KEY_WORD_SIZE] = { 
    "break","case","char","class","continue","do","default","double","define", "else","float","for","if",
    "int","include","long","return","switch","typedef","void","unsigned","while","iostream" 
};

// ���
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

//��Ŀ�����
const string Monocular_Operator[MONOCULAR_OPERATOR_SIZE] = {
    "+","-","*","/","!","%","~","&","|","^", "=" ,">","<"
};

//˫Ŀ�����
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
    void deleComments(int line_count);  // ɾ��ע��
    void deleSpaces(int line_count);    // ɾ���ո�

    bool DFA(int line_count);//״̬�� ���ýṹ���buffer���� ���ص�int ת����kindJudge����
    bool tokenJudge(char* str, int line_count);//kindJudge���� �жϴ���� str ���� �������ת���� print����������

    token lexer::elemGenerate(int kind, char* str, int line_count);

    lexer() {};
    ~lexer() {};

};

int toState(char c);// �жϵ�ǰDFAҪת���״̬
bool spaceRemovable(char c);//�жϿո��ܷ�ɾ��


//���������ж�
int isDelimiter(char c);            //���
int isDelimiter(string c);          //���(���أ��ж��ַ�����)
int isSignWord(string str);         // ��ʶ�� 
int isKeyWord(string str);          // ������  �ؼ��� 
int isBinocularOperator(string str);//�ж�˫Ŀ�����
int isMonocularOperator(string str);//�жϵ�Ŀ�����

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

int isStr(string str);              //�ַ���
int isChar(string str);             //�ִ����ַ�
int isInt(string str);              //����
int isFloat(string str);            //float �� +-xx.xx e +-xx.xx
int hasDot(string str);             //float�� +-xx.xx
int isBlank(string str);            //�жϿո�


