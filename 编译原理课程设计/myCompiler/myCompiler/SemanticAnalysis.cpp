#pragma execution_character_set("utf-8") 

#include "SynSemAnalysis.h"

using namespace std;


//语义动作调用函数
//思路：根据id顺序调用
void SemanticAnalysis::SemanticAction(ProduceForms* ResoProduce)
{
    switch (ResoProduce->id)
    {
        case 0:
            break;
        case 1: //[ <Program>--><N> <声明串>  ]
            Program();
            break;
        case 2: //[ <N>-->空  ]
            ProN();
            break;
        case 3: //[ <声明串>--><声明>  ]
        case 4: //[ <声明串>--><声明> <声明串>  ]
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 5://[ <声明>-->int ID <M> <A> <声明类型>  ]
        case 6://[ <声明>-->void ID <M> <A> <函数声明>  ]
        case 7://[ <声明>-->float ID <M> <A> <声明类型>  ]
            Statement();
            break;
        case 8://[ <M>-->空  ]
            ProM();
            break;
        case 9://[ <A>-->空  ]
            ProA();
            break;
        case 10://[ <声明类型>--><变量声明>  ]
        case 11://[ <声明类型>--><函数声明>  ]
            StateTypeF();
            break;
        case 12://[ <变量声明>-->;  ]
            VarStatement();
            break;
        case 13://[ <函数声明>-->( <形参> ) <语句块>  ]
            FuncStatement();
            break;
        case 14://[ <形参>--><参数列表>  ]
        case 15://[ <形参>-->void  ]
            FormalParameters();
            break;
        case 16://[ <参数列表>--><参数> <逗号和参数>  ]
            ParametersList();
            break;
        case 17://[ <逗号和参数>-->, <参数> <逗号和参数>  ]
            CommaParameter1();
            break;
        case 18://[ <逗号和参数>-->空  ]
            CommaParameter2();
            break;
        case 19://[ <参数>-->int ID  ]
        case 20://[ <参数>-->float ID  ]
            Parameter();
            break;
        case 21://[ <语句块>-->{ <内部声明> <语句串> }  ]
            StatementBlock();
            break;
        case 22://[ <内部声明>-->空  ]
            PopPush(0);
            break;
        case 23://[ <内部声明>--><内部变量声明> ; <内部声明>  ]
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 24://[ <内部变量声明>-->int ID  ]
        case 25://[ <内部变量声明>-->float ID  ]
            InnerVarState();
            break;
        case 26://[ <语句串>--><语句>  ]
            SentenceList1();
            break;
        case 27://[ <语句串>--><语句> <M> <语句串>  ]
            SentenceList2();
            break;
        case 28://[ <语句>--><if语句>  ]
        case 29://[ <语句>--><while语句>  ]
        case 30://[ <语句>--><return语句>  ]
        case 31://[ <语句>--><赋值语句>  ]
            Sentence();
            break;
        case 32://[ <赋值语句>-->ID = <表达式> ;  ]
            AssignMent();
            break;
        case 33://[ <return语句>-->return <表达式> ;  ]
            Return1();
            break;
        case 34://[ <return语句>-->return ;  ]
            Return2();
            break;
        case 35://[ <while语句>-->while <M> ( <表达式> ) <A> <语句块>  ]
            While();
            break;
        case 36://[ <if语句>-->if ( <表达式> ) <A> <语句块>  ]
            If1();
            break;
        case 37://[ <if语句>-->if ( <表达式> ) <A> <语句块> <N> else <M> <A> <语句块>  ]
            If2();
            break;
        case 38://[ <表达式>--><加法表达式>  ]
            Expression_1();
            break;
        case 39://[ <表达式>--><加法表达式> <relop> <加法表达式>  ]
            Expression_2();
            break;
        case 40://[ <relop>--><  ]
        case 41://[ <relop>--><=  ]
        case 42://[ <relop>-->>  ]
        case 43://[ <relop>-->>=  ]
        case 44://[ <relop>-->==  ]
        case 45://[ <relop>-->!=  ]
            Relop();
            break;
        case 46://[ <加法表达式>--><项>  ]
            AddExpr_1();
            break;
        case 47://[ <加法表达式>--><项> + <加法表达式>  ]
        case 48://[ <加法表达式>--><项> - <加法表达式>  ]
            AddExpr_23();
            break;
        case 49://[ <项>--><因子>  ]
            Item_1();
            break;
        case 50://[ <项>--><因子> * <项>  ]
        case 51://[ <项>--><因子> / <项>  ]
            Item_23();
            break;
        case 52://[ <因子>-->num  ]
            Factor_1();
            break;
        case 53://[ <因子>-->( <表达式> )  ]
            Factor_2();
            break;
        case 54://[ <因子>-->ID  ]
            Factor_3();
            break;
        case 55://[ <因子>-->ID ( <实参列表> )  ]
            Factor_4();
            break;
        case 56://[ <实参列表>--><表达式>  ]
            ArgumentList_1();
            break;
        case 57://[ <实参列表>--><表达式> , <实参列表>  ]
            ArgumentList_2();
            break;
        case 58://[ <实参列表>-->空  ]
            ArgumentList_3();
            break;
        case 59://<内部声明> ::= <内部数组声明> ; <内部声明>
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 60://<内部数组声明> ::= int ID <数组>
            ArrayStatement();
            break;
        case 61://<数组> ::= [ num ]
            ArrayFactor1();
            break;
        case 62://<数组> ::= [ num ] <数组>
            ArrayFactor2();
            break;
        case 63://<赋值语句> ::= ID <数组> = <表达式> ;
            AssignMent1();
            break;
        case 64://<因子> ::= ID <数组>
            Factor_5();
            break;

        default:
            break;
    }
}


//查找函数表 没有返回-1
int SemanticAnalysis::FindFuncTable(const string& FuncName)
{
    for (int i = 0; i < int(FuncTable.size()); i++)
    {
        if (FuncTable[i].name == FuncName)
        {
            return i;
        }
    }
    return -1;
}
//<Program> :: = <N> <声明串>
//$语义 将main的地址回填到N的nextlist
void SemanticAnalysis::Program()
{
    int FuncIndex = FindFuncTable("main");
    if (FuncIndex == -1) {
        semanticerror = NoMainName;
        return;
    }
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode program;
    program.Name = SemanticLeftSign.str;
    SemanticTreeNode& N = StorePop[1];
    BackPatch(N.NextList, FuncTable[FuncIndex].adr);
    SemanticStack.push(program);
}
//<N> :: = 空
//$语义 N.nextlist = { nextquad }
//$语义 emit(j, _, _, -1)
void SemanticAnalysis::ProN()
{
    SemanticTreeNode N;
    N.Name = SemanticLeftSign.str;
    N.NextList.push_back(MiddleCodeTable.size());
    MiddleCodeTable.push_back(Code{ "j","-" ,"-","-1" });
    SemanticStack.push(N);
}
//除了出栈 入栈 没有其他操作的
void SemanticAnalysis::PopPush(int len)
{
    SemanticTreeNode NothingToDo;
    NothingToDo.Name = SemanticLeftSign.str;
    GetStorePop(len);
    SemanticStack.push(NothingToDo);
}
//<声明> :: = int  ID <M> <A> <声明类型> | void  ID <M> <A>  <函数声明> | float  ID <M> <A> <声明类型>
//$语义 对于声明类型 如果是变量声明 此处形成一个完整的变量声明 将变量名 类型 （变量层次）加入变量表
//$语义 对于声明类型 如果是函数声明 此处形成一个完整的函数声明 将函数名 类型 参数表 函数地址加入函数表
//$语义 对于函数声明 此处形成一个完整的函数声明 将函数名 类型 参数表 函数地址加入函数表
void SemanticAnalysis::Statement()
{
    int RightLen = 5;
    GetStorePop(RightLen);
    SemanticTreeNode State;
    State.Name = SemanticLeftSign.str;
    SemanticTreeNode& Type = StorePop[4];
    SemanticTreeNode& ID = StorePop[3];
    SemanticTreeNode& M = StorePop[2];
    SemanticTreeNode& A = StorePop[1];
    SemanticTreeNode& StateType = StorePop[0];
    //函数
    if (StateType.Namekind == Proc) {
        NameTableType ValTpye = Type.Name == "int" ? INT1 : Type.Name == "float" ? FLOAT1 : VOID1;
        if ((ValTpye == VOID1 && ReType != VOID1) || (ValTpye != VOID1 && ReType == VOID1)) {
            semanticerror = FuncReturnErr;
            ErrorFuncName = ID.Name;
            return;
        }
        FuncTable.push_back(Func{ ID.Name,ValTpye,M.Quad,StateType.param });
    }
    else {
        //问题很大
        NameTableType ValTpye = Type.Name == "int" ? INT1 : FLOAT1;
        VarTable.push_back(Var{ ID.Name,ValTpye,Varible,Type.ival,Type.fval,ProcNoStack[int(ProcNoStack.size() - 1)] });
    }
    SemanticStack.push(State);
}
void SemanticAnalysis::ProM()
{
    SemanticTreeNode M;
    M.Name = SemanticLeftSign.str;
    M.Quad = MiddleCodeTable.size();
    SemanticStack.push(M);
}
//<A> ::= 空
void SemanticAnalysis::ProA()
{
    SemanticTreeNode A;
    A.Name = SemanticLeftSign.str;
    ProcNo++;
    ProcNoStack.push_back(ProcNo);
    SemanticStack.push(A);
}
//<声明类型>:: = <变量声明> | <函数声明>
//$语义 对于变量声明 记录当前声明类型为变量
//$语义 对于函数声明 直接传递参数表等属性
void SemanticAnalysis::StateTypeF()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode State = StorePop[0];
    State.Name = SemanticLeftSign.str;
    SemanticStack.push(State);
}

//<变量声明> :: = ;
//$语义  此处类型为变量 之后归约不用特判
void SemanticAnalysis::VarStatement()
{
    ProcNoStack.pop_back();
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode VarState;
    VarState.Name = SemanticLeftSign.str;
    VarState.Namekind = Varible;
    SemanticStack.push(VarState);
}
//<函数声明> :: = (<形参>) <语句块>
//$语义 将左侧的形参列表置为<形参>的形参列表 类型为函数声明
void SemanticAnalysis::FuncStatement()
{
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode List = StorePop[2];
    List.Name = SemanticLeftSign.str;
    List.Namekind = Proc;

    SemanticStack.push(List);
}
//<形参>:: = <参数列表> | void
//$语义 对于参数列表 将左侧的形参列表置为<参数列表>的形参列表
//$语义 对于void 将左侧的形参列表置为空
void SemanticAnalysis::FormalParameters()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode fp = StorePop[0];
    fp.Name = SemanticLeftSign.str;
    for (auto& item : fp.param) {
        VarTable.push_back(Var{ item.name,item.type,Varible,0,0,ProcNoStack[int(ProcNoStack.size() - 1)] });
    }
    SemanticStack.push(fp);
}
//<参数列表>  :: = <参数> <逗号和参数>
//$语义 将左侧的参数列表置为右侧的参数的合并 包括数据类型等信息
void SemanticAnalysis::ParametersList()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode fp = StorePop[1];
    fp.Name = SemanticLeftSign.str;
    fp.param.insert(fp.param.end(), StorePop[0].param.begin(), StorePop[0].param.end());//将参数表合并
    SemanticStack.push(fp);
}
//<逗号和参数> ::= , <参数> <逗号和参数>
//$语义 将左侧的参数列表置为右侧的参数的合并 包括数据类型等信息
void SemanticAnalysis::CommaParameter1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode fp = StorePop[1];
    fp.Name = SemanticLeftSign.str;
    fp.param.insert(fp.param.end(), StorePop[0].param.begin(), StorePop[0].param.end());//将参数表合并
    SemanticStack.push(fp);
}
//<逗号和参数> ::= 空
//$语义 无 压栈出栈即可
void SemanticAnalysis::CommaParameter2()
{
    SemanticTreeNode fp;
    fp.Name = SemanticLeftSign.str;
    SemanticStack.push(fp);
}
//<参数> :: = int  ID | float  ID
//$语义 将左侧的形参列表置为一个参数 类型为 int | float 名为 ID
void SemanticAnalysis::Parameter()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode p;
    p.Name = SemanticLeftSign.str;
    if (StorePop[1].Name == "int")
        p.param.push_back({ INT1, StorePop[0].Name });
    else
        p.param.push_back({ FLOAT1, StorePop[0].Name });
    SemanticStack.push(p);
}
//<语句块> :: = { <内部声明>  <语句串> }
//$语义 nowLevel--
//$语义 <语句块>nextlist = <语句串>nextlist
void SemanticAnalysis::StatementBlock()
{
    ProcNoStack.pop_back();
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode statement_block = StorePop[1];
    statement_block.Name = SemanticLeftSign.str;
    SemanticStack.push(statement_block);
}

//<内部变量声明>:: = int  ID | float  ID
//$语义 此处形成一个完整的变量声明 将变量名 类型 （变量层次）加入变量表
void SemanticAnalysis::InnerVarState()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode InnerVar;
    InnerVar.Name = SemanticLeftSign.str;
    SemanticTreeNode& Type = StorePop[1];
    SemanticTreeNode& ID = StorePop[0];
    //问题很大
    int index = 0;
    if ((index = CheckVarTable(ID.Name)) != -1 && index == ProcNoStack[int(ProcNoStack.size()) - 1])
    {
        semanticerror = RedefineVar;
        ErrorVarName = ID.Name;
        return;
    }
    NameTableType ValTpye = Type.Name == "int" ? INT1 : FLOAT1;
    VarTable.push_back(Var{ ID.Name,ValTpye,Varible,Type.ival,Type.fval,ProcNoStack[int(ProcNoStack.size() - 1)] });
    SemanticStack.push(InnerVar);
}
//<语句串> :: = <语句> | <语句> <M> <语句串>
//$语义 <语句> 将右侧 nextList 传递给左侧
//$语义 <语句> <M> <语句串> 将右侧语句串 nextList 传递给左侧 用M回填语句nextlist
void SemanticAnalysis::SentenceList1()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode s = StorePop[1];
    BackPatch(s.NextList, StorePop[0].Quad);
    s.Name = SemanticLeftSign.str;
    SemanticStack.push(s);
}
void SemanticAnalysis::SentenceList2()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode& sentence = StorePop[2];
    SemanticTreeNode sentence_block = StorePop[0];
    SemanticTreeNode& M = StorePop[1];
    sentence_block.Name = SemanticLeftSign.str;
    BackPatch(sentence.NextList, M.Quad);
    BackPatch(sentence.NextList, M.Quad);
    SemanticStack.push(sentence_block);
}
//<语句> ::= <if语句> |<while语句> | <return语句> | <赋值语句>
void SemanticAnalysis::Sentence()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode s = StorePop[0];
    s.Name = SemanticLeftSign.str;
    SemanticStack.push(s);
}
//<赋值语句> ::=  ID = <表达式> ;
//$语义 生成赋值四元式
void SemanticAnalysis::AssignMent()
{
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode Assign;
    Assign.Name = SemanticLeftSign.str;
    SemanticTreeNode& Exp = StorePop[1];
    SemanticTreeNode& ID = StorePop[3];
    if (CheckVarTable(Exp.Name) != -1 || isdigit(Exp.Name[0]))
        ;
    else {
        ErrorVarName = Exp.Name;
        semanticerror = NoVar;
        return;
    }
    if (CheckVarTable(ID.Name) != -1)
    {
        string VIDName = GetMiddleName(ID.Name, ID.dems);
        string VExpName = GetMiddleName(Exp.Name, Exp.dems);
        MiddleCodeTable.push_back(Code{ "=",VExpName ,"-",VIDName });
    }
    else {
        ErrorVarName = ID.Name;
        semanticerror = NoVar;
        return;
    }
    SemanticStack.push(Assign);
}
//<return语句> :: = return <表达式> ; | return ;
//$语义 生成返回四元式
void SemanticAnalysis::Return1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode re;
    re.Name = SemanticLeftSign.str;
    SemanticTreeNode& Exp = StorePop[1];
    ReType = FLOAT1;
    //可以考虑赋值returnplace
    string VExpName = GetMiddleName(Exp.Name, Exp.dems);
    MiddleCodeTable.push_back({ "return",VExpName,"-","-" });
    SemanticStack.push(re);
}
void SemanticAnalysis::Return2()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode re;
    re.Name = SemanticLeftSign.str;

    ReType = VOID1;
    //可以考虑赋值returnplace
    MiddleCodeTable.push_back({ "return","-","-","-" });
    SemanticStack.push(re);
}
//<while语句> ::= while <M> ( <表达式> ) <A> <语句块>
//$语义 M.quad 回填到语句块 nextList
//$语义 左侧 nextList = 表达式 falselist
//$语义 生成 jump 四元式
void SemanticAnalysis::While()
{
    int RightLen = 7;
    GetStorePop(RightLen);
    SemanticTreeNode& M = StorePop[5];
    SemanticTreeNode& Exp = StorePop[3];
    SemanticTreeNode& S_block = StorePop[0];
    //backpatch
    BackPatch(S_block.NextList, M.Quad);
    //nextlist
    SemanticTreeNode while_sentence;
    while_sentence.NextList = Exp.FalseList;
    while_sentence.Name = SemanticLeftSign.str;
    MiddleCodeTable.push_back(Code{ "j","-","-",to_string(M.Quad) });
    SemanticStack.push(while_sentence);
}
//<if语句> ::= if  ( <表达式> )  <A> <语句块>  |if  ( <表达式> ) <A>  <语句块> <N> else <M> <A> <语句块>
//$语义 第一个产生式
//$语义 表达式 falseList  插入 语句块 nextList
//$语义 If 语句 nextList 设置为表达式 falseList
//$语义 第2个产生式前面一致
//$语义 M.quad 回填到表达式 falseList
//$语义 Merge nextList 作为 if 语句的 nextList
void SemanticAnalysis::If1()
{
    int RightLen = 6;
    GetStorePop(RightLen);
    SemanticTreeNode& Exp = StorePop[3];
    SemanticTreeNode& S_block = StorePop[0];
    SemanticTreeNode if_sentence;
    if_sentence.Name = SemanticLeftSign.str;
    //MERGE
    Exp.FalseList.insert(Exp.FalseList.end(), S_block.NextList.begin(), S_block.NextList.end());//将list合并
    if_sentence.NextList = Exp.FalseList;
    SemanticStack.push(if_sentence);
}
//<if语句> ::= if  ( <表达式> )  <A> <语句块>  |if  ( <表达式> ) <A>  <语句块> <N> else <M> <A> <语句块>
void SemanticAnalysis::If2()
{
    int RightLen = 11;
    GetStorePop(RightLen);
    SemanticTreeNode& Exp = StorePop[8];
    SemanticTreeNode& S_block1 = StorePop[5];
    SemanticTreeNode& N = StorePop[4];
    SemanticTreeNode& M = StorePop[2];
    SemanticTreeNode& S_block2 = StorePop[0];

    //backpatch
    BackPatch(Exp.FalseList, M.Quad);
    //MERGE
    SemanticTreeNode if_sentence;
    if_sentence.NextList = N.NextList;
    if_sentence.Name = SemanticLeftSign.str;
    auto& List = if_sentence.NextList;
    List.insert(List.end(), S_block1.NextList.begin(), S_block1.NextList.end());
    List.insert(List.end(), S_block2.NextList.begin(), S_block2.NextList.end());
    SemanticStack.push(if_sentence);
}
//回填
void SemanticAnalysis::BackPatch(vector<int>& BackList, int Addr)
{
    for (auto back : BackList) {
        MiddleCodeTable[back].linkres = to_string(Addr);
    }
}
//查变量表
int SemanticAnalysis::CheckVarTable(const string& name)
{
    for (int i = int(VarTable.size()) - 1; i >= 0; --i) {
        if (VarTable[i].name == name) {
            for (int j = 0; j < (int)ProcNoStack.size(); j++)
                if (VarTable[i].ProcNo == ProcNoStack[j])
                    return ProcNoStack[j];
        }
    }
    return -1;
}

int SemanticAnalysis::CheckArrayTable(const string& name, vector<int>& dims)
{
    for (int i = int(VarTable.size()) - 1; i >= 0; --i) {
        if (VarTable[i].name == name) {
            for (int j = 0; j < (int)ProcNoStack.size(); j++)
                if (VarTable[i].ProcNo == ProcNoStack[j])
                {
                    if (VarTable[i].dims.size() != dims.size())
                        return -2;
                    for (int i = 0; i < (int)VarTable[i].dims.size(); i++)
                        if (VarTable[i].dims[i] <= dims[i])
                            return -2;
                    return ProcNoStack[j];
                }
        }
    }
    return -1;

}

Var SemanticAnalysis::FindVarTable(string name)
{
    for (int i = (int)VarTable.size() - 1; i >= 0; i--)
    {
        if (VarTable[i].name == name)
        {
            return VarTable[i];
        }
    }
    Var temp;
    temp.ProcNo = -1;
    return temp;
}

string SemanticAnalysis::GetMiddleName(string Name, vector<int>dims)
{
    string name = Name;
    Var VNode = FindVarTable(Name);
    if (VNode.ProcNo == -1)
        return name;
    name = name + " " + to_string(VNode.ProcNo);
    if (VNode.valtype == Array)
    {
        for (int i = 0; i < (int)dims.size(); i++)
        {
            cout << "dims[i]:" << dims[i] << " ";
            name = name + " " + to_string(dims[i]);
        }
        cout << endl;
    }
    return name;
}


//在调用下面的函数的时候，产生式的右部已经pop出来并且存在了StorePop数组中

void SemanticAnalysis::GetStorePop(int len)
{
    StorePop.clear();
    for (int i = 0; i < len; i++)
    {
        StorePop.push_back(SemanticStack.top());
        SemanticStack.pop();
    }
}
////变量元素
//struct Var {
//	string name;//名字标识符
//	NameTableType type;//名字的类型
//	bool normal;//true为非形参，false为形参
//	int ival;
//	float fval;
//};

string SemanticAnalysis::MallocVar()
{
    Var temp;
    char name[50] = { 0 };
    sprintf(name, "T%d", MiddleVarIndex);
    MiddleVarIndex++;
    temp.name = name;
    temp.type = FLOAT1;
    temp.fval = 0;
    temp.ival = 0;
    temp.ProcNo = ProcNoStack[int(ProcNoStack.size()) - 1];
    VarTable.push_back(temp);
    return temp.name;
}



void SemanticAnalysis::Factor_1()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode Factor;
    SemanticTreeNode& Num = StorePop[0];
    Factor.Name = Num.Name;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    //Factor.content.push_back(Num);
//    Factor.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Factor);
}

void SemanticAnalysis::Factor_2()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode Factor;
    //    SemanticTreeNode Right_p= StorePop[0];
    SemanticTreeNode& Expr = StorePop[1];
    //    SemanticTreeNode Left_p = StorePop[2];
    Factor.Name = Expr.Name;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    //Factor.content.push_back(Left_p);
    //Factor.content.push_back(Expr);
    //Factor.content.push_back(Right_p);
//    Factor.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Factor);
}
void SemanticAnalysis::Factor_3()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode Factor;
    SemanticTreeNode& ID = StorePop[0];
    Factor.Name = ID.Name;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    //Factor.content.push_back(ID);
//    Factor.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Factor);
}

void SemanticAnalysis::Factor_4()
{
    SemanticTreeNode Factor;
    int RightLen = 4;
    GetStorePop(RightLen);
    for (int i = RightLen - 1; i >= 0; i--)
        Factor.content.push_back(StorePop[i]);
    //检查函数名是否对应
    bool IsFind = false;
    int FuncIndex = 0;
    for (int i = 0; i < (int)FuncTable.size(); i++)
    {
        if (FuncTable[i].name == Factor.content[0].Name)
        {
            IsFind = true;
            FuncIndex = i;
            break;
        }
    }
    if (!IsFind)
    {
        semanticerror = NoFuncName;
        ErrorFuncName = Factor.content[0].Name;
        return;
    }
    //检查参数数量是否一致
    if (FuncTable[FuncIndex].param.size() != Factor.content[2].param.size())
    {
        ErrorFuncName = Factor.content[0].Name;
        semanticerror = FuncParLenError;
    }
    for (int i = (int)Factor.content[2].param.size() - 1; i >= 0; i--)//生成参数压栈中间代码
    {
        string VFactorName = GetMiddleName(Factor.content[2].param[i].name, Factor.content[2].param[i].dims);
        MiddleCodeTable.push_back(Code{ "Par",VFactorName ,"-","-" });
    }
    string Temp = MallocVar();//申请中间变量，填到表中
    MiddleCodeTable.push_back(Code{ "=","@BackReturn","-",GetMiddleName(Temp) });

    MiddleCodeTable.push_back(Code{ "call",GetMiddleName(Factor.content[0].Name),"-","-" });

    Factor.Name = Temp;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    //    Factor.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Factor);
}

//<因子> ::= ID <数组>
//$语义: 检查下标是否正确
//$语义: 检查数组变量是否存在
void SemanticAnalysis::Factor_5()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode Factor;
    SemanticTreeNode& ID = StorePop[1];
    SemanticTreeNode& ArrayExpr = StorePop[0];
    Factor.Name = ID.Name;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    Factor.dems = ArrayExpr.dems;
    int error_num = CheckArrayTable(ID.Name, ArrayExpr.dems);
    if (error_num == -1) {
        ErrorVarName = ID.Name;
        semanticerror = NoVar;
        return;
    }
    else if (error_num == -2)
    {
        ErrorVarName = ID.Name;
        semanticerror = ArrayIndexError;
        return;
    }
    SemanticStack.push(Factor);
}



void SemanticAnalysis::ArgumentList_1()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode ArgumentList;
    SemanticTreeNode& Expr = StorePop[0];
    ArgumentList.Name = "<实参列表>";
    ArgumentList.normal = true;
    ArgumentList.type = NonTerminator;
    ArgumentList.IsNull = 0;
    //ArgumentList.content.push_back(Expr);
    ArgumentList.param.push_back(Par{ FLOAT1,Expr.Name,Expr.dems });
    //    ArgumentList.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(ArgumentList);
}

void SemanticAnalysis::ArgumentList_2()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode ArgumentList;
    SemanticTreeNode& Expr = StorePop[2];
    //    SemanticTreeNode Dot = StorePop[1];
    SemanticTreeNode& Parlist = StorePop[0];

    ArgumentList.Name = "<实参列表>";
    ArgumentList.type = NonTerminator;
    ArgumentList.normal = true;
    ArgumentList.IsNull = 0;
    //ArgumentList.content.push_back(Expr);//无关紧要
    ArgumentList.param = Parlist.param;
    ArgumentList.param.push_back(Par{ FLOAT1,Expr.Name,Expr.dems });
    //    ArgumentList.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(ArgumentList);
}


void SemanticAnalysis::ArgumentList_3()
{
    SemanticTreeNode ArgumentList;
    ArgumentList.Name = "<实参列表>";
    ArgumentList.IsNull = 1;
    SemanticStack.push(ArgumentList);
}

void SemanticAnalysis::Item_1()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode Item;
    SemanticTreeNode& Factor = StorePop[0];
    Item.Name = Factor.Name;
    Item.IsNull = 0;
    //Item.content.push_back(Factor);
    Item.normal = true;
    Item.type = NonTerminator;
    Item.dems = Factor.dems;
    //    Item.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Item);
}

void SemanticAnalysis::Item_23()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode Item;
    SemanticTreeNode& Factor = StorePop[2];
    SemanticTreeNode& CalSign = StorePop[1];
    SemanticTreeNode& LastItem = StorePop[0];
    Item.Name = MallocVar();//分配中间变量
    Item.IsNull = 0;
    //Item.content.push_back(Factor);
    //Item.content.push_back(Muti);
    //Item.content.push_back(LastItem);
    Item.type = NonTerminator;
    Item.normal = true;

    MiddleCodeTable.push_back(Code{ CalSign.Name,GetMiddleName(Factor.Name,Factor.dems),GetMiddleName(LastItem.Name,LastItem.dems),GetMiddleName(Item.Name,Item.dems) });
    //    Item.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Item);
}


void SemanticAnalysis::AddExpr_1()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode AddExpr;
    SemanticTreeNode& Factor = StorePop[0];
    AddExpr.Name = Factor.Name;
    AddExpr.IsNull = 0;
    AddExpr.normal = true;
    AddExpr.type = NonTerminator;
    AddExpr.dems = Factor.dems;
    //    AddExpr.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(AddExpr);
}


void SemanticAnalysis::AddExpr_23()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode AddExpr;
    SemanticTreeNode& Factor = StorePop[2];
    SemanticTreeNode& CalSign = StorePop[1];
    SemanticTreeNode& LastAddExpr = StorePop[0];
    AddExpr.Name = MallocVar();//分配中间变量
    AddExpr.IsNull = 0;
    AddExpr.type = NonTerminator;
    AddExpr.normal = true;

    MiddleCodeTable.push_back(Code{ CalSign.Name,GetMiddleName(Factor.Name,Factor.dems),GetMiddleName(LastAddExpr.Name,LastAddExpr.dems),GetMiddleName(AddExpr.Name,AddExpr.dems) });
    //    AddExpr.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(AddExpr);
}


void SemanticAnalysis::Relop()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode RelopExpr;
    SemanticTreeNode& CalSign = StorePop[0];
    RelopExpr.Name = CalSign.Name;
    RelopExpr.IsNull = 0;
    RelopExpr.type = NonTerminator;
    RelopExpr.normal = true;
    SemanticStack.push(RelopExpr);
}

void SemanticAnalysis::Expression_1()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode Expr;
    SemanticTreeNode& AddExpr = StorePop[0];
    Expr.Name = AddExpr.Name;
    Expr.IsNull = 0;
    Expr.type = NonTerminator;
    //    Expr.NextList.push_back((int)MiddleCodeTable.size());
    Expr.normal = true;
    Expr.dems = AddExpr.dems;
    SemanticStack.push(Expr);
}
void SemanticAnalysis::Expression_2()
{
    unordered_map<string, string>reverse;
    reverse["=="] = "!=";
    reverse["<="] = ">";
    reverse[">="] = "<";
    reverse["!="] = "==";
    reverse[">"] = "<=";
    reverse["<"] = ">=";
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode Expr;
    SemanticTreeNode& AddExpr1 = StorePop[2];
    SemanticTreeNode& Relop = StorePop[1];
    SemanticTreeNode& AddExpr2 = StorePop[0];
    string TempVar = MallocVar();
    Expr.Name = TempVar;
    Expr.IsNull = 0;
    Expr.type = NonTerminator;
    Expr.normal = true;
    Expr.FalseList.push_back((int)MiddleCodeTable.size());

    MiddleCodeTable.push_back(Code{ ("j" + reverse[Relop.Name]),GetMiddleName(AddExpr1.Name,AddExpr1.dems),GetMiddleName(AddExpr2.Name,AddExpr2.dems),"-1" });
    //    Expr.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Expr);
}
//<数组> ::= [ num ]
//$语义: 需要记录num的值
void SemanticAnalysis::ArrayFactor1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode ArrayFactor;
    SemanticTreeNode& dimennum = StorePop[1];
    ArrayFactor.dems.push_back(dimennum.ival);
    SemanticStack.push(ArrayFactor);
}

//<数组> ::= [ num ] <数组>
//$语义: 需要记录num的值
void SemanticAnalysis::ArrayFactor2()
{
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode ArrayFactor;
    SemanticTreeNode& PreArrayFactor = StorePop[0];
    SemanticTreeNode& dimennum = StorePop[2];
    ArrayFactor.dems = PreArrayFactor.dems;
    ArrayFactor.dems.push_back(dimennum.ival);
    SemanticStack.push(ArrayFactor);
}
//<内部数组声明> ::= int ID <数组>
//$语义: 记录变量表中数组类型变量的维度，数组的名称，以及数组所在的过程位置
void SemanticAnalysis::ArrayStatement()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode ArrayState;
    SemanticTreeNode& ArrayFactor = StorePop[0];
    SemanticTreeNode& ID = StorePop[1];
    SemanticTreeNode& type = StorePop[2];
    ArrayState.dems = ArrayFactor.dems;
    ArrayState.Namekind = Array;
    ArrayState.Name = ID.Name;
    VarTable.push_back(Var{ ID.Name,INT1,Array,0,0,ProcNoStack[int(ProcNoStack.size() - 1)],ArrayFactor.dems });
    SemanticStack.push(ArrayState);
}

//<赋值语句> ::= ID <数组> = <表达式> ;
//$语义: 检查下标是否正确
//$语义: 检查数组变量是否存在
void SemanticAnalysis::AssignMent1()
{
    int RightLen = 5;
    GetStorePop(RightLen);
    SemanticTreeNode AssignState;
    AssignState.Name = SemanticLeftSign.str;
    SemanticTreeNode& Exp = StorePop[1];
    SemanticTreeNode& ArrayExpr = StorePop[3];
    SemanticTreeNode& ID = StorePop[4];
    if (CheckVarTable(Exp.Name) != -1 || isdigit(Exp.Name[0]))
        ;
    else {
        ErrorVarName = Exp.Name;
        semanticerror = NoVar;
        return;
    }
    int error_num = CheckArrayTable(ID.Name, ArrayExpr.dems);
    if (error_num == -1) {
        ErrorVarName = ID.Name;
        semanticerror = NoVar;
        return;
    }
    else if (error_num == -2)
    {
        ErrorVarName = ID.Name;
        semanticerror = ArrayIndexError;
        return;
    }
    else
    {
        Var VID = FindVarTable(ID.Name);
        string VIDNo = VID.ProcNo != -1 ? to_string(VID.ProcNo) : "";//需要区分中间变量表中的每一个变量
        ID.Name = ID.Name + " " + VIDNo;
        for (int i = 0; i<int(ArrayExpr.dems.size()); i++)
        {
            ID.Name = ID.Name + " " + to_string(ArrayExpr.dems[i]);
        }
        MiddleCodeTable.push_back(Code{ "=",GetMiddleName(Exp.Name,Exp.dems) ,"-",ID.Name });
    }
    SemanticStack.push(AssignState);
}






//下面是一些文件输出函数(debug)
ofstream& operator << (ofstream& os, const Var& s)
{
    os << s.name << ": " << " s.fval[" << s.fval << "] s.ival[" << s.ival << "] s.type[" << s.type << "]" << " s.ProcNo[" << s.ProcNo << "]";
    return os;
}

ofstream& operator << (ofstream& os, const Func& s)
{
    os << s.name << ":";
    for (int i = 0; i < (int)s.param.size(); i++)
    {
        os << " param" << i + 1 << "[" << s.param[i].name << " " << s.param[i].type << "]";
    }
    os << " s.type[" << s.type << "]";
    os << " s.adr[" << s.adr << "]";
    return os;
}

ofstream& operator << (ofstream& os, const Code& s)
{
    os << s.op << ", " << s.op1 << ", " << s.op2 << ", " << s.linkres;
    return os;
}


void SemanticAnalysis::PrintVarTable()
{
    ofstream fvar(VAR_TABLE_FILE, ios::out);
    fvar<< "============================变量表=============================" << endl;
    for (int i = 0; i < (int)this->VarTable.size(); i++)
        fvar<< VarTable[i] << endl;
    fvar << "==============================================================" << endl;
    fvar.close();
}

void SemanticAnalysis::PrintFuncTable()
{
    ofstream ffunc(FUNC_TABLE_FILE, ios::out);
    ffunc << "============================函数表=============================" << endl;
    for (int i = 0; i < (int)FuncTable.size(); i++)
        ffunc << FuncTable[i] << endl;
    ffunc<< "==============================================================" << endl;
    ffunc.close();
}


void SemanticAnalysis::PrintCodeTable()
{
    ofstream fcode(QUATERNION_FILE, ios::out);
    fcode << "============================中间代码表=============================" << endl;
    for (int i = 0; i < (int)MiddleCodeTable.size(); i++)
    {
       fcode<< i << ":";
       fcode<< "(";
       fcode << MiddleCodeTable[i] << ")" << endl;
    }
    fcode<< "==============================================================" << endl;
    fcode.close();
}

