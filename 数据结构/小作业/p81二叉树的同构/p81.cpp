//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <stack>
using namespace std;
string opt;
char dt;
 
 
class Node
{
public:
    Node(char d, Node* l=NULL, Node* r=NULL)
    {
        data = d;
        lc = l;
        rc = r;
    }
 
    char data;
    Node* lc;
    Node* rc;
};
 
stack<Node*> st;  //��¼��������û�б�����Ľڵ�
 
//��߼ӽڵ�Ӳ�����(pop��)���ͻص���һ�����Լ��ҽڵ�ĵط���(push)
 
void buildTree(int n, Node* root)
{
    
 
    Node* pointer = root;
 
    bool leftAva = true;
 
    while (n)
    {
 
        cin >> opt;
        if (opt == "pop")
        {
            if (leftAva) leftAva = false;  //����Ҷ�ӽڵ��������û�б������Ҫpop�����
 
            else  
            {
                if (st.empty()) continue;
 
                pointer = st.top();  //���˵���һ����������û�б�������Ǹ��ڵ�
                st.pop();
            }
        }
 
        else
        {
            cin >> dt;
            st.push(pointer);
            //���Ȳ���ߣ�֮���ұ��ܲ��ܲ�
            if (leftAva)
            {
                pointer->lc = new Node(dt);
                pointer = pointer->lc;
            }
 
            else
            {
                pointer->rc = new Node(dt);
                pointer = pointer->rc;
 
                st.pop();  //�ܲ��ұ߽ڵ㣬˵������ڵ��Ѿ����ˣ������֮ǰ����push��ջ�ˣ��������ʱ��Ҫpop��ȥ
            }
 
            leftAva = true;  //���������Բ�����
            n--;
 
        }
    }
 
 
}
 
 
void backTranverse(Node* root)
{
    if (root->lc) backTranverse(root->lc);
    if (root->rc) backTranverse(root->rc);
    cout << root->data;
}
 
int main()
{
    int n;
    cin >> n;
 
    cin >> opt >> dt;  //��ʼ�������ڵ�
 
    Node* root = new Node(dt);
 
    buildTree(n - 1, root);
 
    backTranverse(root);
 
 
    return 0;
}
��
