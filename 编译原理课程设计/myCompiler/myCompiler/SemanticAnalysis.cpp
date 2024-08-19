#pragma execution_character_set("utf-8") 

#include "SynSemAnalysis.h"

using namespace std;


//���嶯�����ú���
//˼·������id˳�����
void SemanticAnalysis::SemanticAction(ProduceForms* ResoProduce)
{
    switch (ResoProduce->id)
    {
        case 0:
            break;
        case 1: //[ <Program>--><N> <������>  ]
            Program();
            break;
        case 2: //[ <N>-->��  ]
            ProN();
            break;
        case 3: //[ <������>--><����>  ]
        case 4: //[ <������>--><����> <������>  ]
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 5://[ <����>-->int ID <M> <A> <��������>  ]
        case 6://[ <����>-->void ID <M> <A> <��������>  ]
        case 7://[ <����>-->float ID <M> <A> <��������>  ]
            Statement();
            break;
        case 8://[ <M>-->��  ]
            ProM();
            break;
        case 9://[ <A>-->��  ]
            ProA();
            break;
        case 10://[ <��������>--><��������>  ]
        case 11://[ <��������>--><��������>  ]
            StateTypeF();
            break;
        case 12://[ <��������>-->;  ]
            VarStatement();
            break;
        case 13://[ <��������>-->( <�β�> ) <����>  ]
            FuncStatement();
            break;
        case 14://[ <�β�>--><�����б�>  ]
        case 15://[ <�β�>-->void  ]
            FormalParameters();
            break;
        case 16://[ <�����б�>--><����> <���źͲ���>  ]
            ParametersList();
            break;
        case 17://[ <���źͲ���>-->, <����> <���źͲ���>  ]
            CommaParameter1();
            break;
        case 18://[ <���źͲ���>-->��  ]
            CommaParameter2();
            break;
        case 19://[ <����>-->int ID  ]
        case 20://[ <����>-->float ID  ]
            Parameter();
            break;
        case 21://[ <����>-->{ <�ڲ�����> <��䴮> }  ]
            StatementBlock();
            break;
        case 22://[ <�ڲ�����>-->��  ]
            PopPush(0);
            break;
        case 23://[ <�ڲ�����>--><�ڲ���������> ; <�ڲ�����>  ]
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 24://[ <�ڲ���������>-->int ID  ]
        case 25://[ <�ڲ���������>-->float ID  ]
            InnerVarState();
            break;
        case 26://[ <��䴮>--><���>  ]
            SentenceList1();
            break;
        case 27://[ <��䴮>--><���> <M> <��䴮>  ]
            SentenceList2();
            break;
        case 28://[ <���>--><if���>  ]
        case 29://[ <���>--><while���>  ]
        case 30://[ <���>--><return���>  ]
        case 31://[ <���>--><��ֵ���>  ]
            Sentence();
            break;
        case 32://[ <��ֵ���>-->ID = <���ʽ> ;  ]
            AssignMent();
            break;
        case 33://[ <return���>-->return <���ʽ> ;  ]
            Return1();
            break;
        case 34://[ <return���>-->return ;  ]
            Return2();
            break;
        case 35://[ <while���>-->while <M> ( <���ʽ> ) <A> <����>  ]
            While();
            break;
        case 36://[ <if���>-->if ( <���ʽ> ) <A> <����>  ]
            If1();
            break;
        case 37://[ <if���>-->if ( <���ʽ> ) <A> <����> <N> else <M> <A> <����>  ]
            If2();
            break;
        case 38://[ <���ʽ>--><�ӷ����ʽ>  ]
            Expression_1();
            break;
        case 39://[ <���ʽ>--><�ӷ����ʽ> <relop> <�ӷ����ʽ>  ]
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
        case 46://[ <�ӷ����ʽ>--><��>  ]
            AddExpr_1();
            break;
        case 47://[ <�ӷ����ʽ>--><��> + <�ӷ����ʽ>  ]
        case 48://[ <�ӷ����ʽ>--><��> - <�ӷ����ʽ>  ]
            AddExpr_23();
            break;
        case 49://[ <��>--><����>  ]
            Item_1();
            break;
        case 50://[ <��>--><����> * <��>  ]
        case 51://[ <��>--><����> / <��>  ]
            Item_23();
            break;
        case 52://[ <����>-->num  ]
            Factor_1();
            break;
        case 53://[ <����>-->( <���ʽ> )  ]
            Factor_2();
            break;
        case 54://[ <����>-->ID  ]
            Factor_3();
            break;
        case 55://[ <����>-->ID ( <ʵ���б�> )  ]
            Factor_4();
            break;
        case 56://[ <ʵ���б�>--><���ʽ>  ]
            ArgumentList_1();
            break;
        case 57://[ <ʵ���б�>--><���ʽ> , <ʵ���б�>  ]
            ArgumentList_2();
            break;
        case 58://[ <ʵ���б�>-->��  ]
            ArgumentList_3();
            break;
        case 59://<�ڲ�����> ::= <�ڲ���������> ; <�ڲ�����>
            PopPush((int)ResoProduce->RightSign.size());
            break;
        case 60://<�ڲ���������> ::= int ID <����>
            ArrayStatement();
            break;
        case 61://<����> ::= [ num ]
            ArrayFactor1();
            break;
        case 62://<����> ::= [ num ] <����>
            ArrayFactor2();
            break;
        case 63://<��ֵ���> ::= ID <����> = <���ʽ> ;
            AssignMent1();
            break;
        case 64://<����> ::= ID <����>
            Factor_5();
            break;

        default:
            break;
    }
}


//���Һ����� û�з���-1
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
//<Program> :: = <N> <������>
//$���� ��main�ĵ�ַ���N��nextlist
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
//<N> :: = ��
//$���� N.nextlist = { nextquad }
//$���� emit(j, _, _, -1)
void SemanticAnalysis::ProN()
{
    SemanticTreeNode N;
    N.Name = SemanticLeftSign.str;
    N.NextList.push_back(MiddleCodeTable.size());
    MiddleCodeTable.push_back(Code{ "j","-" ,"-","-1" });
    SemanticStack.push(N);
}
//���˳�ջ ��ջ û������������
void SemanticAnalysis::PopPush(int len)
{
    SemanticTreeNode NothingToDo;
    NothingToDo.Name = SemanticLeftSign.str;
    GetStorePop(len);
    SemanticStack.push(NothingToDo);
}
//<����> :: = int  ID <M> <A> <��������> | void  ID <M> <A>  <��������> | float  ID <M> <A> <��������>
//$���� ������������ ����Ǳ������� �˴��γ�һ�������ı������� �������� ���� ��������Σ����������
//$���� ������������ ����Ǻ������� �˴��γ�һ�������ĺ������� �������� ���� ������ ������ַ���뺯����
//$���� ���ں������� �˴��γ�һ�������ĺ������� �������� ���� ������ ������ַ���뺯����
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
    //����
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
        //����ܴ�
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
//<A> ::= ��
void SemanticAnalysis::ProA()
{
    SemanticTreeNode A;
    A.Name = SemanticLeftSign.str;
    ProcNo++;
    ProcNoStack.push_back(ProcNo);
    SemanticStack.push(A);
}
//<��������>:: = <��������> | <��������>
//$���� ���ڱ������� ��¼��ǰ��������Ϊ����
//$���� ���ں������� ֱ�Ӵ��ݲ����������
void SemanticAnalysis::StateTypeF()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode State = StorePop[0];
    State.Name = SemanticLeftSign.str;
    SemanticStack.push(State);
}

//<��������> :: = ;
//$����  �˴�����Ϊ���� ֮���Լ��������
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
//<��������> :: = (<�β�>) <����>
//$���� �������β��б���Ϊ<�β�>���β��б� ����Ϊ��������
void SemanticAnalysis::FuncStatement()
{
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode List = StorePop[2];
    List.Name = SemanticLeftSign.str;
    List.Namekind = Proc;

    SemanticStack.push(List);
}
//<�β�>:: = <�����б�> | void
//$���� ���ڲ����б� �������β��б���Ϊ<�����б�>���β��б�
//$���� ����void �������β��б���Ϊ��
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
//<�����б�>  :: = <����> <���źͲ���>
//$���� �����Ĳ����б���Ϊ�Ҳ�Ĳ����ĺϲ� �����������͵���Ϣ
void SemanticAnalysis::ParametersList()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode fp = StorePop[1];
    fp.Name = SemanticLeftSign.str;
    fp.param.insert(fp.param.end(), StorePop[0].param.begin(), StorePop[0].param.end());//��������ϲ�
    SemanticStack.push(fp);
}
//<���źͲ���> ::= , <����> <���źͲ���>
//$���� �����Ĳ����б���Ϊ�Ҳ�Ĳ����ĺϲ� �����������͵���Ϣ
void SemanticAnalysis::CommaParameter1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode fp = StorePop[1];
    fp.Name = SemanticLeftSign.str;
    fp.param.insert(fp.param.end(), StorePop[0].param.begin(), StorePop[0].param.end());//��������ϲ�
    SemanticStack.push(fp);
}
//<���źͲ���> ::= ��
//$���� �� ѹջ��ջ����
void SemanticAnalysis::CommaParameter2()
{
    SemanticTreeNode fp;
    fp.Name = SemanticLeftSign.str;
    SemanticStack.push(fp);
}
//<����> :: = int  ID | float  ID
//$���� �������β��б���Ϊһ������ ����Ϊ int | float ��Ϊ ID
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
//<����> :: = { <�ڲ�����>  <��䴮> }
//$���� nowLevel--
//$���� <����>nextlist = <��䴮>nextlist
void SemanticAnalysis::StatementBlock()
{
    ProcNoStack.pop_back();
    int RightLen = 4;
    GetStorePop(RightLen);
    SemanticTreeNode statement_block = StorePop[1];
    statement_block.Name = SemanticLeftSign.str;
    SemanticStack.push(statement_block);
}

//<�ڲ���������>:: = int  ID | float  ID
//$���� �˴��γ�һ�������ı������� �������� ���� ��������Σ����������
void SemanticAnalysis::InnerVarState()
{
    int RightLen = 2;
    GetStorePop(RightLen);
    SemanticTreeNode InnerVar;
    InnerVar.Name = SemanticLeftSign.str;
    SemanticTreeNode& Type = StorePop[1];
    SemanticTreeNode& ID = StorePop[0];
    //����ܴ�
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
//<��䴮> :: = <���> | <���> <M> <��䴮>
//$���� <���> ���Ҳ� nextList ���ݸ����
//$���� <���> <M> <��䴮> ���Ҳ���䴮 nextList ���ݸ���� ��M�������nextlist
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
//<���> ::= <if���> |<while���> | <return���> | <��ֵ���>
void SemanticAnalysis::Sentence()
{
    int RightLen = 1;
    GetStorePop(RightLen);
    SemanticTreeNode s = StorePop[0];
    s.Name = SemanticLeftSign.str;
    SemanticStack.push(s);
}
//<��ֵ���> ::=  ID = <���ʽ> ;
//$���� ���ɸ�ֵ��Ԫʽ
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
//<return���> :: = return <���ʽ> ; | return ;
//$���� ���ɷ�����Ԫʽ
void SemanticAnalysis::Return1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode re;
    re.Name = SemanticLeftSign.str;
    SemanticTreeNode& Exp = StorePop[1];
    ReType = FLOAT1;
    //���Կ��Ǹ�ֵreturnplace
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
    //���Կ��Ǹ�ֵreturnplace
    MiddleCodeTable.push_back({ "return","-","-","-" });
    SemanticStack.push(re);
}
//<while���> ::= while <M> ( <���ʽ> ) <A> <����>
//$���� M.quad ������� nextList
//$���� ��� nextList = ���ʽ falselist
//$���� ���� jump ��Ԫʽ
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
//<if���> ::= if  ( <���ʽ> )  <A> <����>  |if  ( <���ʽ> ) <A>  <����> <N> else <M> <A> <����>
//$���� ��һ������ʽ
//$���� ���ʽ falseList  ���� ���� nextList
//$���� If ��� nextList ����Ϊ���ʽ falseList
//$���� ��2������ʽǰ��һ��
//$���� M.quad ������ʽ falseList
//$���� Merge nextList ��Ϊ if ���� nextList
void SemanticAnalysis::If1()
{
    int RightLen = 6;
    GetStorePop(RightLen);
    SemanticTreeNode& Exp = StorePop[3];
    SemanticTreeNode& S_block = StorePop[0];
    SemanticTreeNode if_sentence;
    if_sentence.Name = SemanticLeftSign.str;
    //MERGE
    Exp.FalseList.insert(Exp.FalseList.end(), S_block.NextList.begin(), S_block.NextList.end());//��list�ϲ�
    if_sentence.NextList = Exp.FalseList;
    SemanticStack.push(if_sentence);
}
//<if���> ::= if  ( <���ʽ> )  <A> <����>  |if  ( <���ʽ> ) <A>  <����> <N> else <M> <A> <����>
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
//����
void SemanticAnalysis::BackPatch(vector<int>& BackList, int Addr)
{
    for (auto back : BackList) {
        MiddleCodeTable[back].linkres = to_string(Addr);
    }
}
//�������
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


//�ڵ�������ĺ�����ʱ�򣬲���ʽ���Ҳ��Ѿ�pop�������Ҵ�����StorePop������

void SemanticAnalysis::GetStorePop(int len)
{
    StorePop.clear();
    for (int i = 0; i < len; i++)
    {
        StorePop.push_back(SemanticStack.top());
        SemanticStack.pop();
    }
}
////����Ԫ��
//struct Var {
//	string name;//���ֱ�ʶ��
//	NameTableType type;//���ֵ�����
//	bool normal;//trueΪ���βΣ�falseΪ�β�
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
    //��麯�����Ƿ��Ӧ
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
    //�����������Ƿ�һ��
    if (FuncTable[FuncIndex].param.size() != Factor.content[2].param.size())
    {
        ErrorFuncName = Factor.content[0].Name;
        semanticerror = FuncParLenError;
    }
    for (int i = (int)Factor.content[2].param.size() - 1; i >= 0; i--)//���ɲ���ѹջ�м����
    {
        string VFactorName = GetMiddleName(Factor.content[2].param[i].name, Factor.content[2].param[i].dims);
        MiddleCodeTable.push_back(Code{ "Par",VFactorName ,"-","-" });
    }
    string Temp = MallocVar();//�����м�����������
    MiddleCodeTable.push_back(Code{ "=","@BackReturn","-",GetMiddleName(Temp) });

    MiddleCodeTable.push_back(Code{ "call",GetMiddleName(Factor.content[0].Name),"-","-" });

    Factor.Name = Temp;
    Factor.type = NonTerminator;
    Factor.normal = true;
    Factor.IsNull = 0;
    //    Factor.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(Factor);
}

//<����> ::= ID <����>
//$����: ����±��Ƿ���ȷ
//$����: �����������Ƿ����
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
    ArgumentList.Name = "<ʵ���б�>";
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

    ArgumentList.Name = "<ʵ���б�>";
    ArgumentList.type = NonTerminator;
    ArgumentList.normal = true;
    ArgumentList.IsNull = 0;
    //ArgumentList.content.push_back(Expr);//�޹ؽ�Ҫ
    ArgumentList.param = Parlist.param;
    ArgumentList.param.push_back(Par{ FLOAT1,Expr.Name,Expr.dems });
    //    ArgumentList.NextList.push_back((int)MiddleCodeTable.size());
    SemanticStack.push(ArgumentList);
}


void SemanticAnalysis::ArgumentList_3()
{
    SemanticTreeNode ArgumentList;
    ArgumentList.Name = "<ʵ���б�>";
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
    Item.Name = MallocVar();//�����м����
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
    AddExpr.Name = MallocVar();//�����м����
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
//<����> ::= [ num ]
//$����: ��Ҫ��¼num��ֵ
void SemanticAnalysis::ArrayFactor1()
{
    int RightLen = 3;
    GetStorePop(RightLen);
    SemanticTreeNode ArrayFactor;
    SemanticTreeNode& dimennum = StorePop[1];
    ArrayFactor.dems.push_back(dimennum.ival);
    SemanticStack.push(ArrayFactor);
}

//<����> ::= [ num ] <����>
//$����: ��Ҫ��¼num��ֵ
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
//<�ڲ���������> ::= int ID <����>
//$����: ��¼���������������ͱ�����ά�ȣ���������ƣ��Լ��������ڵĹ���λ��
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

//<��ֵ���> ::= ID <����> = <���ʽ> ;
//$����: ����±��Ƿ���ȷ
//$����: �����������Ƿ����
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
        string VIDNo = VID.ProcNo != -1 ? to_string(VID.ProcNo) : "";//��Ҫ�����м�������е�ÿһ������
        ID.Name = ID.Name + " " + VIDNo;
        for (int i = 0; i<int(ArrayExpr.dems.size()); i++)
        {
            ID.Name = ID.Name + " " + to_string(ArrayExpr.dems[i]);
        }
        MiddleCodeTable.push_back(Code{ "=",GetMiddleName(Exp.Name,Exp.dems) ,"-",ID.Name });
    }
    SemanticStack.push(AssignState);
}






//������һЩ�ļ��������(debug)
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
    fvar<< "============================������=============================" << endl;
    for (int i = 0; i < (int)this->VarTable.size(); i++)
        fvar<< VarTable[i] << endl;
    fvar << "==============================================================" << endl;
    fvar.close();
}

void SemanticAnalysis::PrintFuncTable()
{
    ofstream ffunc(FUNC_TABLE_FILE, ios::out);
    ffunc << "============================������=============================" << endl;
    for (int i = 0; i < (int)FuncTable.size(); i++)
        ffunc << FuncTable[i] << endl;
    ffunc<< "==============================================================" << endl;
    ffunc.close();
}


void SemanticAnalysis::PrintCodeTable()
{
    ofstream fcode(QUATERNION_FILE, ios::out);
    fcode << "============================�м�����=============================" << endl;
    for (int i = 0; i < (int)MiddleCodeTable.size(); i++)
    {
       fcode<< i << ":";
       fcode<< "(";
       fcode << MiddleCodeTable[i] << ")" << endl;
    }
    fcode<< "==============================================================" << endl;
    fcode.close();
}

