//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <stdio.h>
#include <algorithm>
using namespace std;
 
 
 
 
class Node
{
public:
    //construction
    Node(int ele, Node* parent)
    {
        this->ele = ele;
        this->parent = parent;
        this->left = NULL;
        this->right = NULL;
        this->cnt = 1;
        this->height = 1;
    }
 
    //balance factor
    int finderBalanceFactor()
    {
        //the height of right subtree minus the height of left subtree
        return (this->right ? this->right->height : 0) - (this->left ? this->left->height : 0);
    }
 
    //update the height
    void updateHeight()
    {
        //dont forget to add up the height of itself
        this->height = max((this->left ? this->left->height : 0), (this->right ? this->right->height : 0)) + 1;
    }
 
    bool finder(int ele, Node*& pre, Node*& thiss, Node*& next, bool& found)
    {
        if (this->left)
            if (this->left->finder(ele, pre, thiss, next, found))
                return true;
        if (found) { // �Ѿ��ҵ����ýڵ㼴Ϊ��̽ڵ㣬���нڵ㶼�ҵ��������˳�
            next = this;
            return true;
        }
        if (this->ele > ele) { // û�ж�Ӧ�ڵ㣬�ҵ�ǰ���ͺ�̣������˳�
            thiss = NULL;
            next = this;
            return true;
        }
        if (this->ele == ele) { // �ҵ���Ӧ�ڵ㣬��Ҫ�����������Һ��
            thiss = this;
            found = true;
        }
        else
            pre = this; // û���ҵ���Ӧ�ڵ㣬��ǰ�ڵ�ֵС�ڲ���Ԫ�أ���ʱ��Ϊǰ��
        if (this->right)
            return this->right->finder(ele, pre, thiss, next, found);
        return false; // ���һ�δ����
    }
    Node* left, * right, * parent;
    int ele, cnt, height;
};
 
void keepBalance(Node* p);
void ChangeParent(Node* p, Node* t);
void RotateRight(Node* p);
void RotateLeft(Node* p);
void UpdateRelation(Node* p, Node* t);
void ChangeParent(Node* p, Node* t);
 
 
//the root of AVL tree
Node* root = NULL;
 
 
void insert(int ele)
{
    if (!root)
    {
        root = new Node(ele, NULL);
        return;
    }
 
    Node* l = NULL, * p = NULL, * n = NULL;
    bool found = false;
    root->finder(ele, l, p, n, found);
 
    if (p)
    {
        p->cnt++;
        return;
    }
 
    if (l && !l->right)
    {
        p = (l->right = new Node(ele, l));
    }
    else
    {
        p = (n->left = new Node(ele, n));
    }
    keepBalance(p);
 
}
 
bool deleteItem(int ele)
{
    if (!root)
        return false;
    Node* l = NULL, * p = NULL, * n = NULL;
    bool found = false;
    root->finder(ele, l, p, n, found);
    if (!p) // û�ҵ�
        return false;
    if (p->cnt > 1) { // ��ɾ��Ԫ�ز�ֹһ����������һ����
        p->cnt--;
        return true;
    }
    // ��ɾ��Ԫ��ֻ��һ������Ҫɾ���ڵ�
    if (!p->left && !p->right) { // ��ɾ���ڵ�ΪҶ�ӽڵ�
        if (!p->parent) {
            root = NULL;
        }
        else {
            ChangeParent(p, NULL);
            keepBalance(p->parent);
        }
    }
    else if (!p->left) { // û�����������޷���ǰ�������Լ����������������Լ�
        // ���û����������Ҳ����ֱ���������������Լ�
        ChangeParent(p, p->right);
        keepBalance(p->parent);
    }
    else { // ������������ǰ��һ�����������У���ǰ�������Լ�
        if (p->left == l) { // ǰ����������
            l->right = p->right;
            if (p->right)
                p->right->parent = l;
            ChangeParent(p, l);
            keepBalance(l);
        }
        else { // �ҵ�ǰ������ǰ�������Լ����������ӹ�ϵ
            Node* t = l->parent;
            t->right = l->left;
            if (l->left)
                l->left->parent = t;
 
            l->left = p->left;
            p->left->parent = l;
            l->right = p->right;
            if (p->right)
                p->right->parent = l;
            ChangeParent(p, l);
            keepBalance(t);
        }
    }
    p->left = p->right = NULL;
    delete p;
    return 1;
}
 
int counter(int ele)
{
    if (!root)
        return 0;
    Node* l = NULL, * p = NULL, * n = NULL;
    bool found = false;
    root->finder(ele, l, p, n, found);
    if (p)
        return p->cnt;
    return 0;
}
 
int finderMin()
{
    if (!root)
        return -1;
    Node* p = root;
    while (p->left)
        p = p->left;
    return p->ele;
}
 
int finderParent(int ele)
{
    if (!root)
        return -1;
    Node* l = NULL, * p = NULL, * n = NULL;
    bool found = false;
 
    root->finder(ele, l, p, n, found);
    if (l)
        return l->ele;
    return -1;
}
 
void keepBalance(Node* p)
{
    while (p) {
        p->updateHeight();
        if (-2 >= p->finderBalanceFactor() || p->finderBalanceFactor() >= 2) {
            // ��ת
            if (p->finderBalanceFactor() < 0) {
                if (p->left->finderBalanceFactor() <= 0) {
                    RotateRight(p);
                }
                else {
                    RotateLeft(p->left);
                    RotateRight(p);
                }
            }
            else {
                if (p->right->finderBalanceFactor() >= 0) {
                    RotateLeft(p);
                }
                else {
                    RotateRight(p->right);
                    RotateLeft(p);
 
                }
            }
        }
        p = p->parent;
    }
}
 
/*
��ת
*/
void RotateRight(Node* p) {
    Node* t = p->left;
    p->left = t->right;
    if (t->right)
        t->right->parent = p;
    t->right = p;
    UpdateRelation(p, t);
}
/*
��ת
 */
void RotateLeft(Node* p) {
    Node* t = p->right;
    p->right = t->left;
    if (t->left)
        t->left->parent = p;
    t->left = p;
    UpdateRelation(p, t);
}
/*
������ת��ڵ��ϵ
 */
void UpdateRelation(Node* p, Node* t) {
    ChangeParent(p, t);
    p->parent = t;
    p->updateHeight();
    t->updateHeight();
}
 
void ChangeParent(Node* p, Node* t) {
    if (t)
        t->parent = p->parent;
    if (p->parent) {
        if (p == p->parent->left)
            p->parent->left = t;
        else
            p->parent->right = t;
    }
    else
        root = t;
}
 
int main()
{
    int n;
    cin >> n;
    while (n)
    {
        int option = 0, ele = 0, last = 0;
        cin >> option;
        if (option != 4)
            cin >> ele;
 
        if (option == 1)
            insert(ele);
        else if (option == 2)
        {
            if (!deleteItem(ele))
                cout << "None" << endl;
        }
        else if (option == 3)
            cout << counter(ele) << endl;
        else if (option == 4)
            cout << finderMin() << endl;
        else if (option == 5)
        {
            last = finderParent(ele);
            if (last == -1)
                cout << "None" << endl;
            else
                cout << last << endl;
        }
 
 
        n--;
    }
 
 
    return 0;
}
��
