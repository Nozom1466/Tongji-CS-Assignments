#pragma execution_character_set("utf-8") 
#include "SynSemAnalysis.h"


using namespace std;



/**************************Action***********************************/

ostream& operator << (ostream& os, const Actions& s)
{
    if (s.Type == MoveIn) {
        os << "s" << s.State;
    }
    else if (s.Type == Resolution) {
        os << "r" << s.ProduceFormsId;
    }
    else {
        os << "acc";
    }
    return os;
}


ofstream& operator << (ofstream& os, const Actions& s)
{
    if (s.Type == MoveIn) {
        os << "s" << s.State;
    }
    else if (s.Type == Resolution) {
        os << "r" << s.ProduceFormsId;
    }
    else {
        os << "acc";
    }
    return os;
}



/***************Sign*****************/
Sign::Sign(string MyStr, SignType MyType)
{
    this->str = MyStr;
    this->type = MyType;
}
Sign::Sign(const Sign& s)
{
    this->str = s.str;
    this->type = s.type;
}

Sign::Sign()
{

}

/***************************ProduceForms*******************************/
ProduceForms::ProduceForms()
{

}

fstream& operator << (fstream& os, const ProduceForms* s)
{
    os << "[ " << s->LeftSign.str << "-->";
    for (int i = 0; i < (int)s->RightSign.size(); i++)
        os << s->RightSign[i].str << " ";
    os << " ]";
    return os;
}

ostream& operator << (ostream& os, const ProduceForms* s)
{
    os << "[ " << s->LeftSign.str << "-->";
    for (int i = 0; i < (int)s->RightSign.size(); i++)
        os << s->RightSign[i].str << " ";
    os << " ]";
    return os;
}


/***************************LR1Production*******************************/

LR1Production::LR1Production(int MyDotPosition, Sign MyForesee, const ProduceForms& BaseProdction)
{
    this->DotPosition = MyDotPosition;
    this->Foresee = MyForesee;//������=����ΪҪ��string�������
    this->id = BaseProdction.id;
    this->LeftSign = BaseProdction.LeftSign;
    for (int i = 0; i < (int)BaseProdction.RightSign.size(); i++)
        this->RightSign.push_back(BaseProdction.RightSign[i]);
}

void LR1Production::PopSign()
{
    this->DotPosition++;
}

LR1Production* LR1Production::next_LR1Production(Sign InSign)//ͨ������������һЩ��Ŀ
{
    if (this->DotPosition < (int)this->RightSign.size() && InSign.str == this->RightSign[this->DotPosition].str && InSign.type == this->RightSign[this->DotPosition].type)//˵������ת��
        return (LR1Production*)new LR1Production(this->DotPosition + 1, this->Foresee, (const ProduceForms)(*this));
    else
        return NULL;
}


/***************************ProductionSet*******************************/

ProductionSet::ProductionSet(vector<LR1Production> MyLR1Productions, int MyId)
{
    size_t len = MyLR1Productions.size();
    for (size_t i = 0; i < len; i++)
        this->LR1Productions.push_back(&MyLR1Productions[i]);
    this->id = MyId;
}

ProductionSet::ProductionSet()
{
    
}




/*****************�ķ��������� ********************/

GrammaAnalysis::GrammaAnalysis()
{
    TableOut = Table_FILE;
    GraphOut = TREE_PNG_FILE;
    GraphOutDot = TREE_DOT_FILE;


    this->SemanticProcess.MiddleVarIndex = 0;
    this->SemanticProcess.Level = 0;//�㼶
    this->SemanticProcess.ProcNo = 0;//Ŀǰ�Ĺ��̱��
    this->SemanticProcess.ProcNoStack.push_back(this->SemanticProcess.ProcNo);//����ջ
    this->SemanticProcess.semanticerror = None;
}


//�������е���Ŀ
int GrammaAnalysis::ProduceFormsGenerate(string file)
{
    ifstream fin;
    fin.open(file, ios::in);//����ʮ���ƴ�,getline��βͳһ����
    if (!fin.is_open()) {
        
        return GRAMMAR_FILE_OPEN_ERROR;
    }
    ofstream ferr;
    ferr.open(GRAMMAR_ERROR_FILE, ios::out);

    string str;
    int i = 0, j = 0;
    while (1) {
        fin.clear();
        getline(fin, str);
        ++i;
        if (!str.length()) {
            if (fin.eof()) {
                break;
            }
            else {
                continue;
            }
        }
        //ע��
        if (str[0] == '$') {
            continue;
        }
        auto pos = str.find("::=");
        if (pos == str.npos) {
            ferr << "error at line" << i << " str:" << str << endl;
            return GRAMMAR_ERROR;
        }
        ++j;//��¼��Ч�� Ѱ�ҿ�ʼ��
        ProduceForms pf;
        //��ֱ��ȡ ��Ϊ��һ��Ϊ��ʼ��
        pf.LeftSign.str = str.substr(0, pos);
        ClearAllSpace(pf.LeftSign.str);
        pf.LeftSign.type = (j == 1 ? StartTerminator : NonTerminator);
        string right_s = str.substr(pos + 3);
        //ÿ��|��һ���Ҳ�
        while ((pos = right_s.find("|")) != string::npos) {
            ProduceForms* ppf = new ProduceForms;
            *ppf = pf;
            ppf->id = j - 1;
            ++j;
            string right_s_one = right_s.substr(0, pos);
            right_s = right_s.substr(pos + 1);
            analysis_right(ppf->RightSign, right_s_one);
            ProduceFormsVec.push_back(ppf);
        }
        //���һ��
        ProduceForms* ppf = new ProduceForms;
        *ppf = pf;
        ppf->id = j - 1;
        analysis_right(ppf->RightSign, right_s);
        ProduceFormsVec.push_back(ppf);
    }
    for (auto i : ProduceFormsVec) {
        AllNonTerminator.insert(i->LeftSign);
        for (auto j : i->RightSign) {
            if (j.type == Terminator || j.type == Empty) {
                AllTerminator.insert(j);
            }
        }
    }
    return SUCCESS;
}


//��ĳ�����ŵ�FIRST��
//�����ս����FIRST�������Լ�
//���ڷ��ս��
    /*
    ��X ��VN�����в���ʽX��a��,a��VT�����a���뵽FIRST��X���У�����X���ţ���Ѧż���FIRST��X);
    ��X��VN , ��X��Y �� , Y��VN, ���FIRST (Y) - {��}�ӵ�FIRST (X)��
    ��X��Y1Y2 �� Yk,Y1, Y2, �� ,Yi-1 �� VN,�š�FIRST(Yj)
    ���(1<= j <= i -1)FIRST (Yi) - {��}�ӵ�FIRST (X)�С�
    �ر�أ����š�FIRST (Yj)(1<=j <= k )����š�FIRST(X)
    */
set<Sign> GrammaAnalysis::GetFirstSign(Sign InSign)
{
    set<Sign> sign_vec;
    if (InSign.type == Terminator) {
        sign_vec.insert(InSign);
        return sign_vec;
    }
    //���ھͷ���
    if (!first_table[InSign.str].empty()) {
        return first_table[InSign.str];
    }
    //��������ʽ
    for (auto pf_p : ProduceFormsVec) {
        //�����
        if (pf_p->LeftSign == InSign) {
            //��һ����Terminator
            if (pf_p->RightSign[0].type == Terminator) {
                first_table[InSign.str].insert(pf_p->RightSign[0]);
            }
            //���ǵĻ� �����Ҳ�
            else {
                size_t length = 0;
                for (auto& right : pf_p->RightSign) {
                    auto right_set = GetFirstSign(right);
                    int zero = 0;
                    for (auto& right_set_sign : right_set)
                    {
                        //ȥ��first-{��}
                        if (right_set_sign.str == "��") {
                            zero = 1;
                            continue;
                        }
                        //�ǿղ���
                        first_table[InSign.str].insert(right_set_sign);
                    }
                    //˵��û��{��} ���ؼ�����һ����first
                    if (!zero) {
                        break;
                    }
                    ++length;
                }
                //�������Ҳ�����{��} ��{��}����first
                if (length == pf_p->RightSign.size()) {
                    first_table[InSign.str].insert(Sign("��", Terminator));
                }
            }
        }
    }

    return first_table[InSign.str];
}


//���������ַ������õ�first��
//1.���ڷ��Ŵ���= X1X2�� Xn������ FIRST (��)
//2.�������е� Xj ,1<=j<= i -1, �š�FIRST (Xj), ���FIRST(Xi) -{��}�ӵ�FIRST(��)��
//3.�������е� Xj, 1 <= j <= n, �š�FIRST(Xj), ��Ѧżӵ�FIRST(��)��
set<Sign> GrammaAnalysis::GetFirstSign(vector<Sign> InSign)
{
    set<Sign> set_sign;
    size_t i;
    for (i = 0; i < InSign.size(); ++i) {
        auto right_set = GetFirstSign(InSign[i]);
        int zero = 0;
        for (auto& right_set_sign : right_set)
        {
            //ȥ��first-{��}
            if (right_set_sign.str == "��") {
                zero = 1;
                continue;
            }
            //�ǿղ���
            set_sign.insert(right_set_sign);
        }
        //˵��û��{��} ���ؼ�����һ����first
        if (!zero) {
            break;
        }
    }
    if (i == InSign.size()) {
        set_sign.insert(Sign("��", Terminator));
    }
    return set_sign;
}

//�õ���Ŀ���հ�
void GrammaAnalysis::GetFull(ProductionSet& MyDFAState)
{
    int CurCheckPosition = 0;
    for (; CurCheckPosition < (int)MyDFAState.LR1Productions.size(); CurCheckPosition++)
    {
        //�����Ѿ�Ҫ��Լ��ʽ�ӣ����������
        LR1Production Check = *MyDFAState.LR1Productions[CurCheckPosition];
        if (Check.DotPosition == (int)Check.RightSign.size())//˵���ǹ�Լ��
            continue;
        for (int i = 0; i < (int)ProduceFormsVec.size(); i++)
        {
            if (ProduceFormsVec[i]->LeftSign.str == Check.RightSign[Check.DotPosition].str && ProduceFormsVec[i]->LeftSign.type == Check.RightSign[Check.DotPosition].type)
            {
                //�ȵõ�Ҫ��first��
                vector<Sign> FirstInSign;
                set<Sign> FirstSet;
                for (int j = Check.DotPosition + 1; j < (int)Check.RightSign.size(); j++)
                    FirstInSign.push_back(Check.RightSign[j]);
                FirstInSign.push_back(Check.Foresee);

                FirstSet = GetFirstSign(FirstInSign);
                //��¼һ�����ɵ�first����
                string FirstInSignStr;
                for (int z = 0; z < (int)FirstInSign.size(); z++)
                {
                    FirstInSignStr += " ";
                    FirstInSignStr += FirstInSign[z].str;
                }
                first_table[FirstInSignStr] = FirstSet;
                //ͨ��first���Լ�ɸѡ�õĲ���ʽ������Ҫ���뼯�ϵ�LR1��Ŀ
                set<Sign>::iterator iter;
                for (iter = FirstSet.begin(); iter != FirstSet.end(); ++iter)
                {
                    LR1Production* AddProduction = (LR1Production*)new LR1Production(0, *iter, *ProduceFormsVec[i]);
                    //����
                    int IsFind = 0;
                    for (int k = 0; k < (int)MyDFAState.LR1Productions.size(); k++)
                    {
                        if (*MyDFAState.LR1Productions[k] == *AddProduction)
                            IsFind = 1;
                    }
                    if (!IsFind)//û���ظ�
                    {
                        MyDFAState.LR1Productions.push_back(AddProduction);
                    }
                }
            }
        }

    }
}

//����DFA
void GrammaAnalysis::BuildDFA()
{
    //1. �����ʼ����
    ProductionSet* InitSet = new ProductionSet();
    for (int i = 0; i < (int)ProduceFormsVec.size(); i++)
    {
        if (ProduceFormsVec[i]->LeftSign.type == StartTerminator)//�ҵ���ʼ���ŵĲ���ʽ��
        {
            LR1Production* StartPd = new LR1Production(0, Sign{ "#", Terminator }, *ProduceFormsVec[i]);
            //�����ʼ����
            InitSet->id = 0;//��ʼ����id
            InitSet->LR1Productions.push_back(StartPd);
            GetFull(*InitSet);//�õ��հ�
            break;
        }
    }
    ProductionSetVec.push_back(InitSet);
    //2. �ӳ�ʼ���ϳ�����ʼ����DFA
    int SetId = 1;
    int CurCheckPosition = 0;
    //2.1 �ȵõ���������ת�Ƶķ���
    for (; CurCheckPosition < (int)ProductionSetVec.size(); CurCheckPosition++)//����ÿһ������
    {
        vector<Sign> TranSign;
        ProductionSet* CurSet = ProductionSetVec[CurCheckPosition];
        for (int j = 0; j < (int)CurSet->LR1Productions.size(); j++)//����ÿһ��LR1��Ŀ
        {
            int IsFind = 0;
            for (int k = 0; k < (int)TranSign.size(); k++)
                if (CurSet->LR1Productions[j]->DotPosition < (int)CurSet->LR1Productions[j]->RightSign.size() && TranSign[k] == CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition])
                    IsFind = 1;
            if (!IsFind)
                if (CurSet->LR1Productions[j]->DotPosition < (int)CurSet->LR1Productions[j]->RightSign.size() && CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition].str != "��")//���ڷǹ�Լ��
                    TranSign.push_back(CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition]);//���뵽����
        }
        //2.2 ͨ��ÿһ��ת�Ʒ�����������Ŀ��
        for (int k = 0; k < (int)TranSign.size(); k++)//����ÿһ��ת�Ʒ���
        {
            ProductionSet* AddPdSet = new ProductionSet();//Ҫ��ӵ���Ŀ��
            for (int j = 0; j < (int)CurSet->LR1Productions.size(); j++)//����ÿһ��LR1��Ŀ
            {
                LR1Production* AddPd = CurSet->LR1Productions[j]->next_LR1Production(TranSign[k]);
                if (AddPd == NULL)//û�в���
                    continue;
                AddPdSet->LR1Productions.push_back(AddPd);//�����ˣ����뼯��
            }
            GetFull(*AddPdSet);//�õ��հ�
            int IsFind = 0;
            for (int l = 0; l < (int)ProductionSetVec.size(); l++)
                if (*AddPdSet == *ProductionSetVec[l])
                {
                    delete AddPdSet;
                    AddPdSet = ProductionSetVec[l];
                    IsFind = l;
                    break;
                }
            if (!IsFind)
            {
                AddPdSet->id = SetId++;
                CurSet->MyMap[TranSign[k].str] = k;
                CurSet->NextNode.push_back(AddPdSet);//������ϵ
                ProductionSetVec.push_back(AddPdSet);//������Ŀ��
            }
            else
            {
                CurSet->MyMap[TranSign[k].str] = k;
                CurSet->NextNode.push_back(AddPdSet);//������ϵ
            }
        }
    }

}

//����ķ����޹�Լ��ͻ
bool GrammaAnalysis::Conflict_Detection(const Actions& action, pair<int, Sign>& tableindex, int i, LR1Production* j)
{
    ofstream ferr(GRAMMAR_ERROR_FILE,ios::out);
    if (ActionTable.count(tableindex)) {
        if (ActionTable[tableindex] != action) {
            ferr << "error ��Լ��ͻ!�Ѿ���action[" << i << "," << j->Foresee.str << "]=" << action <<endl<<"������LR1�ķ�" << endl;
            return 1;
        }
    }
    return 0;
}


//����ACtiongo��
int GrammaAnalysis::BuildActionGo()
{
    //goto��
    for (int i = 0; (int)i < ProductionSetVec.size(); i++) {
        for (auto& sign : AllNonTerminator) {
            if (ProductionSetVec[i]->MyMap.count(sign.str)) {
                auto ProductionSet_p = ProductionSetVec[i]->NextNode[ProductionSetVec[i]->MyMap[sign.str]];
                GoTable[{i, sign}] = ProductionSet_p->id;
            }
        }
    }
    pair<int, Sign>tableindex;
    Actions action;
    //action��
    for (int i = 0; (int)i < ProductionSetVec.size(); i++) {
        for (auto j : ProductionSetVec[i]->LR1Productions) {
            //��Լ
            if (j->DotPosition == j->RightSign.size()) {
                if (j->Foresee.str != "#" || j->id != 0) {
                    tableindex = { i, j->Foresee };
                    action = Actions{ -1,j->id,Resolution };
                    if (Conflict_Detection(action, tableindex, i, j)) {
                        return GRAMMAR_ERROR;
                    }
                    ActionTable[tableindex] = action;
                }
                else {
                    tableindex = { i, j->Foresee };
                    action = Actions{ -1,j->id,Acc };
                    if (Conflict_Detection(action, tableindex, i, j)) {
                        return GRAMMAR_ERROR;
                    }
                    ActionTable[tableindex] = action;
                }

            }
            else
            {
                auto& sign = j->RightSign[j->DotPosition];
                if (sign.type == Terminator) {
                    if (sign.str == "��") {
                        tableindex = { i, j->Foresee };
                        action = Actions{ -1,j->id,Resolution };
                        if (Conflict_Detection(action, tableindex, i, j)) {
                            return GRAMMAR_ERROR;
                        }
                        ActionTable[tableindex] = action;
                    }
                    else {
                        if (ProductionSetVec[i]->MyMap.count(sign.str)) {
                            auto ProductionSet_p = ProductionSetVec[i]->NextNode[ProductionSetVec[i]->MyMap[sign.str]];
                            tableindex = { i, sign };
                            action = Actions{ ProductionSet_p->id,-1,MoveIn };
                            if (Conflict_Detection(action, tableindex, i, j)) {
                                return GRAMMAR_ERROR;
                            }
                            ActionTable[tableindex] = action;
                        }
                    }

                }
            }
        }
    }
    return SUCCESS;

}


//��ӡaction��goto��
void GrammaAnalysis::PrintActionGoToFile()
{
    ofstream fout;
    fout.open(TableOut, ios::out);
    pair<int, Sign>tableindex;
    Actions action;
    fout << "״̬,";
    for (auto& terminator : AllTerminator) {
        if (terminator.str == ",")
            fout << "\",\",";
        else if (terminator.str == "��")
            fout << "#,";
        else
            fout << terminator.str << ",";
    }

    for (auto& terminator : AllNonTerminator) {
        fout << terminator.str << ",";
    }
    fout << "\n";
    for (int i = 0; i < (int)ProductionSetVec.size(); i++) {
        fout << i << ",";
        for (auto& terminator : AllTerminator) {
            tableindex = { i, terminator };
            if (tableindex.second.str == "��") {
                tableindex.second.str = "#";
            }
            if (ActionTable.count(tableindex)) {
                action = ActionTable[tableindex];
                fout << action << ",";
            }
            else {
                fout << " ,";
            }
        }
        int id;
        for (auto& terminator : AllNonTerminator) {
            tableindex = { i, terminator };
            if (GoTable.count(tableindex)) {
                id = GoTable[tableindex];
                fout << id << ",";
            }
            else {
                fout << " ,";
            }

        }
        fout << "\n";
    }
    fout.close();
}

//��������DFA
int GrammaAnalysis::printParseDFA() {


    ofstream fdot(DFA_DOT_FILE, ios::out);
    if (!fdot.is_open()) {
        return DFA_DOT_FILE_OPEN_ERROR;
    }


    fdot << "digraph G {\n";
    fdot << "node [shape=rectangle, fontname=\"FangSong\"];\n";

    for (int i = 0; i < (int)ProductionSetVec.size(); i++) {
        for (auto j : ProductionSetVec[i]->MyMap) {
            fdot << "  \"I" << ProductionSetVec[i]->id << "\" -> \"I" << ProductionSetVec[i]->NextNode[j.second]->id << "\" [label=\"";
            // �˴�������ſ���ֱ�������ʵ�ʿ�����Ҫת��
            fdot << j.first << "\" ,fontname=\"FangSong\"];\n";
        }
    }

    fdot << "}\n";
    fdot.close();
    return SUCCESS;
}



/************************��Լ����**************************************/


int GrammaAnalysis::Forward(lexer *MyWordSets)//ǰ�򴫲��������﷨��
{
   // int cnt = 0;
    WordSets = MyWordSets;
    int NodeId = 0;//�������ֲ�ͬ��Node
    //��ʼ������ջ��״̬ջ
    SignStack.push(TreeNode{ "#","#",StartTerminator });
    StateStack.push(0);
    int step = 0;

    ofstream  Reduction(REDUCTION_PROCESS_FILE, ios::out);//��¼��Լ����
    // �����һ��
    //Reduction << "STEP: " << step << endl;
  
    ofstream ErrorList(PARSE_ERROR_FILE,ios::out);//��¼��Լ���̵Ĵ���


    //���dot�ļ�
    ofstream GraphFile(TREE_DOT_FILE, ios::in | ios::trunc);//������ɾ��
    GraphFile << ("digraph G{") << endl;
    GraphFile << ("edge[fontname = \"FangSong\"];") << endl;
    GraphFile << ("node[shape = box, fontname = \"FangSong\" size = \"20,20\"];") << endl;
    //�Ӵʷ���������ȡԪ�أ�Ȼ���ж�
    TreeNode InSign;//����ķ���
    SemanticTreeNode SInSign;//����ջ�ķ���
    int TopState;//ջ����״̬
    int WordPos = 0;//�����λ��
    Actions Action;//����

    //debug ����
    vector<TreeNode> SignStackVec;
    vector<int> StateStackVec;
    int i = 0;
    while (WordPos < (int)WordSets->token_vec.size())
    {
        
        step++;
        Reduction << "STEP: " << step << endl;
        //��ʼ��Ҫʹ�õĶ���
        if (WordSets->token_vec[WordPos].type == 1)//�Ӵʷ���������ȡ��Ԫ��
        {
            InSign = TreeNode{ "ID" ,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;//��ʼ��SInSign���ս��
            SInSign.type = Terminator;
               
        }
        else if (WordSets->token_vec[WordPos].type == 2)
        {
            InSign = TreeNode{ "num" ,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;
            //cout << SInSign.Name << endl;
            SInSign.ValTpye = FLOAT1;
            sscanf(SInSign.Name.c_str(), "%d", &SInSign.ival);
            sscanf(SInSign.Name.c_str(), "%f", &SInSign.fval);//��ʼ��SInSign���ս��
            //cout << SInSign.ival << ' ' << SInSign.fval << endl;
            SInSign.type = Terminator;
              
        }
        else
        {
            InSign = TreeNode{ WordSets->token_vec[WordPos].word,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;
            SInSign.type = Terminator;//��ʼ��SInSign���ս��
              
           
        }
        TopState = StateStack.top();

        stringstream StateStackStr;
        stringstream SignStackStr;
        while (!StateStack.empty())
        {
            StateStackVec.push_back(StateStack.top());
            StateStack.pop();
        }
        for (int i = (int)StateStackVec.size() - 1; i >= 0; i--)
        {
            StateStack.push(StateStackVec[i]);
            if (i == (int)StateStackVec.size() - 1)
                StateStackStr << StateStackVec[i];
            else
                StateStackStr << "," << StateStackVec[i];
        }
        while (!SignStack.empty())
        {
            SignStackVec.push_back(SignStack.top());
            SignStack.pop();
        }
        for (int i = (int)SignStackVec.size() - 1; i >= 0; i--)
        {
            SignStack.push(SignStackVec[i]);
            if (i == (int)SignStackVec.size() - 1)
                SignStackStr << SignStackVec[i].str;
            else
                SignStackStr << "," << SignStackVec[i].str;
        }
        Reduction << "SYMBOL_STACK:  ";
        Reduction<<SignStackStr.str()<<endl;
       
        Reduction << "STATUS_STACK: ";
        Reduction<<StateStackStr.str()<<endl;

        Reduction << "SHIFT_SYMBOL:  ";
        Reduction<<InSign.str<<endl;

        Reduction << "INPUT: ";
        for (int i = WordPos; i < (int)WordSets->token_vec.size(); i++) {
           Reduction << WordSets->token_vec[i].word;
        }
        

        Reduction <<endl<< "ACTION: ";

        SignStackVec.clear();
        StateStackVec.clear();
        if (ActionTable.count(pair<int, Sign>{TopState, Sign{ InSign.str,InSign.type }}) == 0)//˵��û���ҵ�
        {
            
            ErrorList << "�﷨��������\n" << "�Ҳ���" << WordSets->token_vec[WordPos].word << "��ת��, ";
            ErrorList << "����λ���ڵ�" << WordSets->token_vec[WordPos].line << "��\n";
            this->ErrorLine = WordSets->token_vec[WordPos].line;
          
            return ACTION_ERROR;
            //            return 0;
        }
        Action = ActionTable[pair<int, Sign>{TopState, Sign{ InSign.str,InSign.type }}];
        if (Action.Type == Acc)//����
        {
            Reduction<<"Success!!!";
            break;
        }
        else if (Action.Type == Resolution)//���
        {
           // cout << cnt++;
            ProduceForms* ResoProduce = ProduceFormsVec[Action.ProduceFormsId];//��Ҫ��������ҵ�����Ϊ�±��id��һһ��Ӧ��ϵ
            //���������嶯��
            //this->SemanticProcess.semanticerror = None;
            this->SemanticProcess.SemanticLeftSign = ResoProduce->LeftSign;
            //cout << ResoProduce->id << ' ' << ResoProduce->LeftSign.str<<endl;
            this->SemanticProcess.SemanticAction(ResoProduce);
            //cout << this->SemanticProcess.semanticerror;
            if (this->SemanticProcess.semanticerror != None)
            {
                
                this->ErrorLine = this->WordSets->token_vec[WordPos-1].line;
                SDisError(WordPos, this->SemanticProcess.semanticerror);//��ӡ���������Ϣ
                return SEMANTIC_ERROR;
            }
                
        
            //�������﷨�����еĹ��
            int PopSize = ResoProduce->RightSign.size();
            if (ResoProduce->RightSign[0].str == "��")
                PopSize = 0;
            while (PopSize--)//����ջ����״̬ջ��Ԫ��ȫ����ջ
            {
                string OutStr = "\"" + SignStack.top().StrType + "\"" + " -> " + "\"" + ResoProduce->LeftSign.str + " id=" + to_string(NodeId) + "\"" + ";";
                GraphFile << (OutStr.c_str()) << endl;
                SignStack.pop();
                StateStack.pop();
            }
            SignStack.push(TreeNode{ ResoProduce->LeftSign.str,ResoProduce->LeftSign.str + " id=" + to_string(NodeId++) ,ResoProduce->LeftSign.type });//����ʽ����ջ
            TopState = StateStack.top();
            StateStack.push(GoTable[pair<int, Sign>{TopState, ResoProduce->LeftSign}]);
         
            stringstream TempOutput;
            TempOutput << "��Լ :" << ResoProduce << endl;

            Reduction << TempOutput.str() << endl;

        }
        else//�ƽ�
        {
            StateStack.push(Action.State);
            SignStack.push(InSign);
           // cout << SInSign.ival <<' '<<SInSign.fval << endl;
            this->SemanticProcess.SemanticStack.push(SInSign);
            WordPos++;
            Reduction << "�ƽ�" << endl<<endl;
        }
        i++;
       
    }
    GraphFile << ("}") << endl;
    GraphFile.close();//�ر��ļ�
    Reduction.close();

    return SUCCESS;
}


//�����������
void GrammaAnalysis::SDisError(int WordPos, SemanticError ErrType)
{
    ofstream fout(SEMANTIC_ERROR_FILE, ios::out);
     
    string ErrorInfo = "�������:";
    if (ErrType == NoFuncName)
    {
        ErrorInfo += "����δ����ĺ���:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "\n";
    }
    else if (ErrType == FuncParLenError)
    {
        ErrorInfo += "����:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "������������ò�һ��\n";
    }
    else if (ErrType == NoMainName)
    {
        ErrorInfo += "δ����main����\n";
    }
    else if (ErrType == FuncReturnErr)
    {
        ErrorInfo += "����:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "�����������������Ͳ�һ��\n";
    }
    else if (ErrType == NoVar)
    {
        ErrorInfo += "δ�������";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
    }
    else if (ErrType == RedefineVar)
    {
        ErrorInfo += "�ض������";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
  
       
    }
    else if (ErrType == ArrayIndexError)
    {
        ErrorInfo += "�����±�ʹ�ô���";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
    }
    ErrType = None;
    fout << ErrorInfo;
    fout << "����λ��: ��" << this->WordSets->token_vec[WordPos-1].line << "��\n";
    fout.close();
}


//�ͷ��ڴ�
GrammaAnalysis::~GrammaAnalysis()
{
    for (int i = 0; i < (int)ProduceFormsVec.size(); i++)
        delete ProduceFormsVec[i];
    for (int i = 0; i < (int)ProductionSetVec.size(); i++)
        delete ProductionSetVec[i];
}




static string& ClearAllSpace(string& str)
{
    size_t index = 0;
    if (!str.empty())
    {
        while ((index = str.find(' ', index)) != string::npos)
        {
            str.erase(index, 1);
        }
    }
    return str;
}
// right ������ <> {} '' [] |
static void analysis_right(vector<Sign>& RightSign, const string& str)
{
    istringstream sin(str);
    string s;
    Sign sign;
    while (sin >> sign.str) {
        sign.type = (sign.str[0] == '<' && sign.str[sign.str.length() - 1] == '>' ? NonTerminator : Terminator);
        RightSign.push_back(sign);
    }
}





