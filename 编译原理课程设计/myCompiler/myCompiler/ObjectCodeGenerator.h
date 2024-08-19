#pragma once
#pragma execution_character_set("utf-8") 
#include "Optimizer.h"

//待用活跃信息
struct InfoItem {
    string op1;
    string op2;
};

//待用信息表的表项
struct InfoTableItem {
    int num;//四元式序号
    InfoItem lval;
    InfoItem lop;
    InfoItem rop;
};

struct VarInfo {
    int addr;
    int isnew;//内存中是否是新的,1表示新的，0表示旧的需要写回
};

const int begin_addr = 0x10010000;//内存起始地址为0x10010000，这个需要根据机器的起始地址做出修改
const int addr_unit = 4;//表示是32位系统
const vector<string> RegisterName = { "$t0","$t1","$t2","$t3","$t4","$a0","$a1","$a2","$a3" };


class myCompiler;
class ObjectCodeGenerator {
    friend myCompiler;
private:
    vector<Var> VarTable;//变量表
    map<int, VarInfo> VarAddr;//变量的一些信息，下标与变量表一致
    vector<Func> FuncTable;//函数表
    vector<Code> MiddleCodeTable;//中间代码表
    vector<CODE_BLOCK> CodeBlocks;//代码基本块
    vector<string> ObCodeTable;//目标代码表
    int cur_addr;//目前分配的内存地址
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
        {"=","add"},//使用 add A,$t0,B来做 A=B
        {"Par","add"},
        {"Call","jal"}
    };//构建操作数和指令之间的关系
    map<string, vector<string>> RVALUE = {
        //            {"$t0",vector<string>{}},//t0中恒为0
                    {"$t1",vector<string>{}},
                    {"$t2",vector<string>{}},
                    {"$t3",vector<string>{}},
                    {"$t4",vector<string>{}},
                    {"$a0",vector<string>{}},//下面四个寄存器是用来函数传参
                    {"$a1",vector<string>{}},
                    {"$a2",vector<string>{}},
                    {"$a3",vector<string>{}},
                    {"$v0",vector<string>{}}//这个寄存器储存函数的返回值
    };
    vector<int> Ocode2Obcode;
    map<string, vector<string>> AVALUE;
    map<string, string> VRVALUE;//形参的对应关系 $a0 - param1 $a1 - param2 $a2 - param3 $a3 - param4
    map<int, map<string, InfoItem>> InfoTabletest;//每一时刻的所有变量的待用活跃信息
    vector<InfoTableItem> InfoTable;//待用活跃信息表
    map<string, InfoItem> InfoChain;//待用活跃链
    vector<int> CurValDims;//如果变量为数组的话，需要取这里的维度信息
    ofstream ObjectOut;
    int CurOcodeIndex;//第几个语句，用来获取待用活跃信息表
    int Varindex;//分配变量时寻找到的变量下标
    vector<int>JumpPos;//跳转语句的位置
public:

    ObjectCodeGenerator(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1, vector<CODE_BLOCK> CodeBlocks1, string filename);
    ~ObjectCodeGenerator();
    //构建待用活跃信息表
    void CreateInfoTable(CODE_BLOCK& CodeBlock);
    //判断变量类型，是数组变量还是普通变量，0为普通变量，1为数组变量，-1表示没有找到
    int JudgeValType(string var);
    //为变量分配内存
    void AllocVarAddr(CODE_BLOCK& CodeBlock);
    //在生成目标代码之前修改基本块语句，添加label语句
    void ModifyCode();
    //处理完基本块后更新RVALUE以及AVALUE
    void GenOcodeEnd(CODE_BLOCK& CodeBlock);
    //分配寄存器，其中会对AVALUE以及RVALUE做出修改，在之前需要判断变量类型，0表示为普通变量分配，1表示为参数变量分配
    string AllocRegister(int& is_arr, string var, int type, CODE_BLOCK& CurBlock, bool is_A, bool is_B = false);
    //查询某一变量是否在寄存器中，返回找到的寄存器名字，没找到返回None
    string FindVarInReg(string var);
    //查找到Var后需要做的删除操作
    void DeleteVar(string var, string reg);
    //寻找尚未分配的寄存器, 返回None表示没有找到
    string FindUnAllocReg(string var, int type);
    //在已被分配的寄存器中选择一个寄存器
    string FindAllocReg(string var, int type, CODE_BLOCK& CurBlock);
    //生成目标代码
    void GenOcode(CODE_BLOCK& CodeBlock);
    //Forward 整个生成流程在这里面
    void Forward();
    //每处理完一个基本块，需要清理一些变量
    void GenOcodeClear(CODE_BLOCK& CodeBlock);
    //在生成目标代码之前需要更新一下基本块的变量列表
    void updateVarlist();
    //添加变量
    void addOVar(CODE_BLOCK& CurBlock, string name, Pos pos);
    //寻找变量,没找到返回-1
    int FindVar(string var, CODE_BLOCK& CurBlock);
    //计算数组下标对应的位置
    int CalArrayPos(string var);
    //重新计算跳转语句的下标
    void CalJumpTar();
};

ostream& operator << (ostream& os, const vector<string> s);

