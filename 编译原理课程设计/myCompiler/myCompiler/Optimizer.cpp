#pragma execution_character_set("utf-8") 
#include "Optimizer.h"
using namespace std;

int Nodelen;//Ŀǰ�ж��ٸ����
vector<int> DAGEdges[maxn];//DAG�ı�
DAGNode DAGNodes[maxn];//DAG�ڵ㼯
vector<int> DAGRoots;//DAGͼ�ĸ����
int indegree[maxn];//DAG�������
int CutVis[maxn];//��֦ʱ�������


//�ж��ַ����Ƿ�Ϊ�����������жϸ�ʽ�������Ƿ�Χ��
bool isFloat(const char* str)
{
    bool isE = false,
        isPoint = false,
        numBefore = false,
        numBehind = false;

    int index = 0;
    for (; *str != '\0'; str++, index++)
    {
        switch (*str)
        {
            case '0':case'1':case'2':case'3':case'4':case'5':
            case'6':case'7':case'8':case'9':
                if (isE)
                {
                    numBehind = true;
                }
                else
                {
                    numBefore = true;
                }
                break;
            case '+':case '-':
                if (index != 0)
                {
                    return false;
                }
                break;
            case 'e':case 'E':
                if (isE || !numBefore)
                {
                    return false;
                }
                else
                {
                    isPoint = true;
                    index = -1;
                    isE = true;
                }
                break;
            case '.':
                if (isPoint)
                {
                    return false;
                }
                else
                {
                    isPoint = true;
                }
                break;
            default:
                return false;
        }
    }

    if (!numBefore)
    {
        return false;
    }
    else if (isE && !numBehind)
    {
        return false;
    }

    return true;
}

//atof
double atof(string a)
{
    double temp;
    int error = sscanf(a.c_str(), "%lf", &temp);
    return temp;
}

//ftoa
string ftoa(double f)
{
    char temp[100] = { 0 };
    sprintf(temp, "%g", f);
    string res = temp;
    return res;
}



Optimizer::Optimizer(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1)
{
    this->VarTable = VarTable1;
    this->FuncTable = FuncTable1;
    this->MiddleCodeTable = MiddleCodeTable1;
    memset(CutVis, 0x0, sizeof(CutVis));
    memset(indegree, 0x0, sizeof(indegree));
    Divblocks();
    UpdateJumpTar();//�޸�Jump��Ŀ���ַΪ������ı��

    

    for (int i = 0; i < (int)CodeBlock.size(); i++)
    {
        if (i == (int)CodeBlock.size() - 1)//main����
        {
            for (int j = 0; j < (int)this->CodeBlock[i].varlist.size(); j++) {
                bool infunc = 0;
                for (int k = 0; k < (int)this->FuncTable.size(); k++) {
                    if (this->CodeBlock[i].varlist[j].name == this->FuncTable[k].name) {
                        infunc = 1;
                    }
                }
                if (this->CodeBlock[i].varlist[j].name[0] != '@' && this->CodeBlock[i].varlist[j].name[0] != 'T' && infunc == 0) {
                    OVar temp;
                    temp.name = this->CodeBlock[i].varlist[j].name;
                    CodeBlock[i].Outvarlist.push_back(temp);
                }
            }
           
        }

        GenOCode(CodeBlock[i]);
    }
    int begin_index = 0;
    for (int i = 0; i < (int)CodeBlock.size(); i++)
    {
        CodeBlock[i].BeginIndex = begin_index;
        begin_index += CodeBlock[i].Ocode.size();
    }

    for (int i = 0; i < (int)CodeBlock.size(); i++)
    {
        for (int j = 0; j < (int)CodeBlock[i].Ocode.size(); j++)
        {
            if (CodeBlock[i].Ocode[j].op[0] == 'j')//�������תָ��
            {
                int jumpblockid = atoi(CodeBlock[i].Ocode[j].linkres.c_str());
                CodeBlock[i].Ocode[j].linkres = to_string(CodeBlock[jumpblockid].BeginIndex);//��ԭjumpָ��ĵ�ַ
            }
            OptimCodeTable.push_back(CodeBlock[i].Ocode[j]);
        }
    }


     for (int i = 0; i < (int)this->CodeBlock.size(); i++)
    {
        cout << this->CodeBlock[i].id << endl;
        for (int j = 0; j < (int)this->CodeBlock[i].code.size(); j++) {
            cout << this->CodeBlock[i].code[j].op << ' ' << this->CodeBlock[i].code[j].op1 <<
                ' ' << this->CodeBlock[i].code[j].op2 << ' ' << this->CodeBlock[i].code[j].linkres << endl;
        }
        cout << endl;
        for (int j = 0; j < (int)this->CodeBlock[i].varlist.size(); j++) {
            cout << this->CodeBlock[i].varlist[j].name << endl;
        }
        cout << endl;
        for (int j = 0; j < (int)this->CodeBlock[i].Ocode.size(); j++) {
            cout << this->CodeBlock[i].Ocode[j].op << ' ' << this->CodeBlock[i].Ocode[j].op1 <<
                ' ' << this->CodeBlock[i].Ocode[j].op2 << ' ' << this->CodeBlock[i].Ocode[j].linkres << endl;
        }
        cout << endl;
        for (int j = 0; j < (int)this->CodeBlock[i].Outvarlist.size(); j++) {
            cout << this->CodeBlock[i].Outvarlist[j].name << endl;
        }
        cout << "______________________________"<<endl;
    }

    
    //    cout<<"OptimCodeTable.size():"<<OptimCodeTable.size()<<endl;
}

void Optimizer::addOVar(CODE_BLOCK& CurBlock, string name, Pos pos)
{
    if (name != "-" && !isFloat(name.c_str()))
    {
        bool isfind = false;
        for (int k = 0; k < (int)CurBlock.varlist.size(); k++)
        {
            if (CurBlock.varlist[k].name == name)//����Ѿ��ڱ���������
            {

                CurBlock.varlist[k].pos.push_back(pos);
                isfind = true;
                break;
            }
        }
        if (!isfind)
        {
            OVar temp;
            temp.name = name;
            temp.pos.push_back(pos);
            CurBlock.varlist.push_back(temp);
        }
    }
}

void Optimizer::FindOutVar(int find_id, int cur_id)
{
    cout << find_id <<"**"<< endl;
    CODE_BLOCK& find_cb = CodeBlock[find_id];
    CODE_BLOCK& cur_cb = CodeBlock[cur_id];
    if (find_id != cur_id)
    {
        for (int j = 0; j < (int)find_cb.varlist.size(); j++)//Ѱ����ͬ����
        {
          
            int flag = 0;
            for (int k = 0; k < (int)cur_cb.varlist.size(); k++)
            {
                if (find_cb.varlist[j].name == cur_cb.varlist[k].name)//�ҵ��˱���
                {
                    find_cb.Outvarlist.push_back(find_cb.varlist[j]);
                    flag = 1;
                    break;
                }
            }
            //��ֹ�ظ�����
            if (flag == 0) {
             
                //�ж����飬����Ԫ�����ǻ�Ծ����������ȥ��
                int cnt = 0;
                for (char ch : find_cb.varlist[j].name) {
                    if (ch == ' ') {
                        cnt++;
                        if (cnt == 2) {
                            break;
                        }
                    }
                }
                if (cnt == 2) {
                    //cout << find_cb.varlist[j].name << endl;
                    find_cb.Outvarlist.push_back(find_cb.varlist[j]);
                }
            }
          
        }

    }
    for (int i = 0; i < (int)CodeBlock[cur_id].NextBlock.size(); i++)//�������е�nextblock
    {
        CODE_BLOCK& next_cb = *CodeBlock[cur_id].NextBlock[i];
        if (next_cb.vis == 1)//����Ѿ�����������Ҫ��������
            continue;
        //�ƻ�ȥ������
        if (next_cb.id == find_id) {
            for (int j = 0; j < (int)find_cb.varlist.size(); j++)//Ѱ����ͬ����
            {
                find_cb.Outvarlist.push_back(find_cb.varlist[j]);
            }
            return;
        }
        next_cb.vis = 1;//�ñ��λ���������Ի��������
        FindOutVar(find_id, next_cb.id);//�ݹ���ã�һֱ������
    }
    ////����main���������б������ǻ�Ծ��Ϣ������¼�����ս��
    //if ((int)CodeBlock[cur_id].NextBlock.size() == 0) {
    //    cout << "&&"<<cur_id << endl;
    //    for (int j = 0; j < (int)find_cb.varlist.size(); j++) {
    //      find_cb.Outvarlist.push_back(find_cb.varlist[j]);
    //    }
    //}
}

void Optimizer::Divblocks()
{
    vector<int> EntryEentence;//�����伯��
    bool is_last_jump = false;
    bool is_last_return = false;//ǰһ������Ƿ�Ϊreturn
    for (int i = 0; i < (int)MiddleCodeTable.size(); i++)
    {
        if (i == 0)//�������
        {
            EntryEentence.push_back(i);
        }
        if (is_last_jump || is_last_return)//��ת���֮��ĵ�һ�����
        {
            int is_find = 0;
            for (int j = 0; j < (int)EntryEentence.size(); j++)
                if (EntryEentence[j] == i)
                {
                    is_find = 1;
                    break;
                }
            if (!is_find)
                EntryEentence.push_back(i);
        }
        if (MiddleCodeTable[i].op == "j" || MiddleCodeTable[i].op == "j<" || MiddleCodeTable[i].op == "j<=" || MiddleCodeTable[i].op == "j>"
            || MiddleCodeTable[i].op == "j>=" || MiddleCodeTable[i].op == "j==" || MiddleCodeTable[i].op == "j!=")
        {
            is_last_jump = true;
            int is_find = 0;
            for (int j = 0; j < (int)EntryEentence.size(); j++)
                if (EntryEentence[j] == atoi(MiddleCodeTable[i].linkres.c_str()))
                {
                    is_find = 1;
                    break;
                }
            if (!is_find)
                EntryEentence.push_back(atoi(MiddleCodeTable[i].linkres.c_str()));//��ת��Ŀ�����
        }
        else
            is_last_jump = false;
        if (MiddleCodeTable[i].op == "return")
            is_last_return = true;
        else
            is_last_return = false;
    }
    EntryEentence.push_back(MiddleCodeTable.size());
    sort(EntryEentence.begin(), EntryEentence.end());//����������
    for (int i = 0; i < (int)EntryEentence.size() - 1; i++)
    {
        CODE_BLOCK ThisBlock;
        ThisBlock.BeginIndex = EntryEentence[i];
        int curi = ThisBlock.BeginIndex;
        while (curi < min(EntryEentence[i + 1], (int)MiddleCodeTable.size()))
        {
            ThisBlock.code.push_back(MiddleCodeTable[curi]);
            curi++;
        }
        ThisBlock.id = (int)CodeBlock.size();
        CodeBlock.push_back(ThisBlock);
    }
    for (int i = 0; i < (int)CodeBlock.size(); i++)//����ÿһ���������Ѱ�ұ���
    {
        CODE_BLOCK& CurBlock = CodeBlock[i];
        for (int j = 0; j < (int)CurBlock.code.size(); j++)//Ѱ�ұ���
        {
            Pos pos;
            string name;
            pos.lno = i; pos.ipos = lvar; name = CurBlock.code[j].linkres;
            addOVar(CurBlock, name, pos);
            pos.lno = i; pos.ipos = opl; name = CurBlock.code[j].op1;
            addOVar(CurBlock, name, pos);
            pos.lno = i; pos.ipos = opr; name = CurBlock.code[j].op2;
            addOVar(CurBlock, name, pos);
        }
        Code lastcode = CurBlock.code[CurBlock.code.size() - 1];
        if (lastcode.op == "j" || lastcode.op == "j<" || lastcode.op == "j<=" || lastcode.op == "j>"
            || lastcode.op == "j>=" || lastcode.op == "j==" || lastcode.op == "j!=")//��ת
        {
            int nextfirstlno = atoi(lastcode.linkres.c_str());
            for (int j = 0; j < (int)CodeBlock.size(); j++)//Ѱ�����ӵ���һ�������
            {
                if (nextfirstlno == CodeBlock[j].BeginIndex)
                {
                    CurBlock.NextBlock.push_back(&CodeBlock[j]);//��ת����һ�������
                    break;
                }
            }
        }
        if (lastcode.op != "j" && i < (int)CodeBlock.size() - 1)
            CurBlock.NextBlock.push_back(&CodeBlock[i + 1]);//��������һ�������
    }
   
    for (int i = 0; i < (int)CodeBlock.size(); i++)//Ѱ��ÿһ�������ڿ��ⱻ���õı���
    {
        for (int j = 0; j < (int)CodeBlock.size(); j++)//���vis
        {
            CodeBlock[j].vis = 0;
        }
        
        FindOutVar(CodeBlock[i].id, CodeBlock[i].id);

        //�����return��䣬return�ı������ڻ�����֮��ʹ��
        if (CodeBlock[i].code[CodeBlock[i].code.size() - 1].op == "return")
        {
            OVar temp;
            temp.name = CodeBlock[i].code[CodeBlock[i].code.size() - 1].op1;
            if (temp.name != "-")
                CodeBlock[i].Outvarlist.push_back(temp);
        }
        //ȥ��
        vector<OVar> temp;
        for (int j = 0; j < (int)CodeBlock[i].Outvarlist.size(); j++)
        {
            int is_find = 0;
            for (int k = 0; k < (int)temp.size(); k++)
            {
                if (temp[k].name == CodeBlock[i].Outvarlist[j].name)
                {
                    is_find = 1;
                    break;
                }
            }
            if (!is_find)
                temp.push_back(CodeBlock[i].Outvarlist[j]);
        }
        CodeBlock[i].Outvarlist = temp;
    }
    return;
}
void Optimizer::UpdateJumpTar()
{
    for (int i = 0; i < (int)CodeBlock.size(); i++)
    {
        if (CodeBlock[i].code[CodeBlock[i].code.size() - 1].op[0] == 'j')//˵������תָ��
        {
            int jumptar = atoi(CodeBlock[i].code[CodeBlock[i].code.size() - 1].linkres.c_str());
            for (int j = 0; j < (int)CodeBlock.size(); j++)
            {
                if (CodeBlock[j].BeginIndex == jumptar)
                    CodeBlock[i].code[CodeBlock[i].code.size() - 1].linkres = to_string(j);//�ĳɻ�����ı��
            }
        }
    }
}

void Optimizer::DeleteVar(string val1, int id)
{
    int findi = 0;
    for (int i = 0; i < (int)DAGNodes[id].Dval.size(); i++)
    {
        if (DAGNodes[id].Dval[i].name == val1)//ֻ�з�Ҷ�ӽ��ſ���ɾ�����
        {
            findi = i;
            break;
        }
    }
    DAGNodes[id].Dval.erase(DAGNodes[id].Dval.begin() + findi);
}


int Optimizer::LeftIsDefine(string node1)
{
    for (int i = 0; i < Nodelen; i++)
    {
        for (int j = 0; j < (int)DAGNodes[i].Dval.size(); j++)
        {
            if (node1 == DAGNodes[i].Dval[j].name)
                return DAGNodes[i].id;
        }
    }
    return -1;
}

int Optimizer::BottomIsDefine(string var)
{
    for (int i = Nodelen - 1; i >= 0; i--)
    {
        if (var == DAGNodes[i].BDval.name || (isFloat(var.c_str()) && DAGNodes[i].Btype == 1 && atof(var) == DAGNodes[i].BCval))
        {
            //            cout<<"var:"<<var<<" DAGNodes[i].BDval.name:"<<DAGNodes[i].BDval.name<<" DAGNodes[i].BCval:"<<DAGNodes[i].BCval<<endl;
            return DAGNodes[i].id;
        }
        for (int j = 0; j < (int)DAGNodes[i].Dval.size(); j++)
        {
            if (DAGNodes[i].Dval[j].name == var)
            {
                //                cout<<"var:"<<var<<" DAGNodes["<<i<<"].Dval["<<j<<"].name:"<<DAGNodes[i].Dval[j].name<<endl;
                return DAGNodes[i].id;
            }
        }
    }
    return -1;
}

int Optimizer::FindSubB(string B)
{
    for (int i = 0; i < Nodelen; i++)
    {
        if ((int)DAGEdges[i].size() == 1)
        {
            int to = DAGEdges[i][0];
            if (DAGNodes[to].Btype == 0 && DAGNodes[to].BDval.name == B)
            {
                return i;
            }
        }
    }
    return -1;
}

int Optimizer::FindSubBC(string B, string C, string op)
{
    for (int i = 0; i < Nodelen; i++)
    {
        if ((int)DAGEdges[i].size() == 2)
        {
            DAGNode lto = DAGNodes[DAGEdges[i][0]];//����
            DAGNode rto = DAGNodes[DAGEdges[i][1]];//�Һ��
            int findB = 0;
            int findC = 0;
            int findop = 0;
            if (isFloat(B.c_str()) && !isFloat(C.c_str()))//�ǳ���
            {
                if (atof(B) == lto.BCval)
                    findB = 1;
                for (int j = 0; j < (int)rto.Dval.size(); j++)
                    if (rto.Dval[j].name == C)
                    {
                        findC = 1;
                        break;
                    }
            }
            else if (!isFloat(B.c_str()) && isFloat(C.c_str()))
            {
                for (int j = 0; j < (int)lto.Dval.size(); j++)
                    if (lto.Dval[j].name == B)
                    {
                        findB = 1;
                        break;
                    }
                if (atof(C) == rto.BCval)
                    findC = 1;
            }
            else if (!isFloat(B.c_str()) && !isFloat(C.c_str()))
            {
                for (int j = 0; j < (int)lto.Dval.size(); j++)
                    if (lto.Dval[j].name == B)
                    {
                        findB = 1;
                        break;
                    }
                for (int j = 0; j < (int)rto.Dval.size(); j++)
                    if (rto.Dval[j].name == C)
                    {
                        findC = 1;
                        break;
                    }
            }
            if (op == DAGNodes[i].op)
                findop = 1;
            if (findB == 1 && findC == 1 && findop == 1)
                return i;
        }
    }
    return -1;
}


int Optimizer::AllocDAGNode(DAGNode* Node, string str)
{

    int Sid = BottomIsDefine(str);
    if (Sid == -1)
    {
        Node = new DAGNode;
        Node->id = Nodelen;
        if (isFloat(str.c_str()))
        {
            Node->Btype = 1;
            Node->BCval = atof(str);
        }
        else
        {
            Node->Btype = 0;
            Node->BDval.name = str;
        }
        indegree[Nodelen] = 0;
        DAGNodes[Nodelen++] = *Node;
        //        cout<<"Nodelen:"<<Nodelen<<endl;
        delete Node;
        Sid = Nodelen - 1;
    }
    return Sid;
}


void Optimizer::CreateDAG(CODE_BLOCK& CodeBlock)
{
    Nodelen = 0;
    for (int i = 0; i < (int)CodeBlock.code.size(); i++)//���ڻ������е�ÿ�����
    {
        //׼�����������
        int optype = OpType(CodeBlock.code[i]);
        if (optype == 0)
        {
            DAGNode* cur_node;
            string A = CodeBlock.code[i].linkres;
            string B = CodeBlock.code[i].op1;
            string op = CodeBlock.code[i].op;
            if (op == "j")//��תָ��,ֱ�Ӵ�����㼴�ɣ���Ϊ�������������Ż�
            {
                cur_node = new DAGNode;
                cur_node->type = DAGJ;
                cur_node->Btype = 2;
                cur_node->id = Nodelen++;
                //                cout<<"2 Nodelen:"<<Nodelen<<endl;
                cur_node->Cval = atof(A);
                cur_node->op = "j";
                indegree[Nodelen - 1] = 0;
                DAGNodes[Nodelen - 1] = *cur_node;
                delete cur_node;
            }
            else if (op == "=")//��ֵָ���Ҫ�����㷨������
            {
                int Bid = BottomIsDefine(B);//Ѱ���²��B�ڵ��Ƿ����
                if (Bid == -1)
                {
                    //������
                    cur_node = new DAGNode;
                    cur_node->type = DAGA;
                    cur_node->id = Nodelen++;
                    //                    cout<<"3 Nodelen:"<<Nodelen<<endl;
                    if (isFloat(B.c_str()))//���������
                    {
                        cur_node->Btype = 1;
                        cur_node->BCval = atof(B);
                    }
                    else
                    {
                        cur_node->Btype = 0;
                        cur_node->BDval.name = B;
                    }
                    cur_node->op = "=";
                    int Aid = LeftIsDefine(A);
                    if (Aid != -1)//�ҵ���
                        DeleteVar(A, Aid);//ɾ�����
                    //Ȼ�����µĽ��
                    OVar Anode;
                    Anode.name = A;
                    cur_node->Dval.push_back(Anode);//��A����Node(B)���Ҳ�
                    indegree[Nodelen - 1] = 0;
                    DAGNodes[Nodelen - 1] = *cur_node;
                    delete cur_node;
                }
                else//B������
                {
                    int Aid = LeftIsDefine(A);
                    if (Aid != -1)//�ҵ���
                        DeleteVar(A, Aid);//ɾ�����
                    //Ȼ�����µĽ��
                    OVar Anode;
                    Anode.name = A;
                    DAGNodes[Bid].Dval.push_back(Anode);//��A����Node(B)���Ҳ�
                }
                //���Ĳ�
            }
        }
        else if (optype == 1)//1�Ͳ���
        {
            DAGNode* cur_nodeA = NULL;
            DAGNode* cur_nodeB = NULL;
            string A = CodeBlock.code[i].linkres;
            string B = CodeBlock.code[i].op1;
            string C = CodeBlock.code[i].op2;
            string op = CodeBlock.code[i].op;
            int Aid = LeftIsDefine(A);
            if (Aid != -1)//�ҵ���
                DeleteVar(A, Aid);//ɾ�����
            int Bid = BottomIsDefine(B);//Ѱ���²��B�ڵ��Ƿ����
            if (Bid == -1)
            {
                if (isFloat(B.c_str()))
                {
                    string opB = ftoa(-atof(B));//ֻ��һ��op
                    int opBid = BottomIsDefine(opB);//Ѱ�Ҿ���op������B�ڵ��Ƿ����
                    if (opBid == -1)
                    {
                        cur_nodeB = new DAGNode;
                        cur_nodeB->id = Nodelen;
                        cur_nodeB->Btype = 1;
                        OVar Anode;
                        Anode.name = A;
                        cur_nodeB->Dval.push_back(Anode);
                        cur_nodeB->BCval = atof(opB);
                        cur_nodeB->type = DAGA;
                        indegree[Nodelen] = 0;
                        DAGNodes[Nodelen++] = *cur_nodeB;
                        //                        cout<<"4 Nodelen:"<<Nodelen<<endl;
                        delete cur_nodeB;
                    }
                    else
                    {
                        OVar Anode;
                        Anode.name = A;
                        DAGNodes[opBid].Dval.push_back(Anode);//��A����Node(B)���Ҳ�
                    }
                }
                else
                {
                    cur_nodeB = new DAGNode;
                    cur_nodeB->id = Nodelen;
                    cur_nodeB->Btype = 0;
                    cur_nodeB->BDval.name = B;
                    cur_nodeB->type = DAGA;
                    indegree[Nodelen] = 0;
                    DAGNodes[Nodelen++] = *cur_nodeB;
                    //                    cout<<"5 Nodelen:"<<Nodelen<<endl;

                    cur_nodeA = new DAGNode;
                    cur_nodeA->id = Nodelen;
                    cur_nodeA->Btype = 2;//��ʾ��Ҷ�ӽ��
                    cur_nodeA->type = DAGA;
                    cur_nodeA->op = "-";
                    OVar Anode;
                    Anode.name = A;
                    cur_nodeA->Dval.push_back(Anode);
                    indegree[Nodelen] = 0;
                    DAGNodes[Nodelen++] = *cur_nodeA;
                    //                    cout<<"6 Nodelen:"<<Nodelen<<endl;

                    DAGEdges[cur_nodeA->id].push_back(cur_nodeB->id);//��ͼ
                    indegree[cur_nodeB->id]++;

                    delete cur_nodeA;
                    delete cur_nodeB;
                }
            }
            else
            {
                if (isFloat(B.c_str()) || DAGNodes[Bid].Btype == 1)//Ϊ����
                {
                    string opB = ftoa(-atof(B));//ֻ��һ��op
                    int opBid = BottomIsDefine(opB);//Ѱ�Ҿ���op������B�ڵ��Ƿ����
                    if (opBid == -1)
                    {
                        cur_nodeB = new DAGNode;
                        cur_nodeB->id = Nodelen;
                        cur_nodeB->Btype = 1;
                        OVar Anode;
                        Anode.name = A;
                        cur_nodeB->Dval.push_back(Anode);
                        cur_nodeB->BCval = atof(opB);
                        cur_nodeB->type = DAGA;
                        indegree[Nodelen] = 0;
                        DAGNodes[Nodelen++] = *cur_nodeB;
                        //                        cout<<"7 Nodelen:"<<Nodelen<<endl;
                        delete cur_nodeB;
                    }
                    else
                    {
                        OVar Anode;
                        Anode.name = A;
                        DAGNodes[opBid].Dval.push_back(Anode);//��A����Node(B)���Ҳ�
                    }
                }
                else
                {
                    int b_fa = FindSubB(B);
                    if (b_fa != -1)
                    {
                        OVar Anode;
                        Anode.name = A;
                        DAGNodes[b_fa].Dval.push_back(Anode);
                    }
                    else
                    {
                        cur_nodeA = new DAGNode;
                        cur_nodeA->id = Nodelen;
                        cur_nodeA->Btype = 2;//��ʾ��Ҷ�ӽ��
                        cur_nodeA->type = DAGA;
                        cur_nodeA->op = "-";
                        OVar Anode;
                        Anode.name = A;
                        cur_nodeA->Dval.push_back(Anode);
                        indegree[Nodelen] = 0;
                        DAGNodes[Nodelen++] = *cur_nodeA;
                        //                        cout<<"8 Nodelen:"<<Nodelen<<endl;

                        DAGEdges[cur_nodeA->id].push_back(Bid);//��ͼ
                        indegree[Bid]++;
                        delete cur_nodeA;
                    }
                }
            }
        }
        else if (optype == 22)
        {
            DAGNode* CurNodeA = NULL;
            DAGNode* CurNodeB = NULL;
            DAGNode* CurNodeC = NULL;
            DAGNode* CurNodeP = NULL;
            string A = CodeBlock.code[i].linkres;
            string B = CodeBlock.code[i].op1;
            string C = CodeBlock.code[i].op2;
            string op = CodeBlock.code[i].op;
            int Aid = LeftIsDefine(A);
            if (Aid != -1)//�ҵ���
                DeleteVar(A, Aid);//ɾ�����
            //            cout<<"12 ";
            int Bid = AllocDAGNode(CurNodeB, B);
            CurNodeB = &DAGNodes[Bid];
            //            cout<<"13 ";
            int Cid = AllocDAGNode(CurNodeC, C);
            CurNodeC = &DAGNodes[Cid];
            int Pid = 0;

            if (CurNodeB->Btype == 1 && CurNodeC->Btype == 1)//2(4)
            {
                string P;
                if (op == "+")
                    P = ftoa(CurNodeB->BCval + CurNodeC->BCval);
                else if (op == "-")
                    P = ftoa(CurNodeB->BCval - CurNodeC->BCval);
                else if (op == "*")
                    P = ftoa(CurNodeB->BCval * CurNodeC->BCval);
                else if (op == "/")
                    P = ftoa(CurNodeB->BCval / CurNodeC->BCval);
                if (Cid == -1)
                {
                    Nodelen--;
                    delete CurNodeB;
                }
                if (Bid == -1)
                {
                    Nodelen--;
                    delete CurNodeC;
                }
                //                cout<<"14 ";
                Pid = AllocDAGNode(CurNodeP, P);
                CurNodeP = &DAGNodes[Pid];
                OVar Anode;
                Anode.name = A;
                CurNodeP->Dval.push_back(Anode);
                if (Pid == -1)
                    CurNodeP->type = DAGA;
            }
            else
            {
                int Fid = FindSubBC(B, C, op);//�������ΪB��C�ĸ��׽��
                if (Fid == -1)//�޸ýڵ�
                {
                    CurNodeA = new DAGNode;
                    CurNodeA->id = Nodelen;
                    CurNodeA->type = DAGA;
                    CurNodeA->Btype = 2;
                    CurNodeA->op = op;
                    OVar Anode;
                    Anode.name = A;
                    CurNodeA->Dval.push_back(Anode);

                    DAGNodes[Nodelen++] = *CurNodeA;
                    //                    cout<<"9 Nodelen:"<<Nodelen<<endl;
                    DAGEdges[CurNodeA->id].push_back(CurNodeB->id);
                    DAGEdges[CurNodeA->id].push_back(CurNodeC->id);
                    indegree[CurNodeB->id]++;
                    indegree[CurNodeC->id]++;

                    delete CurNodeA;
                }
                else
                {
                    OVar Anode;
                    Anode.name = A;
                    CurNodeA = &DAGNodes[Fid];
                    CurNodeA->Dval.push_back(Anode);
                }
            }
        }
        else if (optype == 21)
        {
            string A = CodeBlock.code[i].linkres;
            string B = CodeBlock.code[i].op1;
            string C = CodeBlock.code[i].op2;
            string op = CodeBlock.code[i].op;
            DAGNode* CurNodeA = NULL;
            DAGNode* CurNodeB = NULL;
            DAGNode* CurNodeC = NULL;

            //            cout<<"15 ";
            //            cout<<"B:"<<B<<endl;
            int Bid = AllocDAGNode(CurNodeB, B);
            CurNodeB = &DAGNodes[Bid];
            //            cout<<"16 ";
            int Cid = AllocDAGNode(CurNodeC, C);
            CurNodeC = &DAGNodes[Cid];

            CurNodeA = new DAGNode;
            CurNodeA->id = Nodelen;
            CurNodeA->type = DAGJ;
            CurNodeA->Btype = 2;
            CurNodeA->op = op;
            CurNodeA->Cval = atoi(A.c_str());
            DAGNodes[Nodelen++] = *CurNodeA;
            //            cout<<"10 Nodelen:"<<Nodelen<<endl;
            DAGEdges[CurNodeA->id].push_back(CurNodeB->id);
            DAGEdges[CurNodeA->id].push_back(CurNodeC->id);
            indegree[CurNodeB->id]++;
            indegree[CurNodeC->id]++;

            delete CurNodeA;
        }
        else if (optype == 3)
        {
            string A = CodeBlock.code[i].linkres;
            string B = CodeBlock.code[i].op1;
            string C = CodeBlock.code[i].op2;
            string op = CodeBlock.code[i].op;
            DAGNode* CurNodeA = NULL;

            int Aid = LeftIsDefine(A);
            if (Aid != -1)//�ҵ���
                DeleteVar(A, Aid);//ɾ�����

            CurNodeA = new DAGNode;
            CurNodeA->id = Nodelen;
            indegree[Nodelen] = 0;
            //            Aid = AllocDAGNode(CurNodeA,B);
            //            CurNodeA = &DAGNodes[Aid];

            CurNodeA->op = "=";
            CurNodeA->Btype = 2;
            OVar nodeA;
            nodeA.name = A;
            CurNodeA->Dval.push_back(nodeA);
            CurNodeA->id = Nodelen;
            CurNodeA->type = DAGA;
            CurNodeA->FuncN = CodeBlock.code[i + 1].op1;//����

            for (int j = i - 1; j >= 0; j--)
            {
                if (CodeBlock.code[j].op == "Par")//˵���Ǻ����Ĳ���
                {
                    DAGNode* CurNode = NULL;
                    //                    cout<<"17 ";
                    int id = AllocDAGNode(CurNode, CodeBlock.code[j].op1);
                    CurNode = &DAGNodes[id];
                    DAGEdges[CurNodeA->id].push_back(CurNode->id);
                    indegree[CurNode->id]++;
                }
                else
                    break;
            }

            DAGNodes[Nodelen++] = *CurNodeA;
            //            cout<<"11 Nodelen:"<<Nodelen<<endl;
            delete CurNodeA;
        }
    }
}

void Optimizer::FindRoots()
{
    for (int i = 0; i < Nodelen; i++)
        if (indegree[i] == 0)
            DAGRoots.push_back(i);
}

void Optimizer::CutDAGMakeVis(int id)
{
    if (CutVis[id] == 1)
        return;
    CutVis[id] = 1;
    if (DAGNodes[id].Btype == 0 || DAGNodes[id].Btype == 1)//Ҷ�ӽ��
        return;
    for (int i = 0; i < (int)DAGEdges[id].size(); i++)
        if (CutVis[DAGEdges[id][i]] == 0)
            CutDAGMakeVis(DAGEdges[id][i]);
}

void Optimizer::CutDAGGraph(CODE_BLOCK& CodeBlock)
{
    for (int i = 0; i < Nodelen; i++)//���������vis
        CutVis[i] = 0;
    for (int i = 0; i < (int)CodeBlock.Outvarlist.size(); i++)//�����еĳ��ڻ�Ծ����
    {
        int fid = BottomIsDefine(CodeBlock.Outvarlist[i].name);//Ѱ�ҵ����λ��
        if (fid == -1)
            continue;
        CutDAGMakeVis(fid);//��fid������ȫ����1
    }
    for (int i = 0; i < Nodelen; i++)//�����е���ת���
        if (DAGNodes[i].type == DAGJ)//˵������ת��㣬���ܱ��Ż���
            CutDAGMakeVis(DAGNodes[i].id);//���ϱ��
}

void Optimizer::GenOCodedfs(CODE_BLOCK& CodeBlock, int cur)
{
    if (CutVis[cur] == 0)//���������
        return;
    if (DAGNodes[cur].Btype == 1 || DAGNodes[cur].Btype == 0)//Ҷ�ӽ��
    {
        vector<OVar> var_list;//ѡ����Ҫ���ɵı�����
        for (int i = 0; i < (int)DAGNodes[cur].Dval.size(); i++)
        {
            for (int j = 0; j < (int)CodeBlock.Outvarlist.size(); j++)
                if (CodeBlock.Outvarlist[j].name == DAGNodes[cur].Dval[i].name)
                {
                    var_list.push_back(DAGNodes[cur].Dval[i]);
                    break;
                }
        }
        if (DAGNodes[cur].Btype == 1)//����
            for (int i = 0; i < (int)var_list.size(); i++)
            {
                CodeBlock.Ocode.push_back(Code{ "=",ftoa(DAGNodes[cur].BCval),"-",var_list[i].name });
            }

        if (DAGNodes[cur].Btype == 0)//����
            for (int i = 0; i < (int)var_list.size(); i++)
            {
                CodeBlock.Ocode.push_back(Code{ "=",DAGNodes[cur].BDval.name,"-",var_list[i].name });
            }
        return;
    }
    for (int i = 0; i < (int)DAGEdges[cur].size(); i++)//�����ӽڵ�
        if (CutVis[cur] == 1)
        {
            GenOCodedfs(CodeBlock, DAGEdges[cur][i]);
        }
    //���ɱ��ڵ�Ӧ�����ɵ����
    if (DAGNodes[cur].type == DAGJ)
    {
        if (DAGEdges[cur].size() == 0)
            CodeBlock.Ocode.push_back(Code{ DAGNodes[cur].op,"-","-",ftoa(DAGNodes[cur].Cval) });
        else if (DAGEdges[cur].size() == 2)
        {
            Code this_code;
            DAGNode lto = DAGNodes[DAGEdges[cur][0]];
            DAGNode rto = DAGNodes[DAGEdges[cur][1]];
            this_code.op = DAGNodes[cur].op;
            this_code.linkres = ftoa(DAGNodes[cur].Cval);
            if (lto.Btype == 2)//��Ҷ�ӽ��
                this_code.op1 = lto.Dval[0].name;
            else if (lto.Btype == 1)//����
                this_code.op1 = ftoa(lto.BCval);
            else if (lto.Btype == 0)//����
                this_code.op1 = lto.BDval.name;

            if (rto.Btype == 2)//��Ҷ�ӽ��
                this_code.op2 = rto.Dval[0].name;
            else if (rto.Btype == 1)//����
                this_code.op2 = ftoa(rto.BCval);
            else if (rto.Btype == 0)//����
                this_code.op2 = rto.BDval.name;
            CodeBlock.Ocode.push_back(Code{ this_code.op,this_code.op1,this_code.op2,this_code.linkres });
        }
    }
    else
    {
        if (DAGNodes[cur].FuncN.length() == 0)//˵�����ǵ��ú���
        {
            vector<OVar> var_list;//ѡ����Ҫ���ɵı�����
            for (int i = 0; i < (int)DAGNodes[cur].Dval.size(); i++)
            {
                for (int j = 0; j < (int)CodeBlock.Outvarlist.size(); j++)
                    if (CodeBlock.Outvarlist[j].name == DAGNodes[cur].Dval[i].name)
                    {
                        var_list.push_back(DAGNodes[cur].Dval[i]);
                        break;
                    }
            }
            if (var_list.size() == 0)
                var_list.push_back(DAGNodes[cur].Dval[0]);

            for (int i = 0; i < (int)var_list.size(); i++)
            {
                if (DAGEdges[cur].size() == 1)//��Ŀ�����
                {
                    DAGNode to = DAGNodes[DAGEdges[cur][0]];
                    if (to.Btype == 2 && to.Dval.size() > 0)//��Ҷ�ӽ��
                        CodeBlock.Ocode.push_back(Code{ DAGNodes[cur].op,to.Dval[0].name, "-",var_list[i].name });//������п��ܻ�Խ��emmm
                    else if (to.Btype == 1)//����
                        CodeBlock.Ocode.push_back(Code{ DAGNodes[cur].op,ftoa(to.BCval), "-",var_list[i].name });
                    else if (to.Btype == 0)//����
                        CodeBlock.Ocode.push_back(Code{ DAGNodes[cur].op,to.BDval.name, "-",var_list[i].name });
                }
                else if (DAGEdges[cur].size() == 2)//˫Ŀ�����
                {
                    Code this_code;
                    DAGNode lto = DAGNodes[DAGEdges[cur][0]];
                    DAGNode rto = DAGNodes[DAGEdges[cur][1]];
                    this_code.op = DAGNodes[cur].op;
                    this_code.linkres = var_list[i].name;
                    if (lto.Btype == 2 && lto.Dval.size() > 0)//��Ҷ�ӽ��
                        this_code.op1 = lto.Dval[0].name;//������п��ܻ�Խ��emmm
                    else if (lto.Btype == 1)//����
                        this_code.op1 = ftoa(lto.BCval);
                    else if (lto.Btype == 0)//����
                        this_code.op1 = lto.BDval.name;

                    if (rto.Btype == 2 && rto.Dval.size() > 0)//��Ҷ�ӽ��
                        this_code.op2 = rto.Dval[0].name;//������п��ܻ�Խ��emmm
                    else if (rto.Btype == 1)//����
                        this_code.op2 = ftoa(rto.BCval);
                    else if (rto.Btype == 0)//����
                        this_code.op2 = rto.BDval.name;

                    CodeBlock.Ocode.push_back(Code{ this_code.op,this_code.op1,this_code.op2,this_code.linkres });
                }
            }
        }
        else//˵���ǵ��ú���
        {
            vector<OVar> var_list;//ѡ����Ҫ���ɵı�����
            for (int i = 0; i < (int)DAGNodes[cur].Dval.size(); i++)
            {
                for (int j = 0; j < (int)CodeBlock.Outvarlist.size(); j++)
                    if (CodeBlock.Outvarlist[j].name == DAGNodes[cur].Dval[i].name)
                    {
                        var_list.push_back(DAGNodes[cur].Dval[i]);
                        break;
                    }
            }
            if (var_list.size() == 0)
                var_list.push_back(DAGNodes[cur].Dval[0]);

            for (int i = 0; i < (int)var_list.size(); i++)
            {
                //�������еĲ���������par
                for (int j = 0; j < (int)DAGEdges[cur].size(); j++)
                {
                    int to = DAGEdges[cur][j];
                    if (DAGNodes[to].Btype == 0)
                        CodeBlock.Ocode.push_back(Code{ "Par",DAGNodes[to].BDval.name,"-","-" });
                    else if (DAGNodes[to].Btype == 1)
                        CodeBlock.Ocode.push_back(Code{ "Par",ftoa(DAGNodes[to].BCval),"-","-" });
                    else if (DAGNodes[to].Btype == 2)
                        if (DAGNodes[to].Dval.size() > 0)
                            CodeBlock.Ocode.push_back(Code{ "Par",DAGNodes[to].Dval[0].name,"-","-" });//������п��ܻ�Խ��emmm
                }
                //���ɸ�ֵ���
                CodeBlock.Ocode.push_back(Code{ "=","@BackReturn","-",var_list[i].name });
                //���ɵ��ú������
                CodeBlock.Ocode.push_back(Code{ "Call",DAGNodes[cur].FuncN,"-","-" });
            }
        }
    }
    return;
}

void Optimizer::GenOCode(CODE_BLOCK& CodeBlock)
{
    //���DAGͼ
    ClearDAG();
    //����DAGͼ
    CreateDAG(CodeBlock);
    //    cout<<"Nodelen:"<<Nodelen<<endl;
    //    for(int i=0;i<Nodelen;i++)
    //        cout<<"DAGNodes[i].Dval.size(): "<<DAGNodes[i].Dval.size()<<endl;
        //�Ҹ����
    FindRoots();
    //�������Ǹ��ݳ��ڴ���Ծ�ı�������DAGͼ���м�֦
    CutDAGGraph(CodeBlock);
    //�����Ż�����м����
    for (int i = 0; i < (int)DAGRoots.size(); i++)//�Ӹ���㿪ʼ����
        GenOCodedfs(CodeBlock, DAGRoots[i]);
    if (CodeBlock.code[CodeBlock.code.size() - 1].op == "return")//�ж��Ƿ�Ϊ�����Ľ���������
        CodeBlock.Ocode.push_back(CodeBlock.code[CodeBlock.code.size() - 1]);


    //    for(int i=0;i<(int)DAGRoots.size();i++)
    //        cout<<"DAGRoots["<<i<<"]:"<<DAGRoots[i]<<endl;
    //    cout<<"nodelen:"<<Nodelen<<endl;
}

void Optimizer::ClearDAG()
{
    for (int i = 0; i < Nodelen; i++)
    {
        DAGEdges[i].clear();
        CutVis[i] = 0;
        indegree[i] = 0;
        DAGNodes[i].BCval = 0;
        DAGNodes[i].Btype = 0;
        DAGNodes[i].Cval = 0;
        DAGNodes[i].id = 0;
        DAGNodes[i].type = DAGJ;
    }
    DAGRoots.clear();
    Nodelen = 0;
}

int Optimizer::OpType(Code MiddleCode)
{
    if (MiddleCode.op1 == "@BackReturn")
        return 3;
    if (MiddleCode.op == "j" || MiddleCode.op == "=")
        return 0;
    if ((MiddleCode.op == "-" && MiddleCode.op2 == "-") || (MiddleCode.op == "+" && MiddleCode.op2 == "-"))
        return 1;
    if (MiddleCode.op == "j<" || MiddleCode.op == "j<=" || MiddleCode.op == "j>"
        || MiddleCode.op == "j>=" || MiddleCode.op == "j==" || MiddleCode.op == "j!=")//��ת
        return 21;
    if (MiddleCode.op == "+" || MiddleCode.op == "-" || MiddleCode.op == "/" || MiddleCode.op == "*")
        return 22;
    return -1;
}

