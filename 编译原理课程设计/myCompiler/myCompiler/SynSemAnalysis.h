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


//�ķ���������
#define GRAMMAR_FILE_OPEN_ERROR 2
#define SUCCESS 1
#define GRAMMAR_ERROR 3
#define GRAMMAR_ERROR_FILE_OPEN_ERROR 4

//�﷨����������̴���
#define ACTION_ERROR 2
#define GOTO_ERROR  3
#define REDUCTION_PROCESS_FILE_OPEN_ERROR 4
#define PARSE_ERROR_FILE_OPEN_ERROR 5
#define TREE_DOT_FILE_OPEN_ERROR 6
#define DFA_DOT_FILE_OPEN_ERROR 7
#define SEMANTIC_ERROR 8


//����ļ���ַ
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


//��������
enum SignType {
    StartTerminator, //��ʼ��
    Terminator,     //�ս��
    NonTerminator,   //���ս��
    Empty             //��
};
//�﷨����
enum ActionType {
    Resolution,    //��Լ
    MoveIn,         //�ƽ�
    Acc             //����̬
};
//����������
enum NameTableKind {
    Varible,     //����
    ConstV,      //������
    Array,       //����
    Proc         //����
};
//���������������
enum NameTableType {
    INT1,            //int
    FLOAT1,          //float
    VOID1            //void
};
//�����������
enum SemanticError {
    None,          //�޴���
    NoFuncName,     //����δ����
    FuncParLenError,  //�����������ȴ���
    FuncReturnErr,   //�������ش���
    NoMainName,      //û��main����
    RedefineVar,     //�ض���
    NoVar,           //δ�������
    ArrayIndexError //������������
};


//����Ԫ��
struct Var {
    string name;//���ֱ�ʶ��
    NameTableType type;//���ֵ�����
    NameTableKind valtype;//��������
    int ival;        //int����ֵ
    float fval;       //float����ֵ
    int ProcNo;//���̱��
    vector<int> dims;//ά�����飬�Ӻ���ǰ
};

struct Par {
    NameTableType type;//��������
    string name;//������
    vector<int> dims;
};



//����Ԫ��
struct Func {
    string name;//���ֱ�ʶ��
    NameTableType type;//����������
    int adr;//���̵ĵ�ַ
    vector<Par> param;//����
};

//����Ԫ��
struct Array_ {
    string name;//���ֱ�ʶ��
    int len;//����Ԫ�ظ���
};

//��Ԫʽ
struct Code {
    string op;
    string op1;
    string op2;
    string linkres;
};

//�﷨�����
struct TreeNode {
    string str;//�ַ�����
    string StrType;//�ڲ��ı�ʾ
    SignType type;//����
};

struct Sign
{
    string str;//�ַ�����
    SignType type;//����
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
    //���ص��ں� �����ж�
    bool operator==(const Sign& s) {
        return (s.str == this->str && s.type == this->type);
    }
    //����С�ں� �������map set�����ݽṹ
    bool operator<(const Sign& s)const {
        return (this->str < s.str);
    }
};

struct ProduceForms
{
    int id;//���������һ��id�����ڹ��Ѱ�Ҳ���ʽ
    Sign LeftSign;//�󲿷���
    vector<Sign> RightSign;//�Ҳ������б�
    ProduceForms();
    friend ostream& operator << (ostream& os, const ProduceForms* s);
    friend fstream& operator << (fstream& os, const ProduceForms* s);
    //���ص��� �����ж�
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
    int State;//��ת����״̬
    int ProduceFormsId;//����ʽ�ı��
    ActionType Type;//���ƽ����ǹ�Լ
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
    int DotPosition;//���λ��
    Sign Foresee;//չ��
public:
    friend GrammaAnalysis;
    friend ostream& operator << (ostream& os, const LR1Production* s);
    LR1Production(int MyDotPosition, Sign MyForesee, const ProduceForms& BaseProdction);
    void PopSign();//��ջ����������һ��
    LR1Production* next_LR1Production(Sign);//ͨ������������һЩ��Ŀ
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

//��Ŀ��
class ProductionSet
{
private:
    vector<LR1Production*> LR1Productions;
    int id;//���ڱ�ʶ��ͬ��״̬
    vector<ProductionSet*>NextNode;//��һ��״̬
    unordered_map<string, int> MyMap;//ʹ��stringӳ�䵽map
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

//���������
struct SemanticTreeNode
{
    string Name;//���Name = num ����ival��fval������ֵ
    vector <SemanticTreeNode> content;//���ݣ����ټ�¼���ս����Name��content��str��һ����
    SignType type;//���ս�������ս��
    vector<int> NextList;
    vector<int> FalseList;
    vector<int> TrueList;//���������������ϲ�
    int Quad;//��M,N,A��
    int IsNull;//�Ƿ�Ϊ�գ������ж�
    NameTableKind Namekind;//���ֵ�����
    //�������ֵ
    NameTableType ValTpye; //ֵ����
    int ival;
    float fval;//��Ҫ������
    //���ս����һЩ����
    bool normal;//���βλ���ʵ��
    int Adr;//����Ǻ�������¼��ַ(����ĵ�ַӦ������Ԫʽ�ı��)
    vector<int> dems;//����ά��,�Ӻ���ǰ
    vector<Par> param;//����������Ǻ���
};


class SemanticAnalysis
{
public:
    friend ofstream& operator << (ofstream& os, const vector<Var>& s);
    friend ofstream& operator << (ofstream& os, const vector<Func>& s);
    friend ofstream& operator << (ofstream& os, const Code& s);
    vector<Var> VarTable;//������
    vector<Func> FuncTable;//������
    vector<Array_> ArrayTable;//�����
    vector<Code> MiddleCodeTable;//�м�����


    //��������������ĸ��ֱ�
    stack <SemanticTreeNode> SemanticStack;//�������ջ
    vector <SemanticTreeNode> StorePop;//����pop������ֵ

    //�㼶
    int Level;
    //Ŀǰ�Ĺ��̱��
    int ProcNo;
    vector<int> ProcNoStack;//����ջ

    //���������嶯������
    Sign SemanticLeftSign;
    SemanticError semanticerror; //�ڵ�������ĺ�����ʱ���Լ���ջ��ջ
    //�����м����
    int MiddleVarIndex;
    string MallocVar();
    //��ջ����
    void GetStorePop(int len);

    // ʵ���б� ��Լ��ʵ���б�ĵ�һ������ʽ�����溬��ͬ
    //<ʵ���б�> ::=<���ʽ>| <���ʽ> , <ʵ���б�>|��
    void ArgumentList_1();
    void ArgumentList_2();
    void ArgumentList_3();
    //����
    //<����> ::=num |  ( <���ʽ> )  |ID  |ID ( <ʵ���б�> ) | ID <����>
    void Factor_1();
    void Factor_2();
    void Factor_3();
    void Factor_4();
    void Factor_5();
    //��
    //<��> ::= <����>| <����> * <��>|<����> / <��>
    void Item_1();
    void Item_23();
    //�ӷ����ʽ
    //<�ӷ����ʽ> ::= <��> |<��> + <�ӷ����ʽ>|<��> - <�ӷ����ʽ>
    void AddExpr_1();
    void AddExpr_23();
    //relop
    //<relop>::=<|<=|>|>=|==|!=
    void Relop();
    //���ʽ
    //<���ʽ>::=<�ӷ����ʽ>|<�ӷ����ʽ> <relop> <�ӷ����ʽ>
    void Expression_1();
    void Expression_2();
    //<Program> ::= <N> <������>
    void Program();
    //<N> ::=��
    void ProN();
    //���˳�ջ ��ջ û������������
    //<������> :: = <����> | <����> <������>
    void PopPush(int len);
    //<����> ::=int  ID <M> <A> <��������> | void  ID <M> <A>  <��������>|float  ID <M> <A> <��������>
    void Statement();
    //<M> ::= ��
    void ProM();
    //<A> ::= ��
    void ProA();
    //<��������>:: = <��������> | <��������>
    void StateTypeF();
    //<��������> ::=  ;
    void VarStatement();
    //<��������> ::= ( <�β�> ) <����>
    void FuncStatement();
    //<�β�>::= <�����б�> | void
    void FormalParameters();
    //<�����б�>  ::= <����> <���źͲ���>
    void ParametersList();
    //<���źͲ���> ::= , <����> <���źͲ���>
    void CommaParameter1();
    //<���źͲ���> ::= ��
    void CommaParameter2();
    //<����> ::= int  ID|float  ID
    void Parameter();
    //<����> ::= { <�ڲ�����>  <��䴮> }
    void StatementBlock();
    //<�ڲ�����> ::= �� | <�ڲ���������> ; <�ڲ�����> |  <�ڲ���������> ; <�ڲ�����>
    //ֱ�ӵ���void PopPush(int len);

    //<�ڲ���������>::=int  ID|float  ID
    void InnerVarState();
    //<��䴮> ::= <���>|<���> <M> <��䴮>
    void SentenceList1();
    void SentenceList2();
    //<���> ::= <if���> |<while���> | <return���> | <��ֵ���>
    void Sentence();
    //<��ֵ���> ::=  ID = <���ʽ> ;|ID <����> = <���ʽ> ;
    void AssignMent();
    void AssignMent1();
    //<return���> :: = return <���ʽ>; | return;
    void Return1();
    void Return2();
    //<while���> ::= while <M> ( <���ʽ> ) <A> <����>
    void While();
    //<if���> ::= if  ( <���ʽ> )  <A> <����>  |if  ( <���ʽ> ) <A>  <����> <N> else <M> <A> <����>
    void If1();
    void If2();
    //<�ڲ���������> ::= int ID <����>
    void ArrayStatement();

    //<����> ::= [ num ] | [ num ] <����>
    void ArrayFactor1();
    void ArrayFactor2();



    //���Һ����� û�з���-1
    int FindFuncTable(const string& FuncName);
    //����
    void BackPatch(vector<int>& BackList, int Addr);
    //��Լʱ�������嶯������
    void SemanticAction(ProduceForms* ResoProduce);
    //�������������
    //��ǰ������Լ�����鷵������
    int ReType;
    string ErrorFuncName;
    string ErrorVarName;
    //void SDisError(int WordPos, SemanticError ErrType);
    //�������
    int  CheckVarTable(const string& name);
    //���������
    int  CheckArrayTable(const string& name, vector<int>& dims);
    //ɾ������
    void DeleteVarTable(int level);
    //Ѱ��Ŀǰʹ�õı���,��������Ϊvar
    Var FindVarTable(string name);
    //�õ��м�����ı�����
    string GetMiddleName(string Name, vector<int>dims = vector<int>{});
    //��ӡ����
    void PrintVarTable();
    void PrintFuncTable();
    void PrintCodeTable();
};



class GrammaAnalysis
{
public:
    GrammaAnalysis();
    ~GrammaAnalysis();

    int Forward(lexer* MyWordSets);//ǰ�򴫲��������﷨��
    int ErrorLine;

    string FilePath, TableOut, GraphOut, GraphOutDot;
    lexer* WordSets;//�ʷ��������Ľ�����������
    SemanticAnalysis SemanticProcess;
    stack <TreeNode> SignStack;//����ջ
    stack <int> StateStack;//״̬ջ
    vector<ProduceForms*>ProduceFormsVec;//һ����Ŀ��
    vector<ProductionSet*>ProductionSetVec;//����Ŀ������
    unordered_map<string, set<Sign>> first_table;//ʹ��stringӳ�䵽map
    set<Sign> GetFirstSign(vector<Sign> InSign);//���������ַ������õ�first��
    set<Sign> GetFirstSign(Sign InSign);//����ĳ�����ţ��õ�first��
    map< pair<int, Sign>, Actions>ActionTable;//action��
    map< pair<int, Sign>, int>GoTable;//goto��
    set<Sign>AllTerminator;//�ս������ ������
    set<Sign>AllNonTerminator;//���ս������


    //�������﷨��������
    void GetFull(ProductionSet& MyDFAState);//ͨ����ǰLR1Productions�����е�״̬��հ�
    int ProduceFormsGenerate(string file);//����һ������ProduceForms��vector
    void BuildDFA();//����DFA
    //���ﻹҪ��Action��Goto�ĺ����Ľ���
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