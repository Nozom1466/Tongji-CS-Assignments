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
    this->Foresee = MyForesee;//重载了=，因为要将string对象深拷贝
    this->id = BaseProdction.id;
    this->LeftSign = BaseProdction.LeftSign;
    for (int i = 0; i < (int)BaseProdction.RightSign.size(); i++)
        this->RightSign.push_back(BaseProdction.RightSign[i]);
}

void LR1Production::PopSign()
{
    this->DotPosition++;
}

LR1Production* LR1Production::next_LR1Production(Sign InSign)//通过符号生成下一些项目
{
    if (this->DotPosition < (int)this->RightSign.size() && InSign.str == this->RightSign[this->DotPosition].str && InSign.type == this->RightSign[this->DotPosition].type)//说明可以转移
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




/*****************文法分析过程 ********************/

GrammaAnalysis::GrammaAnalysis()
{
    TableOut = Table_FILE;
    GraphOut = TREE_PNG_FILE;
    GraphOutDot = TREE_DOT_FILE;


    this->SemanticProcess.MiddleVarIndex = 0;
    this->SemanticProcess.Level = 0;//层级
    this->SemanticProcess.ProcNo = 0;//目前的过程编号
    this->SemanticProcess.ProcNoStack.push_back(this->SemanticProcess.ProcNo);//过程栈
    this->SemanticProcess.semanticerror = None;
}


//生成所有的项目
int GrammaAnalysis::ProduceFormsGenerate(string file)
{
    ifstream fin;
    fin.open(file, ios::in);//故意十进制打开,getline行尾统一处理
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
        //注释
        if (str[0] == '$') {
            continue;
        }
        auto pos = str.find("::=");
        if (pos == str.npos) {
            ferr << "error at line" << i << " str:" << str << endl;
            return GRAMMAR_ERROR;
        }
        ++j;//记录有效行 寻找开始符
        ProduceForms pf;
        //左部直接取 认为第一个为开始符
        pf.LeftSign.str = str.substr(0, pos);
        ClearAllSpace(pf.LeftSign.str);
        pf.LeftSign.type = (j == 1 ? StartTerminator : NonTerminator);
        string right_s = str.substr(pos + 3);
        //每个|是一个右部
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
        //最后一个
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


//求某个符号的FIRST集
//对于终结符，FIRST集就是自己
//对于非终结符
    /*
    若X ∈VN，且有产生式X→a…,a∈VT，则把a加入到FIRST（X）中，若有X→ε，则把ε加入FIRST（X);
    若X∈VN , 且X→Y … , Y∈VN, 则把FIRST (Y) - {ε}加到FIRST (X)中
    若X→Y1Y2 … Yk,Y1, Y2, … ,Yi-1 ∈ VN,ε∈FIRST(Yj)
    则把(1<= j <= i -1)FIRST (Yi) - {ε}加到FIRST (X)中。
    特别地，若ε∈FIRST (Yj)(1<=j <= k )，则ε∈FIRST(X)
    */
set<Sign> GrammaAnalysis::GetFirstSign(Sign InSign)
{
    set<Sign> sign_vec;
    if (InSign.type == Terminator) {
        sign_vec.insert(InSign);
        return sign_vec;
    }
    //存在就返回
    if (!first_table[InSign.str].empty()) {
        return first_table[InSign.str];
    }
    //遍历产生式
    for (auto pf_p : ProduceFormsVec) {
        //左部相等
        if (pf_p->LeftSign == InSign) {
            //第一个是Terminator
            if (pf_p->RightSign[0].type == Terminator) {
                first_table[InSign.str].insert(pf_p->RightSign[0]);
            }
            //不是的话 遍历右部
            else {
                size_t length = 0;
                for (auto& right : pf_p->RightSign) {
                    auto right_set = GetFirstSign(right);
                    int zero = 0;
                    for (auto& right_set_sign : right_set)
                    {
                        //去除first-{ε}
                        if (right_set_sign.str == "空") {
                            zero = 1;
                            continue;
                        }
                        //非空插入
                        first_table[InSign.str].insert(right_set_sign);
                    }
                    //说明没有{ε} 不必加入下一个的first
                    if (!zero) {
                        break;
                    }
                    ++length;
                }
                //遍历完右部都有{ε} 将{ε}加入first
                if (length == pf_p->RightSign.size()) {
                    first_table[InSign.str].insert(Sign("空", Terminator));
                }
            }
        }
    }

    return first_table[InSign.str];
}


//输入连续字符串，得到first集
//1.对于符号串α= X1X2… Xn，构造 FIRST (α)
//2.若对所有的 Xj ,1<=j<= i -1, ε∈FIRST (Xj), 则把FIRST(Xi) -{ε}加到FIRST(α)中
//3.若对所有的 Xj, 1 <= j <= n, ε∈FIRST(Xj), 则把ε加到FIRST(α)中
set<Sign> GrammaAnalysis::GetFirstSign(vector<Sign> InSign)
{
    set<Sign> set_sign;
    size_t i;
    for (i = 0; i < InSign.size(); ++i) {
        auto right_set = GetFirstSign(InSign[i]);
        int zero = 0;
        for (auto& right_set_sign : right_set)
        {
            //去除first-{ε}
            if (right_set_sign.str == "空") {
                zero = 1;
                continue;
            }
            //非空插入
            set_sign.insert(right_set_sign);
        }
        //说明没有{ε} 不必加入下一个的first
        if (!zero) {
            break;
        }
    }
    if (i == InSign.size()) {
        set_sign.insert(Sign("空", Terminator));
    }
    return set_sign;
}

//得到项目集闭包
void GrammaAnalysis::GetFull(ProductionSet& MyDFAState)
{
    int CurCheckPosition = 0;
    for (; CurCheckPosition < (int)MyDFAState.LR1Productions.size(); CurCheckPosition++)
    {
        //考虑已经要归约的式子，即点在最后
        LR1Production Check = *MyDFAState.LR1Productions[CurCheckPosition];
        if (Check.DotPosition == (int)Check.RightSign.size())//说明是归约串
            continue;
        for (int i = 0; i < (int)ProduceFormsVec.size(); i++)
        {
            if (ProduceFormsVec[i]->LeftSign.str == Check.RightSign[Check.DotPosition].str && ProduceFormsVec[i]->LeftSign.type == Check.RightSign[Check.DotPosition].type)
            {
                //先得到要的first集
                vector<Sign> FirstInSign;
                set<Sign> FirstSet;
                for (int j = Check.DotPosition + 1; j < (int)Check.RightSign.size(); j++)
                    FirstInSign.push_back(Check.RightSign[j]);
                FirstInSign.push_back(Check.Foresee);

                FirstSet = GetFirstSign(FirstInSign);
                //记录一下生成的first集合
                string FirstInSignStr;
                for (int z = 0; z < (int)FirstInSign.size(); z++)
                {
                    FirstInSignStr += " ";
                    FirstInSignStr += FirstInSign[z].str;
                }
                first_table[FirstInSignStr] = FirstSet;
                //通过first集以及筛选好的产生式来生成要加入集合的LR1项目
                set<Sign>::iterator iter;
                for (iter = FirstSet.begin(); iter != FirstSet.end(); ++iter)
                {
                    LR1Production* AddProduction = (LR1Production*)new LR1Production(0, *iter, *ProduceFormsVec[i]);
                    //查重
                    int IsFind = 0;
                    for (int k = 0; k < (int)MyDFAState.LR1Productions.size(); k++)
                    {
                        if (*MyDFAState.LR1Productions[k] == *AddProduction)
                            IsFind = 1;
                    }
                    if (!IsFind)//没有重复
                    {
                        MyDFAState.LR1Productions.push_back(AddProduction);
                    }
                }
            }
        }

    }
}

//建立DFA
void GrammaAnalysis::BuildDFA()
{
    //1. 构造初始集合
    ProductionSet* InitSet = new ProductionSet();
    for (int i = 0; i < (int)ProduceFormsVec.size(); i++)
    {
        if (ProduceFormsVec[i]->LeftSign.type == StartTerminator)//找到开始符号的产生式了
        {
            LR1Production* StartPd = new LR1Production(0, Sign{ "#", Terminator }, *ProduceFormsVec[i]);
            //构造初始集合
            InitSet->id = 0;//初始集的id
            InitSet->LR1Productions.push_back(StartPd);
            GetFull(*InitSet);//得到闭包
            break;
        }
    }
    ProductionSetVec.push_back(InitSet);
    //2. 从初始集合出发开始构造DFA
    int SetId = 1;
    int CurCheckPosition = 0;
    //2.1 先得到可以用来转移的符号
    for (; CurCheckPosition < (int)ProductionSetVec.size(); CurCheckPosition++)//对于每一个集合
    {
        vector<Sign> TranSign;
        ProductionSet* CurSet = ProductionSetVec[CurCheckPosition];
        for (int j = 0; j < (int)CurSet->LR1Productions.size(); j++)//对于每一个LR1项目
        {
            int IsFind = 0;
            for (int k = 0; k < (int)TranSign.size(); k++)
                if (CurSet->LR1Productions[j]->DotPosition < (int)CurSet->LR1Productions[j]->RightSign.size() && TranSign[k] == CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition])
                    IsFind = 1;
            if (!IsFind)
                if (CurSet->LR1Productions[j]->DotPosition < (int)CurSet->LR1Productions[j]->RightSign.size() && CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition].str != "空")//对于非归约串
                    TranSign.push_back(CurSet->LR1Productions[j]->RightSign[CurSet->LR1Productions[j]->DotPosition]);//加入到集合
        }
        //2.2 通过每一个转移符号来构造项目集
        for (int k = 0; k < (int)TranSign.size(); k++)//对于每一个转移符号
        {
            ProductionSet* AddPdSet = new ProductionSet();//要添加的项目集
            for (int j = 0; j < (int)CurSet->LR1Productions.size(); j++)//对于每一个LR1项目
            {
                LR1Production* AddPd = CurSet->LR1Productions[j]->next_LR1Production(TranSign[k]);
                if (AddPd == NULL)//没有产生
                    continue;
                AddPdSet->LR1Productions.push_back(AddPd);//产生了，加入集合
            }
            GetFull(*AddPdSet);//得到闭包
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
                CurSet->NextNode.push_back(AddPdSet);//建立关系
                ProductionSetVec.push_back(AddPdSet);//加入项目集
            }
            else
            {
                CurSet->MyMap[TranSign[k].str] = k;
                CurSet->NextNode.push_back(AddPdSet);//建立关系
            }
        }
    }

}

//检测文法有无规约冲突
bool GrammaAnalysis::Conflict_Detection(const Actions& action, pair<int, Sign>& tableindex, int i, LR1Production* j)
{
    ofstream ferr(GRAMMAR_ERROR_FILE,ios::out);
    if (ActionTable.count(tableindex)) {
        if (ActionTable[tableindex] != action) {
            ferr << "error 归约冲突!已经有action[" << i << "," << j->Foresee.str << "]=" << action <<endl<<"不符合LR1文法" << endl;
            return 1;
        }
    }
    return 0;
}


//建立ACtiongo表
int GrammaAnalysis::BuildActionGo()
{
    //goto表
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
    //action表
    for (int i = 0; (int)i < ProductionSetVec.size(); i++) {
        for (auto j : ProductionSetVec[i]->LR1Productions) {
            //归约
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
                    if (sign.str == "空") {
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


//打印action和goto表
void GrammaAnalysis::PrintActionGoToFile()
{
    ofstream fout;
    fout.open(TableOut, ios::out);
    pair<int, Sign>tableindex;
    Actions action;
    fout << "状态,";
    for (auto& terminator : AllTerminator) {
        if (terminator.str == ",")
            fout << "\",\",";
        else if (terminator.str == "空")
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
            if (tableindex.second.str == "空") {
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

//画出整个DFA
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
            // 此处假设符号可以直接输出，实际可能需要转换
            fdot << j.first << "\" ,fontname=\"FangSong\"];\n";
        }
    }

    fdot << "}\n";
    fdot.close();
    return SUCCESS;
}



/************************规约过程**************************************/


int GrammaAnalysis::Forward(lexer *MyWordSets)//前向传播，生成语法树
{
   // int cnt = 0;
    WordSets = MyWordSets;
    int NodeId = 0;//用来区分不同的Node
    //初始化符号栈和状态栈
    SignStack.push(TreeNode{ "#","#",StartTerminator });
    StateStack.push(0);
    int step = 0;

    ofstream  Reduction(REDUCTION_PROCESS_FILE, ios::out);//记录规约过程
    // 输出第一行
    //Reduction << "STEP: " << step << endl;
  
    ofstream ErrorList(PARSE_ERROR_FILE,ios::out);//记录规约过程的错误


    //输出dot文件
    ofstream GraphFile(TREE_DOT_FILE, ios::in | ios::trunc);//存在则删除
    GraphFile << ("digraph G{") << endl;
    GraphFile << ("edge[fontname = \"FangSong\"];") << endl;
    GraphFile << ("node[shape = box, fontname = \"FangSong\" size = \"20,20\"];") << endl;
    //从词法分析器中取元素，然后判断
    TreeNode InSign;//输入的符号
    SemanticTreeNode SInSign;//语义栈的符号
    int TopState;//栈顶的状态
    int WordPos = 0;//词语的位置
    Actions Action;//动作

    //debug 变量
    vector<TreeNode> SignStackVec;
    vector<int> StateStackVec;
    int i = 0;
    while (WordPos < (int)WordSets->token_vec.size())
    {
        
        step++;
        Reduction << "STEP: " << step << endl;
        //初始化要使用的对象
        if (WordSets->token_vec[WordPos].type == 1)//从词法分析器中取出元素
        {
            InSign = TreeNode{ "ID" ,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;//初始化SInSign的终结符
            SInSign.type = Terminator;
               
        }
        else if (WordSets->token_vec[WordPos].type == 2)
        {
            InSign = TreeNode{ "num" ,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;
            //cout << SInSign.Name << endl;
            SInSign.ValTpye = FLOAT1;
            sscanf(SInSign.Name.c_str(), "%d", &SInSign.ival);
            sscanf(SInSign.Name.c_str(), "%f", &SInSign.fval);//初始化SInSign的终结符
            //cout << SInSign.ival << ' ' << SInSign.fval << endl;
            SInSign.type = Terminator;
              
        }
        else
        {
            InSign = TreeNode{ WordSets->token_vec[WordPos].word,WordSets->token_vec[WordPos].word + " id=" + to_string(NodeId++),Terminator };
            SInSign.Name = WordSets->token_vec[WordPos].word;
            SInSign.type = Terminator;//初始化SInSign的终结符
              
           
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
        if (ActionTable.count(pair<int, Sign>{TopState, Sign{ InSign.str,InSign.type }}) == 0)//说明没有找到
        {
            
            ErrorList << "语法分析错误：\n" << "找不到" << WordSets->token_vec[WordPos].word << "的转移, ";
            ErrorList << "出错位置在第" << WordSets->token_vec[WordPos].line << "行\n";
            this->ErrorLine = WordSets->token_vec[WordPos].line;
          
            return ACTION_ERROR;
            //            return 0;
        }
        Action = ActionTable[pair<int, Sign>{TopState, Sign{ InSign.str,InSign.type }}];
        if (Action.Type == Acc)//接受
        {
            Reduction<<"Success!!!";
            break;
        }
        else if (Action.Type == Resolution)//归结
        {
           // cout << cnt++;
            ProduceForms* ResoProduce = ProduceFormsVec[Action.ProduceFormsId];//将要归结的语句找到，因为下标和id有一一对应关系
            //下面是语义动作
            //this->SemanticProcess.semanticerror = None;
            this->SemanticProcess.SemanticLeftSign = ResoProduce->LeftSign;
            //cout << ResoProduce->id << ' ' << ResoProduce->LeftSign.str<<endl;
            this->SemanticProcess.SemanticAction(ResoProduce);
            //cout << this->SemanticProcess.semanticerror;
            if (this->SemanticProcess.semanticerror != None)
            {
                
                this->ErrorLine = this->WordSets->token_vec[WordPos-1].line;
                SDisError(WordPos, this->SemanticProcess.semanticerror);//打印语义错误信息
                return SEMANTIC_ERROR;
            }
                
        
            //下面是语法分析中的归结
            int PopSize = ResoProduce->RightSign.size();
            if (ResoProduce->RightSign[0].str == "空")
                PopSize = 0;
            while (PopSize--)//符号栈顶和状态栈顶元素全部出栈
            {
                string OutStr = "\"" + SignStack.top().StrType + "\"" + " -> " + "\"" + ResoProduce->LeftSign.str + " id=" + to_string(NodeId) + "\"" + ";";
                GraphFile << (OutStr.c_str()) << endl;
                SignStack.pop();
                StateStack.pop();
            }
            SignStack.push(TreeNode{ ResoProduce->LeftSign.str,ResoProduce->LeftSign.str + " id=" + to_string(NodeId++) ,ResoProduce->LeftSign.type });//产生式左部入栈
            TopState = StateStack.top();
            StateStack.push(GoTable[pair<int, Sign>{TopState, ResoProduce->LeftSign}]);
         
            stringstream TempOutput;
            TempOutput << "规约 :" << ResoProduce << endl;

            Reduction << TempOutput.str() << endl;

        }
        else//移进
        {
            StateStack.push(Action.State);
            SignStack.push(InSign);
           // cout << SInSign.ival <<' '<<SInSign.fval << endl;
            this->SemanticProcess.SemanticStack.push(SInSign);
            WordPos++;
            Reduction << "移进" << endl<<endl;
        }
        i++;
       
    }
    GraphFile << ("}") << endl;
    GraphFile.close();//关闭文件
    Reduction.close();

    return SUCCESS;
}


//接受语义错误
void GrammaAnalysis::SDisError(int WordPos, SemanticError ErrType)
{
    ofstream fout(SEMANTIC_ERROR_FILE, ios::out);
     
    string ErrorInfo = "语义错误:";
    if (ErrType == NoFuncName)
    {
        ErrorInfo += "调用未定义的函数:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "\n";
    }
    else if (ErrType == FuncParLenError)
    {
        ErrorInfo += "函数:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "参数定义与调用不一致\n";
    }
    else if (ErrType == NoMainName)
    {
        ErrorInfo += "未定义main函数\n";
    }
    else if (ErrType == FuncReturnErr)
    {
        ErrorInfo += "函数:";
        ErrorInfo += this->SemanticProcess.ErrorFuncName;
        ErrorInfo += "返回类型与声明类型不一致\n";
    }
    else if (ErrType == NoVar)
    {
        ErrorInfo += "未定义变量";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
    }
    else if (ErrType == RedefineVar)
    {
        ErrorInfo += "重定义变量";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
  
       
    }
    else if (ErrType == ArrayIndexError)
    {
        ErrorInfo += "数组下标使用错误";
        ErrorInfo += this->SemanticProcess.ErrorVarName;
        ErrorInfo += "\n";
    }
    ErrType = None;
    fout << ErrorInfo;
    fout << "出错位置: 第" << this->WordSets->token_vec[WordPos-1].line << "行\n";
    fout.close();
}


//释放内存
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
// right 可能是 <> {} '' [] |
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





