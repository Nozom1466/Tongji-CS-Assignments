//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <stack>
#include <string>
using namespace std;
 
class Node
{
public:
    Node(const char ch, Node* le=NULL, Node* ri=NULL)
    {
        c = ch;
        l = le;
        r = ri;
    }
 
    Node* l;
    Node* r;
    char c;
 
};
 
Node* root = NULL;
 
 
void buildTree(string tree)
{
    stack<Node*> s;
    bool left = false;
    Node* ptr;
 
    for (int i = 0; i < tree.size(); i++)
    {
        if (tree[i] == '\r') break;
 
        if (root == NULL)
        {
            root = new Node(tree[i]);
            ptr = root;
            left = true;
        }
        else if (tree[i] == '#')
        {
            if (left) left = false;
            else
            {
                if (s.empty()) break;
                ptr = s.top();
                s.pop();
            }
        }
        else
        {
            s.push(ptr);
 
            //首先插左边，之后看右边能不能插
            if (left)
            {
                ptr->l = new Node(tree[i]);
                ptr = ptr->l;
            }
 
            else
            {
                ptr->r = new Node(tree[i]);
                ptr = ptr->r;
 
                s.pop();  //能插右边节点，说明这个节点已经满了，这个点之前还被push进栈了，所以这个时候要pop出去
                left = true;  //右子树可以插左孩子
            }
 
            
        }
    }
}
 
 
void printNode(Node* node, const char ch, char& last_char, bool& last_ltag, bool& print_now, bool& find, string& message)
{
    if (node->l) printNode(node->l, ch, last_char, last_ltag, print_now, find, message);
    if (print_now) 
    {
        cout << "succ is " << node->c << (node->r == NULL) << endl;
        cout << message << endl;
        print_now = false;
    }
    if (node->c == ch) 
    {
        find = true;
        print_now = true;
 
        if (last_char == '\0') 
        {
            message = "prev is NULL";
        }
        else 
        {
            message = "prev is " + string(1, last_char) + to_string(last_ltag);
        }
    }
    last_char = node->c;
    last_ltag = node->l == NULL;
    if (node->r) printNode(node->r, ch, last_char, last_ltag, print_now, find, message);
}
 
 
void finder(char c)
{
    char last = '\0';
    string message;
    bool print_now = false, find = false, last_ltag = false;
    printNode(root, c, last, last_ltag, print_now, find, message);
    if (print_now)
    {
        cout << "succ is NULL" << endl;
        cout << message << endl;
    }
    if (find == false)
        cout << "Not found" << endl;
 
}
 
 
void inTranverse(Node* root1)
{
    
    if (root1->l) inTranverse(root1->l);
    cout << root1->c;
    if (root1->r) inTranverse(root1->r);
    
    
    
}
 
 
 
int main()
{
    string t;
    cin >> t;
 
    buildTree(t);
    inTranverse(root);
    cout << endl;
    char c;
    cin >> c;
    finder(c);
 
    return 0;
}
×
