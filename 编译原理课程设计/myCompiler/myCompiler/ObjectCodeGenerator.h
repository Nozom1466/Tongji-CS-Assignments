#pragma once
#pragma execution_character_set("utf-8") 
#include "Optimizer.h"

//���û�Ծ��Ϣ
struct InfoItem {
    string op1;
    string op2;
};

//������Ϣ��ı���
struct InfoTableItem {
    int num;//��Ԫʽ���
    InfoItem lval;
    InfoItem lop;
    InfoItem rop;
};

struct VarInfo {
    int addr;
    int isnew;//�ڴ����Ƿ����µ�,1��ʾ�µģ�0��ʾ�ɵ���Ҫд��
};

const int begin_addr = 0x10010000;//�ڴ���ʼ��ַΪ0x10010000�������Ҫ���ݻ�������ʼ��ַ�����޸�
const int addr_unit = 4;//��ʾ��32λϵͳ
const vector<string> RegisterName = { "$t0","$t1","$t2","$t3","$t4","$a0","$a1","$a2","$a3" };


class myCompiler;
class ObjectCodeGenerator {
    friend myCompiler;
private:
    vector<Var> VarTable;//������
    map<int, VarInfo> VarAddr;//������һЩ��Ϣ���±��������һ��
    vector<Func> FuncTable;//������
    vector<Code> MiddleCodeTable;//�м�����
    vector<CODE_BLOCK> CodeBlocks;//���������
    vector<string> ObCodeTable;//Ŀ������
    int cur_addr;//Ŀǰ������ڴ��ַ
    map<string, string> myop2cmd = {
        {"j","j"},
        {"j<","blt"},
        {"j<=","ble"},
        {"j>","bgt"},
        {"j>=","bge"},
        {"j==","beq"},
        {"j!=","bne"},
        {"+","add"},
        {"-","sub"},
        {"*","mul"},
        {"/","div"},
        {"-1","neg"},
        {"=","add"},//ʹ�� add A,$t0,B���� A=B
        {"Par","add"},
        {"Call","jal"}
    };//������������ָ��֮��Ĺ�ϵ
    map<string, vector<string>> RVALUE = {
        //            {"$t0",vector<string>{}},//t0�к�Ϊ0
                    {"$t1",vector<string>{}},
                    {"$t2",vector<string>{}},
                    {"$t3",vector<string>{}},
                    {"$t4",vector<string>{}},
                    {"$a0",vector<string>{}},//�����ĸ��Ĵ�����������������
                    {"$a1",vector<string>{}},
                    {"$a2",vector<string>{}},
                    {"$a3",vector<string>{}},
                    {"$v0",vector<string>{}}//����Ĵ������溯���ķ���ֵ
    };
    vector<int> Ocode2Obcode;
    map<string, vector<string>> AVALUE;
    map<string, string> VRVALUE;//�βεĶ�Ӧ��ϵ $a0 - param1 $a1 - param2 $a2 - param3 $a3 - param4
    map<int, map<string, InfoItem>> InfoTabletest;//ÿһʱ�̵����б����Ĵ��û�Ծ��Ϣ
    vector<InfoTableItem> InfoTable;//���û�Ծ��Ϣ��
    map<string, InfoItem> InfoChain;//���û�Ծ��
    vector<int> CurValDims;//�������Ϊ����Ļ�����Ҫȡ�����ά����Ϣ
    ofstream ObjectOut;
    int CurOcodeIndex;//�ڼ�����䣬������ȡ���û�Ծ��Ϣ��
    int Varindex;//�������ʱѰ�ҵ��ı����±�
    vector<int>JumpPos;//��ת����λ��
public:

    ObjectCodeGenerator(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1, vector<CODE_BLOCK> CodeBlocks1, string filename);
    ~ObjectCodeGenerator();
    //�������û�Ծ��Ϣ��
    void CreateInfoTable(CODE_BLOCK& CodeBlock);
    //�жϱ������ͣ����������������ͨ������0Ϊ��ͨ������1Ϊ���������-1��ʾû���ҵ�
    int JudgeValType(string var);
    //Ϊ���������ڴ�
    void AllocVarAddr(CODE_BLOCK& CodeBlock);
    //������Ŀ�����֮ǰ�޸Ļ�������䣬���label���
    void ModifyCode();
    //���������������RVALUE�Լ�AVALUE
    void GenOcodeEnd(CODE_BLOCK& CodeBlock);
    //����Ĵ��������л��AVALUE�Լ�RVALUE�����޸ģ���֮ǰ��Ҫ�жϱ������ͣ�0��ʾΪ��ͨ�������䣬1��ʾΪ������������
    string AllocRegister(int& is_arr, string var, int type, CODE_BLOCK& CurBlock, bool is_A, bool is_B = false);
    //��ѯĳһ�����Ƿ��ڼĴ����У������ҵ��ļĴ������֣�û�ҵ�����None
    string FindVarInReg(string var);
    //���ҵ�Var����Ҫ����ɾ������
    void DeleteVar(string var, string reg);
    //Ѱ����δ����ļĴ���, ����None��ʾû���ҵ�
    string FindUnAllocReg(string var, int type);
    //���ѱ�����ļĴ�����ѡ��һ���Ĵ���
    string FindAllocReg(string var, int type, CODE_BLOCK& CurBlock);
    //����Ŀ�����
    void GenOcode(CODE_BLOCK& CodeBlock);
    //Forward ��������������������
    void Forward();
    //ÿ������һ�������飬��Ҫ����һЩ����
    void GenOcodeClear(CODE_BLOCK& CodeBlock);
    //������Ŀ�����֮ǰ��Ҫ����һ�»�����ı����б�
    void updateVarlist();
    //��ӱ���
    void addOVar(CODE_BLOCK& CurBlock, string name, Pos pos);
    //Ѱ�ұ���,û�ҵ�����-1
    int FindVar(string var, CODE_BLOCK& CurBlock);
    //���������±��Ӧ��λ��
    int CalArrayPos(string var);
    //���¼�����ת�����±�
    void CalJumpTar();
};

ostream& operator << (ostream& os, const vector<string> s);

