#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#pragma execution_character_set("utf-8") 
#include "SynSemAnalysis.h"

//lvar为左值，opl为左操作数，opr为右操作数
enum IPOS { lvar, opl, opr };
// == > >= < <= != EQ,GT,GE,LT,LE,NEQ,J
enum OPERATOR { NEG, ADD, SUB, MUL, DIV, MOV, EQ, GT, GE, LT, LE, NEQ, J };
enum DAGNDOETYPE { DAGJ, DAGA };
const int maxn = 1005;

//用来定位基本块中变量的位置
struct Pos {
    int lno;//行号
    IPOS ipos;//代码中的具体位置
};

//用来优化的变量结构体
struct OVar {
    string name;//变量名
    vector<Pos> pos;//代码块中的位置，从小到大
};

//基本块
struct CODE_BLOCK {
    int id;//数组下表，作为唯一索引
    int vis;//dfs的时候是否被遍历，每次调用FindOutVar函数时需要将其清零
    int BeginIndex;//起始的代码序号
    vector<Code> code;//基本块中的中间代码
    vector<Code> Ocode;//优化后的中间代码
    vector<OVar> varlist;//变量列表
    vector<OVar> Outvarlist;//出基本块还需要使用的变量列表
    vector<CODE_BLOCK*> NextBlock;//下一个程序块，构造程序流图需要使用
};

//DAG图的结点
struct DAGNode {
    int id;
    DAGNDOETYPE type;
    vector<OVar> Dval;  //定值，结点右边的值
    int Cval;           //常量值，结点右边的值

    int Btype;          //0表示是变量值，1表示是常量值
    OVar BDval;         //变量值，结点下面的值
    double BCval;       //常数值，结点下面的值
    string op;          //操作类型

    string FuncN;       //记录返回的函数名
};
//判断字符串是否为浮点数（仅判断格式，不考虑范围）
bool isFloat(const char* str);
double atof(string a);
string ftos(double f);

class myCompiler;
class Optimizer {
    friend myCompiler;
private:
    vector<Var> VarTable;//变量表
    vector<Func> FuncTable;//函数表
    vector<Code> MiddleCodeTable;//中间代码表
    vector<CODE_BLOCK> CodeBlock;//代码基本块
    vector<Code> OptimCodeTable;//优化后的中间代码表
    ofstream OptimOutput;//中间代码输出，便于查看
    //使用邻接表进行存储
public:

    //初始化优化器，使用变量表，函数表以及中间代码表即可进行优化
    Optimizer(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1);
    //添加变量
    void addOVar(CODE_BLOCK& CurBlock, string name, Pos pos);
    //划分基本块并构建程序流图
    void Divblocks();
    //出基本块还需要使用的变量列表,find_id为要寻找的CodeBlock表中的index，cur_id为dfs当前结点
    void FindOutVar(int find_id, int cur_id);
    //对每一个基本块构造DAG图
    void CreateDAG(CODE_BLOCK&);
    //寻找右侧结点是否被定义，返回-1表示没找到，返回寻找到的节点编号
    int LeftIsDefine(string node1);
    //寻找下侧结点是否被定义，返回-1表示没找到，返回寻找到的节点编号
    int BottomIsDefine(string node1);
    //寻找DAG图中唯一后缀为B的节点
    int FindSubB(string B);
    //寻找一个结点是否被定义，若未被定义则分配一个结点
    int AllocDAGNode(DAGNode*, string);
    //寻找DAG图中后缀为B，C的结点
    int FindSubBC(string B, string C, string op);
    //删除右边的变量列表中的变量
    void DeleteVar(string val1, int id);
    //使用活跃信息对DAG图做剪枝
    void CutDAGGraph(CODE_BLOCK& CodeBlock);
    //使用dfs为某一结点作为根结点的子树打上标记
    void CutDAGMakeVis(int id);
    //每做一个代码块清空DAG图
    void ClearDAG();
    //确定DAG操作类型,返回0,1,21,22分别代表0,1,2型
    int OpType(Code MiddleCode);
    //生成优化后的中间代码
    void GenOCode(CODE_BLOCK&);
    //寻找根节点
    void FindRoots();
    //使用递归方式生成优化后的节点
    void GenOCodedfs(CODE_BLOCK&, int cur);
    //修改跳转指令的目标地址
    void UpdateJumpTar();
};



#endif // OPTIMIZER_H
