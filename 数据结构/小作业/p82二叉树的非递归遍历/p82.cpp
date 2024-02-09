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
 
stack<Node*> st;  //记录左子树还没有遍历完的节点
 
//左边加节点加不了了(pop了)，就回到上一个可以加右节点的地方加(push)
 
void buildTree(int n, Node* root)
{
    
 
    Node* pointer = root;
 
    bool leftAva = true;
 
    while (n)
    {
 
        cin >> opt;
        if (opt == "pop")
        {
            if (leftAva) leftAva = false;  //仅当叶子节点会出现左边没有遍历完就要pop的情况
 
            else  
            {
                if (st.empty()) continue;
 
                pointer = st.top();  //回退到上一个左子树还没有遍历完的那个节点
                st.pop();
            }
        }
 
        else
        {
            cin >> dt;
            st.push(pointer);
            //首先插左边，之后看右边能不能插
            if (leftAva)
            {
                pointer->lc = new Node(dt);
                pointer = pointer->lc;
            }
 
            else
            {
                pointer->rc = new Node(dt);
                pointer = pointer->rc;
 
                st.pop();  //能插右边节点，说明这个节点已经满了，这个点之前还被push进栈了，所以这个时候要pop出去
            }
 
            leftAva = true;  //右子树可以插左孩子
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
 
    cin >> opt >> dt;  //初始建立根节点
 
    Node* root = new Node(dt);
 
    buildTree(n - 1, root);
 
    backTranverse(root);
 
 
    return 0;
}
×
