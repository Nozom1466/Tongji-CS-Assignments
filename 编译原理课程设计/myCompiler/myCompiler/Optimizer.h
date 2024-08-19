#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#pragma execution_character_set("utf-8") 
#include "SynSemAnalysis.h"

//lvarΪ��ֵ��oplΪ���������oprΪ�Ҳ�����
enum IPOS { lvar, opl, opr };
// == > >= < <= != EQ,GT,GE,LT,LE,NEQ,J
enum OPERATOR { NEG, ADD, SUB, MUL, DIV, MOV, EQ, GT, GE, LT, LE, NEQ, J };
enum DAGNDOETYPE { DAGJ, DAGA };
const int maxn = 1005;

//������λ�������б�����λ��
struct Pos {
    int lno;//�к�
    IPOS ipos;//�����еľ���λ��
};

//�����Ż��ı����ṹ��
struct OVar {
    string name;//������
    vector<Pos> pos;//������е�λ�ã���С����
};

//������
struct CODE_BLOCK {
    int id;//�����±���ΪΨһ����
    int vis;//dfs��ʱ���Ƿ񱻱�����ÿ�ε���FindOutVar����ʱ��Ҫ��������
    int BeginIndex;//��ʼ�Ĵ������
    vector<Code> code;//�������е��м����
    vector<Code> Ocode;//�Ż�����м����
    vector<OVar> varlist;//�����б�
    vector<OVar> Outvarlist;//�������黹��Ҫʹ�õı����б�
    vector<CODE_BLOCK*> NextBlock;//��һ������飬���������ͼ��Ҫʹ��
};

//DAGͼ�Ľ��
struct DAGNode {
    int id;
    DAGNDOETYPE type;
    vector<OVar> Dval;  //��ֵ������ұߵ�ֵ
    int Cval;           //����ֵ������ұߵ�ֵ

    int Btype;          //0��ʾ�Ǳ���ֵ��1��ʾ�ǳ���ֵ
    OVar BDval;         //����ֵ����������ֵ
    double BCval;       //����ֵ����������ֵ
    string op;          //��������

    string FuncN;       //��¼���صĺ�����
};
//�ж��ַ����Ƿ�Ϊ�����������жϸ�ʽ�������Ƿ�Χ��
bool isFloat(const char* str);
double atof(string a);
string ftos(double f);

class myCompiler;
class Optimizer {
    friend myCompiler;
private:
    vector<Var> VarTable;//������
    vector<Func> FuncTable;//������
    vector<Code> MiddleCodeTable;//�м�����
    vector<CODE_BLOCK> CodeBlock;//���������
    vector<Code> OptimCodeTable;//�Ż�����м�����
    ofstream OptimOutput;//�м������������ڲ鿴
    //ʹ���ڽӱ���д洢
public:

    //��ʼ���Ż�����ʹ�ñ������������Լ��м������ɽ����Ż�
    Optimizer(vector<Var> VarTable1, vector<Func> FuncTable1, vector<Code> MiddleCodeTable1);
    //��ӱ���
    void addOVar(CODE_BLOCK& CurBlock, string name, Pos pos);
    //���ֻ����鲢����������ͼ
    void Divblocks();
    //�������黹��Ҫʹ�õı����б�,find_idΪҪѰ�ҵ�CodeBlock���е�index��cur_idΪdfs��ǰ���
    void FindOutVar(int find_id, int cur_id);
    //��ÿһ�������鹹��DAGͼ
    void CreateDAG(CODE_BLOCK&);
    //Ѱ���Ҳ����Ƿ񱻶��壬����-1��ʾû�ҵ�������Ѱ�ҵ��Ľڵ���
    int LeftIsDefine(string node1);
    //Ѱ���²����Ƿ񱻶��壬����-1��ʾû�ҵ�������Ѱ�ҵ��Ľڵ���
    int BottomIsDefine(string node1);
    //Ѱ��DAGͼ��Ψһ��׺ΪB�Ľڵ�
    int FindSubB(string B);
    //Ѱ��һ������Ƿ񱻶��壬��δ�����������һ�����
    int AllocDAGNode(DAGNode*, string);
    //Ѱ��DAGͼ�к�׺ΪB��C�Ľ��
    int FindSubBC(string B, string C, string op);
    //ɾ���ұߵı����б��еı���
    void DeleteVar(string val1, int id);
    //ʹ�û�Ծ��Ϣ��DAGͼ����֦
    void CutDAGGraph(CODE_BLOCK& CodeBlock);
    //ʹ��dfsΪĳһ�����Ϊ�������������ϱ��
    void CutDAGMakeVis(int id);
    //ÿ��һ����������DAGͼ
    void ClearDAG();
    //ȷ��DAG��������,����0,1,21,22�ֱ����0,1,2��
    int OpType(Code MiddleCode);
    //�����Ż�����м����
    void GenOCode(CODE_BLOCK&);
    //Ѱ�Ҹ��ڵ�
    void FindRoots();
    //ʹ�õݹ鷽ʽ�����Ż���Ľڵ�
    void GenOCodedfs(CODE_BLOCK&, int cur);
    //�޸���תָ���Ŀ���ַ
    void UpdateJumpTar();
};



#endif // OPTIMIZER_H
