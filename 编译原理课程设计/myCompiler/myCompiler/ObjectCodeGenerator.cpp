#include "ObjectCodeGenerator.h"
using namespace std;


//itoa
string myitoa(double f)
{
    int g = f;
    char temp[100] = { 0 };
    sprintf(temp, "%x", g);
    string res = temp;
    return res;
}

//������һЩ���ߺ���

int OpType(Code MiddleCode)
{
    if (MiddleCode.op == "return")
        return 5;
    if (MiddleCode.op == "label")
        return 4;
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

int OpType1(Code MiddleCode)
{
    if (MiddleCode.op == "Par")
        return 4;
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



ObjectCodeGenerator::ObjectCodeGenerator(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1, vector<CODE_BLOCK> CodeBlocks1, string filename)
{
    

    VarTable = VarTable1;
    FuncTable = FuncTable1;
    MiddleCodeTable = MiddleCodeTable1;
    CodeBlocks = CodeBlocks1;
    ObjectOut.open(filename.c_str(), ios::out);
    cur_addr = begin_addr;
}

ObjectCodeGenerator::~ObjectCodeGenerator()
{
    ObjectOut.close();
}

void ObjectCodeGenerator::updateVarlist()
{
    for (int i = 0; i < (int)CodeBlocks.size(); i++)
    {
        CodeBlocks[i].varlist.clear();//��ջ������е����б���
    }
    for (int i = 0; i < (int)CodeBlocks.size(); i++)//����ÿһ���������Ѱ�ұ���
    {//Call, @Backward
        CODE_BLOCK& CurBlock = CodeBlocks[i];
        for (int j = 0; j < (int)CurBlock.Ocode.size(); j++)//Ѱ�ұ���
        {
            if (CurBlock.Ocode[j].op == "Call")
                continue;
            if (CurBlock.Ocode[j].op1 == "@Backward")
                continue;
            Pos pos;
            string name;
            pos.lno = i; pos.ipos = lvar; name = CurBlock.Ocode[j].linkres;
            addOVar(CurBlock, name, pos);
            pos.lno = i; pos.ipos = opl; name = CurBlock.Ocode[j].op1;
            addOVar(CurBlock, name, pos);
            pos.lno = i; pos.ipos = opr; name = CurBlock.Ocode[j].op2;
            addOVar(CurBlock, name, pos);
        }
    }
}

//������ӡ
void ObjectCodeGenerator::Forward()
{
    cout << endl;
    //����һ�±����б�
    updateVarlist();
    //����һ�»�����
    ModifyCode();
    //����ÿһ�����������һЩ����
    
    for (int i = 0; i < (int)CodeBlocks.size(); i++)
    {
        GenOcodeClear(CodeBlocks[i]);
        GenOcode(CodeBlocks[i]);
        GenOcodeEnd(CodeBlocks[i]);
        Ocode2Obcode.push_back(ObCodeTable.size());
    }
    //����һ����ת���
    CalJumpTar();
    ObjectOut << ObCodeTable << endl;
}




void ObjectCodeGenerator::CalJumpTar()
{
    int tag_num = 0;
    vector<pair<int, string>> insert_items;
    for (int i = 0; i < (int)JumpPos.size(); i++)
    {
        if (i == 0)
        {
            ObCodeTable[0] = "j main";
            continue;
        }
        string curcode = ObCodeTable[JumpPos[i]];
        vector<string> items;
        string cur_item;
        int j = 0;
        while (j < (int)curcode.size())
        {
            if (curcode[j] == ' ')
            {
                items.push_back(cur_item);
                cur_item.clear();
            }
            else
                cur_item.push_back(curcode[j]);
            j++;
        }
        items.push_back(cur_item);
        int tarpos = atoi(items[1].c_str());
        tarpos = Ocode2Obcode[tarpos];
        string tag;
        tag = "label" + to_string(tag_num++);
        bool is_repeat = false;
        for (int j = 0; j < (int)insert_items.size(); j++)//����
        {
            if (insert_items[j].first == tarpos)
            {
                is_repeat = true;
                tag = insert_items[j].second;
                break;
            }
        }
        if (!is_repeat)
            insert_items.push_back(pair<int, string>{tarpos, tag});
        items[1] = tag;
        curcode.clear();
        if (items.size() == 4)//˵����j<����䣬��Ҫ����һ��˳��
        {
            swap(items[1], items[2]);
            swap(items[2], items[3]);
        }
        for (int j = 0; j < (int)items.size(); j++)
        {
            if (j == 0)
                curcode += items[j];
            else
            {
                curcode += ' ';
                curcode += items[j];
            }
        }
        ObCodeTable[JumpPos[i]] = curcode;
    }
    for (int i = 0; i < (int)insert_items.size(); i++)
    {
        ObCodeTable.insert(ObCodeTable.begin() + insert_items[i].first, insert_items[i].second + ":");
        int tarpos = insert_items[i].first;
        for (int j = i + 1; j < (int)insert_items.size(); j++)
            if (insert_items[j].first >= tarpos)
                insert_items[j].first++;
    }
}

void ObjectCodeGenerator::GenOcodeClear(CODE_BLOCK& CodeBlock)
{
    InfoTabletest.clear();
    InfoChain.clear();
    InfoTable.clear();
    CurValDims.clear();
    CurOcodeIndex = 0;
}

void ObjectCodeGenerator::GenOcodeEnd(CODE_BLOCK& CodeBlock)
{
    for (auto iter = RVALUE.begin(); iter != RVALUE.end(); iter++)//��������RVALUE
    {
        string Reg = iter->first;
        vector<string> Vallist = iter->second;
        for (int i = 0; i < (int)Vallist.size(); i++)
        {
            bool is_active = false;
            for (int j = 0; j < (int)CodeBlock.Outvarlist.size(); j++)
            {
                if (CodeBlock.Outvarlist[j].name == Vallist[i])
                {
                    is_active = true;
                    break;
                }
            }
            if (is_active)//��Ҫд���ڴ�
            {
                int varpos = FindVar(Vallist[i], CodeBlock);
                if (varpos == -1)
                {
                    cout << "�����ˣ���������û���ҵ�" << endl;
                    return;
                }
                int myvartype = JudgeValType(Vallist[i]);
                VarInfo& varinfo = VarAddr[varpos];

                int varaddr = varinfo.addr;
                if (myvartype == 1)//˵��������Ԫ��
                {
                    varaddr += CalArrayPos(Vallist[i]) * addr_unit;//��������Ԫ�ص�λ��
                    //ObCodeTable.push_back("sw " + Reg + " 0x" + myitoa(varaddr) + "($t0)");//д���ڴ�
                }
                varinfo.isnew = 1;//�µ�
            }
            DeleteVar(Vallist[i], Reg);//ɾ���ñ���
        }
    }
}


void ObjectCodeGenerator::GenOcode(CODE_BLOCK& CodeBlock)
{
    //�������û�Ծ��Ϣ��
    CreateInfoTable(CodeBlock);
    //��ʼ����Ŀ����룬���������������ܳ��ܳ�
    vector<Code> OcodeTable = CodeBlock.Ocode;
    //Ϊÿ�����������ڴ��ַ
//    if(OcodeTable[0].op != "label")
    AllocVarAddr(CodeBlock);

    int valtype = 0;
    int Funcvaltype = 1;

    for (int i = 0; i < (int)OcodeTable.size(); i++)
    {
        int A_arr = 0;
        CurOcodeIndex = i;
        string A = OcodeTable[i].linkres;
        string B = OcodeTable[i].op1;
        string C = OcodeTable[i].op2;
        string op = OcodeTable[i].op;
        //cout << op << ", " << A << ", " << B << ", " << C << endl;
        int optype = OpType(OcodeTable[i]);//δ���������������
        if (optype == 0)// j =
        {
            if (op == "j")
            {
                GenOcodeEnd(CodeBlock);
                ObCodeTable.push_back("j " + A);
            }
            if (op == "=")
            {
                string regA = AllocRegister(A_arr,A, valtype, CodeBlock, true);
                string regB = AllocRegister(A_arr,B, valtype, CodeBlock, false);
                if (B[0] >= '0' && B[0] <= '9') {
                    ObCodeTable.push_back("addi " + regA + " $t0 " + regB);
                }
                else {
                    ObCodeTable.push_back("add " + regA + " $t0 " + regB);
                }

                if (JudgeValType(A) == 1)//����ֵ������
                {
                    int varpos = FindVar(A, CodeBlock);
                    VarInfo& varinfo = VarAddr[varpos];

                    int varaddr = varinfo.addr;
                    varaddr += CalArrayPos(A) * addr_unit;;//��������Ԫ�ص�λ��
                    ObCodeTable.push_back("sw " + regA + " 0x" + myitoa(varaddr) + "($t0)");

                }


            }
        }
        else if (optype == 1)//һԪ����� neg $t2 $t1
        {
            string regA = AllocRegister(A_arr,A, valtype, CodeBlock, true);
            string regB = AllocRegister(A_arr,B, valtype, CodeBlock, false);
            ObCodeTable.push_back("neg " + regA + " " + regB);
        }
        else if (optype == 21 || optype == 22)//��Ԫ�����
        {
            //���B���������Ļ�����ҪΪ�������ʱ�Ĵ���
            string regA = AllocRegister(A_arr,A, valtype, CodeBlock, true);
            string regB;
            if (isFloat(B.c_str()))
                regB = AllocRegister(A_arr,B, valtype, CodeBlock, false, true);
            else
                regB = AllocRegister(A_arr,B, valtype, CodeBlock, false);

            string regC;

            if (isFloat(C.c_str())) {
                //cout << 555;
                regC = AllocRegister(A_arr,C, 1, CodeBlock, false, true);
                //cout << 555 << endl;
            }
            else
                regC = AllocRegister(A_arr, C, valtype, CodeBlock, false);

            //cout << regA << ' ' << regB << ' ' << regC << endl;
            if (optype == 21)
                GenOcodeEnd(CodeBlock);
            ObCodeTable.push_back(myop2cmd[op] + " " + regA + " " + regB + " " + regC);
            if (isFloat(B.c_str()))
                DeleteVar(B, regB);
            if (isFloat(C.c_str()))
                DeleteVar(C, regC);
            if (A_arr) {
                int varpos = FindVar(A, CodeBlock);
                VarInfo& varinfo = VarAddr[varpos];

                int varaddr = varinfo.addr;
                varaddr += CalArrayPos(A) * addr_unit;;//��������Ԫ�ص�λ��
                ObCodeTable.push_back("sw " + regA + " 0x" + myitoa(varaddr) + "($t0)");
            }
        }
        else if (optype == 3)//@BackReturn
        {
            //Ϊʵ������������Ĵ���
            for (int j = i - 1;; j--)//��ǰѰ��par
            {
                if (OcodeTable[j].op != "Par")
                    break;
                string ParB = OcodeTable[j].op1;
                //�ȷ�����ͨ�Ĵ�����ַ���ٷ�������Ĵ�����ַ
                string regB = AllocRegister(A_arr, ParB, valtype, CodeBlock, false);
                string assignreg = AllocRegister(A_arr, ParB, Funcvaltype, CodeBlock, false);
                if (regB[0] >= '0' && regB[0] <= '9') {
                    ObCodeTable.push_back("addi " + assignreg + " $t0 " + regB);
                }
                else {
                    ObCodeTable.push_back("add " + assignreg + " $t0 " + regB);
                }
            }
            //�ȵ���call
            ObCodeTable.push_back("jal " + OcodeTable[i + 1].op1);
            //�������ذ�$a3��ֵ��Ҫ���صı���
            string regA = AllocRegister(A_arr, A, valtype, CodeBlock, true);
            ObCodeTable.push_back("add " + regA + " $t0 $v0");
            if (JudgeValType(A) == 1)//����ֵ������
            {
                int varpos = FindVar(A, CodeBlock);
                VarInfo& varinfo = VarAddr[varpos];

                int varaddr = varinfo.addr;
                varaddr += CalArrayPos(A) * addr_unit;;//��������Ԫ�ص�λ��
                ObCodeTable.push_back("sw " + regA + " 0x" + myitoa(varaddr) + "($t0)");

            }
            //���a0-a3�Ĵ����е�����ֵ
            RVALUE["$a0"].clear();
            RVALUE["$a1"].clear();
            RVALUE["$a2"].clear();
            RVALUE["$a3"].clear();
        }
        else if (optype == 4)//����Ҫ���𴫲�
        {
            vector<Par> params;
            for (int i = 0; i < (int)FuncTable.size(); i++)//Ѱ�Һ���
            {
                if (FuncTable[i].name == B)//�ҵ���
                {
                    params = FuncTable[i].param;
                    break;
                }
            }
            //�����βεĶ�Ӧ��ϵ
            for (int i = 0; i < (int)params.size(); i++)
                VRVALUE[params[i].name] = "$a" + to_string(i);
            AllocVarAddr(CodeBlock);
            ObCodeTable.push_back(B + ":");
        }
        else if (optype == 5)
        {
            //cout << 555;
            VRVALUE.clear();
            if (B != "-")
            {
                string regB = AllocRegister(A_arr, B, valtype, CodeBlock, false);//��Ҫ��ֵ��$v0
                ObCodeTable.push_back("add $v0 $t0 " + regB);
            }
            GenOcodeEnd(CodeBlock);
            //�����main,������
            cout << "jr" << CodeBlock.id << (int)CodeBlocks.size() - 1 << endl;
            if (CodeBlock.id!= (int)CodeBlocks.size() - 1) {
                ObCodeTable.push_back("jr $ra");
            }
           
        }
        if (optype == 21 || op == "j")//��ת���
            JumpPos.push_back(ObCodeTable.size() - 1);
        if (i != (int)OcodeTable.size() - 1)
            Ocode2Obcode.push_back(ObCodeTable.size());
    }
}

string ObjectCodeGenerator::AllocRegister(int& is_arr, string var, int type, CODE_BLOCK& CurBlock, bool is_A, bool is_B )
{
    string Reg;
    if (type == 0)//˵������ͨ����,�Լ��β�
    {
        //���ж�var�Ƿ�Ϊ������
        if (isFloat(var.c_str()) && is_B == false)//���������Ҳ���ҪΪ�������ʱ�Ĵ���
        {
            //cout << "lalal"<<endl;
            return var;
        }
        Reg = FindVarInReg(var);
        if (Reg != "None")//�ҵ���
        {
           
            if (is_A) //˵��A��ֵ��Ҫ�ı�
                DeleteVar(var, Reg);//RVALUE[Reg]��ɾ��var����AVALUE[var]��ɾ��Reg
            else//˵������A�������ֱ�ӷ��أ�����Ҫ�ı��κζ���
                return Reg;
        }
    }
  
    Reg = FindUnAllocReg(var, type);//��һ���������δ������ļĴ���
    //cout <<Reg<< endl;
    if (type == 1 || is_B == true)//������
    {
        if (is_B)
            ObCodeTable.push_back("addi " + Reg + " $t0 " + var);
        return Reg;
    }
    if (Reg != "None")//�ҵ���
    {
        //Ѱ�ұ���λ��������
        int varpos = FindVar(var, CurBlock);
        if (varpos == -1)
        {
            cout << "�����ˣ���������û���ҵ�" << endl;
            return "None";
        }
        int myvartype = JudgeValType(var);
        VarInfo& varinfo = VarAddr[varpos];

        int varaddr = varinfo.addr;
        if (myvartype == 1)//˵��������Ԫ��
        {
            varaddr += CalArrayPos(var) * addr_unit;//��������Ԫ�ص�λ��
            //����ȡ��ָ��
            if (!is_A) {
                ObCodeTable.push_back("lw " + Reg + " 0x" + myitoa(varaddr) + "($t0)");
            }
            else {
                is_arr = 1;
            }
        }
        //�޸�varinfo��
        if (is_A)//ֵ�����仯�ˣ��þɣ���Ҫд��
            varinfo.isnew = 0;
        return Reg;
    }
    else//��һ�����������
    {
       
        Reg = FindAllocReg(var, type, CurBlock);
        //Ѱ�ұ���λ��������
        int varpos = FindVar(var, CurBlock);
        if (varpos == -1)
        {
            
            cout<< "�����ˣ���������û���ҵ�" << endl;
            return "None";
        }
        int myvartype = JudgeValType(var);
        VarInfo& varinfo = VarAddr[varpos];

        int varaddr = varinfo.addr;
        if (myvartype == 1)//˵��������Ԫ��
        {
            varaddr += CalArrayPos(var) * addr_unit;//��������Ԫ�ص�λ��
            //����ȡ��ָ��
            if (!is_A) {
                ObCodeTable.push_back("lw " + Reg + " 0x" + myitoa(varaddr) + "($t0)");
            }
            else {
                is_arr = 1;
            }
        }
        //�޸�varinfo��
        if (is_A)//ֵ�����仯�ˣ��þɣ���Ҫд��
            varinfo.isnew = 0;
        return Reg;
    }
}

string ObjectCodeGenerator::FindAllocReg(string var, int type, CODE_BLOCK& CurBlock)//ֻ�������RVALUE��AVALUE
{
    //����Ҫ���ǲ�����������Ϊ���������ı���λ�úͼĴ���λ����һһ��Ӧ��
    int min_num = 0x3f3f3f3f;
    string choose_reg;
    for (auto iter = RVALUE.begin(); iter != RVALUE.end(); iter++)//��������RVALUE
    {
        string reg = iter->first;
        int neednum = RVALUE[reg].size();
        if (reg[1] == 't')
        {
            for (int i = 0; i < (int)RVALUE[reg].size(); i++)//����RVALUE�е�ÿ������
            {
                int varpos = FindVar(var, CurBlock);
                if (InfoTabletest[CurOcodeIndex][RVALUE[reg][i]].op2 == "^" || VarAddr[varpos].isnew == 1)
                    neednum--;
            }
            if (neednum < min_num)
            {
                min_num = neednum;
                choose_reg = reg;
            }
        }
    }//�ҵ���ѡ��ı���

    for (int i = 0; i < (int)RVALUE[choose_reg].size(); i++)
    {
        int varpos = FindVar(var, CurBlock);
        string thisvar = RVALUE[choose_reg][i];
        if (InfoTabletest[CurOcodeIndex][RVALUE[choose_reg][i]].op2 == "^" || VarAddr[varpos].isnew == 1)
            ;
        else//��RVALUE�е����б���д�ص��ڴ�֮��
        {
            //Ѱ�ұ���λ��������
            varpos = FindVar(thisvar, CurBlock);
            if (varpos == -1)
            {
                cout << "�����ˣ���������û���ҵ�" << endl;
                return "None";
            }
            int myvartype = JudgeValType(thisvar);
            VarInfo& varinfo = VarAddr[varpos];

            int varaddr = varinfo.addr;
            if (myvartype == 1)//˵��������Ԫ��
            {
                varaddr += CalArrayPos(thisvar) * addr_unit;;//��������Ԫ�ص�λ��
                ObCodeTable.push_back("sw " + choose_reg + " 0x" + myitoa(varaddr) + "($t0)");
            }
            varinfo.isnew = 1;//��ʾд���ڴ棬���µ���
            for (int j = 0; j < (int)AVALUE[thisvar].size(); j++)//����AVALUE
                if (choose_reg == AVALUE[thisvar][j])
                    AVALUE[thisvar].erase(AVALUE[thisvar].begin() + j);
        }
    }
    RVALUE[choose_reg].clear();//���RVALUE
    return choose_reg;
}


int ObjectCodeGenerator::CalArrayPos(string var)
{
    int i = 0;
    int ig_blank_num = 2;
    vector<string> cur_indexs;
    string str;
    while (i < (int)var.length())//�õ��±���Ϣ
    {
        if (ig_blank_num != 0)
        {
            if (var[i++] == ' ')
            {
                ig_blank_num--;
            }
        }
        else
        {
            if (var[i] == ' ')
            {
                cur_indexs.push_back(str);
                str.clear();
            }
            else
                str.push_back(var[i]);
            i++;
        }
    }
    cur_indexs.push_back(str);
    int res = 0;
    for (int i = (int)CurValDims.size() - 1; i >= 0; i--)
    {
        if (i == 0)
            res += atof(cur_indexs[i]);
        else
        {
            res += atof(cur_indexs[i]);
            res *= CurValDims[i - 1];
        }
    }
    return res;
}

int ObjectCodeGenerator::FindVar(string var, CODE_BLOCK& CurBlock)
{
    int j = 0;
    string var_name;
    string procNostr;
    while (j < (int)var.length())
    {
        if (var[j] == ' ')
            break;
        var_name.push_back(var[j++]);
    }
    j++;
    while (j < (int)var.length())
    {
        if (var[j] == ' ')
            break;
        procNostr.push_back(var[j++]);
    }
    int procNo = atof(procNostr);
    for (int i = 0; i < (int)VarTable.size(); i++)
    {
        if (VarTable[i].name == var_name && VarTable[i].ProcNo == procNo)
            return i;
    }
    return -1;
}

string ObjectCodeGenerator::FindUnAllocReg(string var, int type)
{
    //Ϊ��ͨ����Ѱ��δ������ļĴ���
    if (type == 0)
    {
        for (auto iter = RVALUE.begin(); iter != RVALUE.end(); iter++)//��������RVALUE
        {
            string reg = iter->first;
            if (reg[1] == 't')//��ʾ����ͨ����
            {
                if (iter->second.size() == 0)//˵��û�б���������
                {
                    RVALUE[reg].push_back(var);
                    AVALUE[var].push_back(reg);
                    return reg;
                }
            }
        }
        return "None";
    }
    else if (type == 1)    //Ϊ��������Ѱ��δ������ļĴ���
    {
        for (auto iter = RVALUE.begin(); iter != RVALUE.end(); iter++)//��������RVALUE
        {
            string reg = iter->first;
            if (reg[1] == 'a')//��ʾ�ǲ�������
            {
                if (iter->second.size() == 0)//˵��û�б���������
                {
                    RVALUE[reg].push_back(var);
                    return reg;
                }
            }
        }
        return "None";
    }
    return "None";
}



void ObjectCodeGenerator::DeleteVar(string var, string reg)
{
    //�ҵ��Ĵ���֮����Ҫɾ��
    for (int i = 0; i < (int)RVALUE[reg].size(); i++)
        if (RVALUE[reg][i] == var)//�ҵ���RVALU�е�ֵ����Ҫ����ɾ��
            RVALUE[reg].erase(RVALUE[reg].begin() + i);

    for (int i = 0; i < (int)AVALUE[var].size(); i++)
        if (AVALUE[var][i] == reg)//�ҵ���AVALU�е�ֵ����Ҫ����ɾ��
            AVALUE[var].erase(AVALUE[var].begin() + i);
}


string ObjectCodeGenerator::FindVarInReg(string var)
{
    //�Ȼ�ò���������Ϣ���������
    int i = 0;
    string new_var;
    while (1)
    {
        if (var[i] == ' ')//�ո��Ƴ�
            break;
        new_var.push_back(var[i]);
        i++;
    }
    //�ȼ������Ĵ������Ƿ��б������ڣ������еı����������β��������ٶ���
    for (auto iter = VRVALUE.begin(); iter != VRVALUE.end(); iter++)
    {
        string this_var = iter->first;
        string Reg = iter->second;
        if (this_var == new_var)//�ҵ���
            return Reg;
    }
    //Ѱ��ĳһ�����Ƿ��ڼĴ�����
    for (auto iter = RVALUE.begin(); iter != RVALUE.end(); iter++)//��������RVALUE
    {
        string Reg = iter->first;
        vector<string> Vars = iter->second;
        for (int i = 0; i < (int)Vars.size(); i++)
            if (Vars[i] == var)//�ҵ���
                return Reg;
    }
    return "None";
}





void ObjectCodeGenerator::addOVar(CODE_BLOCK& CurBlock, string name, Pos pos)
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

int ObjectCodeGenerator::JudgeValType(string var)
{
    //cout << var << endl;
    int j = 0;
    string var_name;
    string procNostr;
    while (j < (int)var.length())
    {
        if (var[j] == ' ')
            break;
        var_name.push_back(var[j++]);
    }
    j++;
    while (j < (int)var.length())
    {
        if (var[j] == ' ')
            break;
        procNostr.push_back(var[j++]);

    }
    int procNo = atof(procNostr);
    for (auto iter = VRVALUE.begin(); iter != VRVALUE.end(); iter++)
        if (iter->first == var_name)//˵�����β�
            return 2;
    for (int i = 0; i < (int)VarTable.size(); i++)
    {
        if (var_name == VarTable[i].name && procNo == VarTable[i].ProcNo)//��������ͬ��λ����ͬ
        {
            Varindex = i;
            if (VarTable[i].valtype == Array)//˵��������
            {
                CurValDims = VarTable[i].dims;
                return 1;
            }
            else
                return 0;
        }
    }
    return -1;
}

void ObjectCodeGenerator::AllocVarAddr(CODE_BLOCK& CodeBlock)
{
    //����������ββ��÷����ַ����Ҫʹ���±�Varindex�ж��Ƿ��Ѿ������ڴ�
    for (int i = 0; i < (int)CodeBlock.varlist.size(); i++)
    {
        int vartype = JudgeValType(CodeBlock.varlist[i].name);
        if (vartype == -1)
            cout << "no var!!!!" << endl;
        else if (vartype == 0)//��ͨ����
        {
            if (VarAddr.count(Varindex) == 0)//��Ҫ����
            {
                VarAddr[Varindex] = VarInfo{ cur_addr,1 };
                cur_addr += addr_unit;
            }
        }
        else if (vartype == 1)//�������
        {
            if (VarAddr.count(Varindex) == 0)//��Ҫ����
            {
                VarAddr[Varindex] = VarInfo{ cur_addr,1 };
                int arraylen = 1;
                for (int j = 0; j < (int)CurValDims.size(); j++)
                    arraylen *= CurValDims[j];
                cur_addr += arraylen * addr_unit;
            }
        }
        else if (vartype == 2)//�βα���,�������ڴ�
            continue;
    }
}


void ObjectCodeGenerator::CreateInfoTable(CODE_BLOCK& CodeBlock)
{
    //�Ա�����ʼ��
    for (int i = 0; i < (int)CodeBlock.varlist.size(); i++)//�������б���
    {
        bool is_find = false;
        for (int j = 0; j < (int)CodeBlock.Outvarlist.size(); j++)
        {
            if (CodeBlock.varlist[i].name == CodeBlock.Outvarlist[j].name)
            {
                is_find = true;
                break;
            }
        }
        if (is_find)
            InfoChain[CodeBlock.varlist[i].name] = InfoItem{ "^","y" };//��ʼ��
        else
            InfoChain[CodeBlock.varlist[i].name] = InfoItem{ "^","^" };//��ʼ��
    }
    InfoTabletest[CodeBlock.Ocode.size()] = InfoChain;//��ʼ��InfoTabletest
    //�Ӻ���ǰ����
    for (int i = CodeBlock.Ocode.size() - 1; i >= 0; i--)
    {
        string A = CodeBlock.Ocode[i].linkres;
        string B = CodeBlock.Ocode[i].op1;
        string C = CodeBlock.Ocode[i].op2;
        string op = CodeBlock.Ocode[i].op;
        int optype = OpType1(CodeBlock.Ocode[i]);
        if (optype == 0)// j =
        {
            if (op == "=")//(=,op1,-,reslink)
            {
                //                InfoTable.push_back(InfoTableItem{i,InfoChain[A],InfoChain[B],InfoItem{"-","-"}});
                if (!isFloat(A.c_str()))
                    InfoChain[A] = InfoItem{ "^","^" };
                if (!isFloat(B.c_str()))
                    InfoChain[B] = InfoItem{ to_string(i),"y" };
                InfoTabletest[i] = InfoChain;
            }
        }
        else if (optype == 1)// A=op B
        {
            //            InfoTable.push_back(InfoTableItem{i,InfoChain[A],InfoChain[B],InfoItem{"-","-"}});
            if (!isFloat(A.c_str()))
                InfoChain[A] = InfoItem{ "^","^" };
            if (!isFloat(B.c_str()))
                InfoChain[B] = InfoItem{ to_string(i),"y" };
            InfoTabletest[i] = InfoChain;
        }
        else if (optype == 21 || optype == 22)// j<, j==, j<=, j>=, j>, j!=, +, -, *, /
        {
            //            InfoTable.push_back(InfoTableItem{i,InfoChain[A],InfoChain[B],InfoChain[C]});
            if (!isFloat(A.c_str()))
                InfoChain[A] = InfoItem{ "^","^" };
            if (!isFloat(B.c_str()))
                InfoChain[B] = InfoItem{ to_string(i),"y" };
            if (!isFloat(C.c_str()))
                InfoChain[C] = InfoItem{ to_string(i),"y" };
            InfoTabletest[i] = InfoChain;
        }
        else if (optype == 3)//@BackReturn
        {
            //            InfoTable.push_back(InfoTableItem{i,InfoChain[A],InfoItem{"-","-"},InfoItem{"-","-"}});
            if (!isFloat(A.c_str()))
                InfoChain[A] = InfoItem{ "^","^" };
            InfoTabletest[i] = InfoChain;
        }
        else if (optype == 4)//Par A
        {
            //            InfoTable.push_back(InfoTableItem{i,InfoItem{"-","-"},InfoChain[B],InfoItem{"-","-"}});
            if (!isFloat(B.c_str()))
                InfoChain[B] = InfoItem{ to_string(i),"y" };
            InfoTabletest[i] = InfoChain;
        }
        else//�������������Ծ��
        {
            InfoTabletest[i] = InfoChain;
        }
    }

}


void ObjectCodeGenerator::ModifyCode()
{
    int FuncIndex = 0;
    int is_before_return = 0;
    for (int i = 0; i < (int)CodeBlocks.size(); i++)
    {
        if (i == 0)
            continue;
        if (i == 1)
            CodeBlocks[i].Ocode.insert(CodeBlocks[i].Ocode.begin(), Code{ "label",FuncTable[FuncIndex++].name,"-","-" });
        else if (is_before_return)
        {
            CodeBlocks[i].Ocode.insert(CodeBlocks[i].Ocode.begin(), Code{ "label",FuncTable[FuncIndex++].name,"-","-" });
            is_before_return = 0;
        }
        if (CodeBlocks[i].Ocode[CodeBlocks[i].Ocode.size() - 1].op == "return")
            is_before_return = 1;
    }
}

ostream& operator << (ostream& os, const vector<string> s)
{
    for (int i = 0; i < (int)s.size(); i++)
        os << s[i] << endl;
    return os;
}