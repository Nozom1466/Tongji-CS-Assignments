#pragma once
#pragma execution_character_set("utf-8") 
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<map>
#include <sstream>
#include <stack>
#include <iomanip>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QMainWindow>
#include <windows.h>

#include "LexicalAnalysis.h"


using namespace std;


//文法分析错误
#define GRAMMAR_FILE_OPEN_ERROR 2
#define SUCCESS 1
#define GRAMMAR_ERROR 3
#define GRAMMAR_ERROR_FILE_OPEN_ERROR 4

//语法语义分析过程错误
#define ACTION_ERROR 2
#define GOTO_ERROR  3
#define REDUCTION_PROCESS_FILE_OPEN_ERROR 4
#define PARSE_ERROR_FILE_OPEN_ERROR 5
#define TREE_DOT_FILE_OPEN_ERROR 6
#define DFA_DOT_FILE_OPEN_ERROR 7
#define SEMANTIC_ERROR 8


//相关文件地址
#define QUATERNION_FILE  "results/Quaternion_List.txt"
#define SEMANTIC_ERROR_FILE "error_log/Semantic_Error_List.txt"
#define VAR_TABLE_FILE  "results/Var_Table.txt"
#define FUNC_TABLE_FILE "results/Func_Table.txt"
#define GRAMMAR_ERROR_FILE "error_log/Grammar_Error_List.txt"
#define Table_FILE  "results/ACTION_GOTO_Table.csv"
#define REDUCTION_PROCESS_FILE "results/Reduction_Process.txt"
#define TREE_DOT_FILE  "results/Parse_Tree.dot"
#define TREE_PNG_FILE   "picture/Parse_Tree.png"
#define PARSE_ERROR_FILE "error_log/Parse_Error.txt"
#define DFA_DOT_FILE   "results/Parse_DFA.dot"
#define SEMANTIC_ERROR_FILE "error_log/Semantic_Error_List.txt"


//符号类型
enum SignType {
    StartTerminator, //开始符
    Terminator,     //终结符
    NonTerminator,   //非终结符
    Empty             //空
};
//语法动作
enum ActionType {
    Resolution,    //规约
    MoveIn,         //移进
    Acc             //接受态
};
//命名表类型
enum NameTableKind {
    Varible,     //变量
    ConstV,      //常变量
    Array,       //数组
    Proc         //过程
};
//命名表的数据类型
enum NameTableType {
    INT1,            //int
    FLOAT1,          //float
    VOID1            //void
};
//语义错误类型
enum SemanticError {
    None,          //无错误
    NoFuncName,     //函数未定义
    FuncParLenError,  //函数参数长度错误
    FuncReturnErr,   //函数返回错误
    NoMainName,      //没有main函数
    RedefineVar,     //重定义
    NoVar,           //未定义变量
    ArrayIndexError //数组索引错误
};


//变量元素
struct Var {
    string name;//名字标识符
    NameTableType type;//名字的类型
    NameTableKind valtype;//变量类型
    int ival;        //int变量值
    float fval;       //float变量值
    int ProcNo;//过程编号
    vector<int> dims;//维度数组，从后往前
};

struct Par {
    NameTableType type;//参数类型
    string name;//参数名
    vector<int> dims;
};



//函数元素
struct Func {
    string name;//名字标识符
    NameTableType type;//函数的类型
    int adr;//过程的地址
    vector<Par> param;//参数
};

//数组元素
struct Array_ {
    string name;//名字标识符
    int len;//数组元素个数
};

//四元式
struct Code {
    string op;
    string op1;
    string op2;
    string linkres;
};

//语法树结点
struct TreeNode {
    string str;//字符内容
    string StrType;//内部的表示
    SignType type;//类型
};

struct Sign
{
    string str;//字符内容
    SignType type;//类型
    Sign();
    Sign(const Sign& s);
    Sign(string MyStr, SignType MyType);
    Sign& operator=(const Sign& s) {
        if (this == &s) {
            return *this;
        }
        this->str = s.str;
        this->type = s.type;
        return *this;
    }
    //重载等于号 方便判断
    bool operator==(const Sign& s) {
        return (s.str == this->str && s.type == this->type);
    }
    //重载小于号 方便放入map set等数据结构
    bool operator<(const Sign& s)const {
        return (this->str < s.str);
    }
};

struct ProduceForms
{
    int id;//这里添加了一个id，用于归结寻找产生式
    Sign LeftSign;//左部符号
    vector<Sign> RightSign;//右部符号列表
    ProduceForms();
    friend ostream& operator << (ostream& os, const ProduceForms* s);
    friend fstream& operator << (fstream& os, const ProduceForms* s);
    //重载等于 方便判断
    bool operator==(const ProduceForms& s) {
        if (s.RightSign.size() != this->RightSign.size())
            return false;
        if (!(this->LeftSign == s.LeftSign))
            return false;
        for (int i = 0; i < (int)RightSign.size(); i++)
        {
            if (!(this->RightSign[i] == s.RightSign[i]))
                return false;
        }
        return true;
    }
};

struct Actions
{
    int State;//跳转到的状态
    int ProduceFormsId;//产生式的标号
    ActionType Type;//是移进还是归约
    friend ostream& operator << (ostream& os, const Actions& s);
    friend ofstream& operator << (ofstream& os, const Actions& s);
    bool operator==(const Actions& s)const {
        return State == s.State && ProduceFormsId == s.ProduceFormsId && Type == s.Type;
    }
    bool operator != (const Actions& s)const {
        return !(*this == s);
    }
};

class GrammaAnalysis;
class SemanticAnalysis;

class LR1Production :public ProduceForms
{
private:
    int DotPosition;//点的位置
    Sign Foresee;//展望
public:
    friend GrammaAnalysis;
    friend ostream& operator << (ostream& os, const LR1Production* s);
    LR1Production(int MyDotPosition, Sign MyForesee, const ProduceForms& BaseProdction);
    void PopSign();//出栈，点往后移一格
    LR1Production* next_LR1Production(Sign);//通过符号生成下一些项目
    bool operator==(LR1Production& s) {
        if (s.DotPosition != this->DotPosition)
            return false;
        if (!(this->Foresee == s.Foresee))
            return false;
        if (s.RightSign.size() != this->RightSign.size())
            return false;
        if (!(this->LeftSign == s.LeftSign))
            return false;
        for (int i = 0; i < (int)RightSign.size(); i++)
        {
            if (!(this->RightSign[i] == s.RightSign[i]))
                return false;
        }
        return true;
    }
};

//项目集
class ProductionSet
{
private:
    vector<LR1Production*> LR1Productions;
    int id;//用于标识不同的状态
    vector<ProductionSet*>NextNode;//下一个状态
    unordered_map<string, int> MyMap;//使用string映射到map
public:
    friend GrammaAnalysis;
    friend ostream& operator << (ostream& os, const ProductionSet* s);
    ProductionSet(vector<LR1Production> MyLR1Productions, int MyId);
    ProductionSet();
    bool operator==(ProductionSet& s) {
        if (this->LR1Productions.size() != s.LR1Productions.size())
            return false;
        for (int i = 0; i < (int)LR1Productions.size(); i++)
            if (!(*this->LR1Productions[i] == *s.LR1Productions[i]))
                return false;
        return true;
    }

};

//语义树结点
struct SemanticTreeNode
{
    string Name;//如果Name = num ，则ival和fval里面有值
    vector <SemanticTreeNode> content;//内容，快速记录，终结符的Name和content的str是一样的
    SignType type;//非终结符还是终结符
    vector<int> NextList;
    vector<int> FalseList;
    vector<int> TrueList;//这三个数组用来合并
    int Quad;//给M,N,A用
    int IsNull;//是否为空，快速判断
    NameTableKind Namekind;//名字的类型
    //如果是数值
    NameTableType ValTpye; //值类型
    int ival;
    float fval;//需要被储存
    //非终结符的一些属性
    bool normal;//是形参还是实参
    int Adr;//如果是函数，记录地址(这里的地址应该是四元式的编号)
    vector<int> dems;//数组维度,从后往前
    vector<Par> param;//参数，如果是函数
};


class SemanticAnalysis
{
public:
    friend ofstream& operator << (ofstream& os, const vector<Var>& s);
    friend ofstream& operator << (ofstream& os, const vector<Func>& s);
    friend ofstream& operator << (ofstream& os, const Code& s);
    vector<Var> VarTable;//变量表
    vector<Func> FuncTable;//函数表
    vector<Array_> ArrayTable;//数组表
    vector<Code> MiddleCodeTable;//中间代码表


    //下面是语义分析的各种表
    stack <SemanticTreeNode> SemanticStack;//语义分析栈
    vector <SemanticTreeNode> StorePop;//储存pop出来的值

    //层级
    int Level;
    //目前的过程编号
    int ProcNo;
    vector<int> ProcNoStack;//过程栈

    //下面是语义动作函数
    Sign SemanticLeftSign;
    SemanticError semanticerror; //在调用下面的函数的时候，自己进栈出栈
    //申请中间变量
    int MiddleVarIndex;
    string MallocVar();
    //出栈函数
    void GetStorePop(int len);

    // 实参列表 规约成实参列表的第一个产生式，下面含义同
    //<实参列表> ::=<表达式>| <表达式> , <实参列表>|空
    void ArgumentList_1();
    void ArgumentList_2();
    void ArgumentList_3();
    //因子
    //<因子> ::=num |  ( <表达式> )  |ID  |ID ( <实参列表> ) | ID <数组>
    void Factor_1();
    void Factor_2();
    void Factor_3();
    void Factor_4();
    void Factor_5();
    //项
    //<项> ::= <因子>| <因子> * <项>|<因子> / <项>
    void Item_1();
    void Item_23();
    //加法表达式
    //<加法表达式> ::= <项> |<项> + <加法表达式>|<项> - <加法表达式>
    void AddExpr_1();
    void AddExpr_23();
    //relop
    //<relop>::=<|<=|>|>=|==|!=
    void Relop();
    //表达式
    //<表达式>::=<加法表达式>|<加法表达式> <relop> <加法表达式>
    void Expression_1();
    void Expression_2();
    //<Program> ::= <N> <声明串>
    void Program();
    //<N> ::=空
    void ProN();
    //除了出栈 入栈 没有其他操作的
    //<声明串> :: = <声明> | <声明> <声明串>
    void PopPush(int len);
    //<声明> ::=int  ID <M> <A> <声明类型> | void  ID <M> <A>  <函数声明>|float  ID <M> <A> <声明类型>
    void Statement();
    //<M> ::= 空
    void ProM();
    //<A> ::= 空
    void ProA();
    //<声明类型>:: = <变量声明> | <函数声明>
    void StateTypeF();
    //<变量声明> ::=  ;
    void VarStatement();
    //<函数声明> ::= ( <形参> ) <语句块>
    void FuncStatement();
    //<形参>::= <参数列表> | void
    void FormalParameters();
    //<参数列表>  ::= <参数> <逗号和参数>
    void ParametersList();
    //<逗号和参数> ::= , <参数> <逗号和参数>
    void CommaParameter1();
    //<逗号和参数> ::= 空
    void CommaParameter2();
    //<参数> ::= int  ID|float  ID
    void Parameter();
    //<语句块> ::= { <内部声明>  <语句串> }
    void StatementBlock();
    //<内部声明> ::= 空 | <内部变量声明> ; <内部声明> |  <内部数组声明> ; <内部声明>
    //直接调用void PopPush(int len);

    //<内部变量声明>::=int  ID|float  ID
    void InnerVarState();
    //<语句串> ::= <语句>|<语句> <M> <语句串>
    void SentenceList1();
    void SentenceList2();
    //<语句> ::= <if语句> |<while语句> | <return语句> | <赋值语句>
    void Sentence();
    //<赋值语句> ::=  ID = <表达式> ;|ID <数组> = <表达式> ;
    void AssignMent();
    void AssignMent1();
    //<return语句> :: = return <表达式>; | return;
    void Return1();
    void Return2();
    //<while语句> ::= while <M> ( <表达式> ) <A> <语句块>
    void While();
    //<if语句> ::= if  ( <表达式> )  <A> <语句块>  |if  ( <表达式> ) <A>  <语句块> <N> else <M> <A> <语句块>
    void If1();
    void If2();
    //<内部数组声明> ::= int ID <数组>
    void ArrayStatement();

    //<数组> ::= [ num ] | [ num ] <数组>
    void ArrayFactor1();
    void ArrayFactor2();



    //查找函数表 没有返回-1
    int FindFuncTable(const string& FuncName);
    //回填
    void BackPatch(vector<int>& BackList, int Addr);
    //归约时调用语义动作函数
    void SemanticAction(ProduceForms* ResoProduce);
    //语义分析报错函数
    //当前函数归约的语句块返回类型
    int ReType;
    string ErrorFuncName;
    string ErrorVarName;
    //void SDisError(int WordPos, SemanticError ErrType);
    //查变量表
    int  CheckVarTable(const string& name);
    //查数组变量
    int  CheckArrayTable(const string& name, vector<int>& dims);
    //删变量表
    void DeleteVarTable(int level);
    //寻找目前使用的变量,返回类型为var
    Var FindVarTable(string name);
    //得到中间代码表的变量名
    string GetMiddleName(string Name, vector<int>dims = vector<int>{});
    //打印函数
    void PrintVarTable();
    void PrintFuncTable();
    void PrintCodeTable();
};



class GrammaAnalysis
{
public:
    GrammaAnalysis();
    ~GrammaAnalysis();

    int Forward(lexer* MyWordSets);//前向传播，生成语法树
    int ErrorLine;

    string FilePath, TableOut, GraphOut, GraphOutDot;
    lexer* WordSets;//词法分析器的结果，输入符号
    SemanticAnalysis SemanticProcess;
    stack <TreeNode> SignStack;//符号栈
    stack <int> StateStack;//状态栈
    vector<ProduceForms*>ProduceFormsVec;//一个项目集
    vector<ProductionSet*>ProductionSetVec;//是项目集集合
    unordered_map<string, set<Sign>> first_table;//使用string映射到map
    set<Sign> GetFirstSign(vector<Sign> InSign);//输入连续字符串，得到first集
    set<Sign> GetFirstSign(Sign InSign);//输入某个符号，得到first集
    map< pair<int, Sign>, Actions>ActionTable;//action表
    map< pair<int, Sign>, int>GoTable;//goto表
    set<Sign>AllTerminator;//终结符集合 包括空
    set<Sign>AllNonTerminator;//非终结符集合


    //下面是语法分析函数
    void GetFull(ProductionSet& MyDFAState);//通过当前LR1Productions中已有的状态求闭包
    int ProduceFormsGenerate(string file);//产生一个包含ProduceForms的vector
    void BuildDFA();//建立DFA
    //这里还要有Action和Goto的函数的建立
    int BuildActionGo();
    bool Conflict_Detection(const Actions& action, pair<int, Sign>& tableindex, int i, LR1Production* j);
    void PrintActionGoToFile();
    int printParseDFA();
    void SDisError(int WordPos, SemanticError ErrType);

};

ostream& operator << (ostream& os, const LR1Production* s);
ostream& operator << (ostream& os, const ProductionSet* s);
ostream& operator << (ostream& os, const ProduceForms* s);
fstream& operator << (fstream& os, const ProduceForms* s);
ofstream& operator << (ofstream& os, const Var& s);
ofstream& operator << (ofstream& os, const Func& s);
ofstream& operator << (ofstream& os, const Code& s);
ofstream& operator << (ofstream& os, const vector<Var>& s);

static string& ClearAllSpace(string& str);
static void analysis_right(vector<Sign>& RightSign, const string& str);