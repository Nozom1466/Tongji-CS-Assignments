//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <map>
#include <stack>
#include <queue>
using namespace std;
string s;
int vbnum;
map<char, int> mp;
map<char, int>symblab;
 
const int N = 100;
 
int root;
int r_x, r_y;
 
char mat[500][500];
 
int printLen[500];
 
class Node
{
public:
    //Node();
    Node(int sub=-1, char cq=' ', int le = -1, int ri = -1, int val = 0)
    {
        this->subid = sub;
        this->c = cq;
        this->l = le;
        this->r = ri;
        this->val = val;
    }
    int subid;
    char c;
    int l;
    int r;
    int val;
 
};
 
int doMath(stack<Node>& num, char sym, Node& fa)
{
    if (sym == '+')
    {
        int num1 = num.top().val;
        fa.r = num.top().subid;
        num.pop();
        int num2 = num.top().val;
        fa.l = num.top().subid;
        num.pop();
        return num1 + num2;
    }
    else if (sym == '-')
    {
        int num1 = num.top().val;
        fa.r = num.top().subid;
        num.pop();
        int num2 = num.top().val;
        fa.l = num.top().subid;
        num.pop();
        return num2 - num1;
    }
    else if (sym == '*')
    {
        int num1 = num.top().val;
        fa.r = num.top().subid;
        num.pop();
        int num2 = num.top().val;
        fa.l = num.top().subid;
        num.pop();
        return num1 * num2;
    }
    else if (sym == '/')
    {
        int num1 = num.top().val;
        fa.r = num.top().subid;
        num.pop();
        int num2 = num.top().val;
        fa.l = num.top().subid;
        num.pop();
        return num2 / num1;
    }
    else
        return -1;
}
 
Node tree[N];
stack<Node> numst;
stack<Node> symst;
 
//返回值为表达式的值
int buildTree(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        Node node = Node(i, s[i]);
        node.c = s[i];
        node.subid = i;
        tree[i] = node;
 
        //是数
        if (s[i] >= 'a' && s[i] <= 'z')
        {
 
            node.val = mp[s[i]];
            numst.push(node);
        }
 
        //是符号
        else
        {
 
            if (symst.empty() || s[i] == '(')
            {
                symst.push(node);
            }
 
            else if (symblab[symst.top().c] < symblab[s[i]])
            {
                symst.push(node);
            }
            else
            {
                if (s[i] == ')')
                {
                    while (symst.top().c != '(')
                    {
                        Node nd = symst.top();
                        symst.pop();
                        nd.val = doMath(numst, nd.c, nd);
                        tree[nd.subid] = nd;
                        numst.push(nd);
                    }
                    symst.pop();
                }
 
                else
                {
                    while (!symst.empty() && (symblab[symst.top().c] >= symblab[s[i]]))
                    {
                        Node nd = symst.top();
                        symst.pop();
                        nd.val = doMath(numst, nd.c, nd);
                        tree[nd.subid] = nd;
                        numst.push(nd);
                    }
                    symst.push(node);
                }
            }
        }
    }
 
    while (!symst.empty())
    {
        Node nd = symst.top();
        symst.pop();
        nd.val = doMath(numst, nd.c, nd);
        tree[nd.subid] = nd;
        numst.push(nd);
    }
 
    root = numst.top().subid;
 
    return numst.top().val;
 
}
 
void backTranverse(int pos, int x, int y)
{
    if (tree[pos].l != -1)
    {
        backTranverse(tree[pos].l, x - 1, y + 2);
    }
    if (tree[pos].r != -1)
    {
        backTranverse(tree[pos].r, x + 1, y + 2);
    }
    
 
    cout << tree[pos].c;
}
 
int depth(int root1)
{
    int left = 0, right = 0;
    if (tree[root1].l != -1) left = depth(tree[root1].l);
    if (tree[root1].r != -1) right = depth(tree[root1].r);
    return ((left >= right) ? left : right) + 1;
}
 
int fps(int a, int p)
{
    int ans = 1;
    while (p)
    {
        if (p & 1) ans *= a;;
 
        a *= a;
        p >>= 1;
    }
    return ans;
}
 
 
void bfs(int h)
{
    r_x = 0;
    r_y = (fps(2, h) - 1) / 2;
 
    queue<Node> q;
    queue<pair<int, int>> cod;
    q.push(tree[root]);
    cod.push({ r_x, r_y });
 
    while (!q.empty())
    {
        Node nd = q.front();
        q.pop();
        pair<int, int> pr = cod.front();
        cod.pop();
 
        int t_x = pr.first;
        int t_y = pr.second;
        if (t_x != 0 && mat[t_x - 1][t_y] == '/')
        {
            t_y -= (fps(2, h - t_x / 2 - 1) - 1);
        }
        else if (t_x != 0 && mat[t_x - 1][t_y] == '\\')
        {
            t_y += (fps(2, h - t_x / 2 - 1) - 1);
        }
        
        
 
        mat[t_x][t_y] = nd.c;
 
        if (nd.l != -1)
        {
            mat[t_x + 1][t_y - 1] = '/';
            q.push(tree[nd.l]);
            cod.push({t_x + 2, t_y - 1});
        }
 
        if (nd.r != -1)
        {
            mat[t_x + 1][t_y + 1] = '\\';
            q.push(tree[nd.r]);
            cod.push({ t_x + 2, t_y + 1 });
 
        }
        else
        {
            mat[t_x + 1][t_y + 1] = ' ';
            mat[t_x + 1][t_y - 1] = ' ';
        }
    }
}
 
 
 
int main()
{
    symblab.insert({ '+', 1 });
    symblab.insert({ '-', 1 });
    symblab.insert({ '*', 2 });
    symblab.insert({ '/', 2 });
    symblab.insert({ '(', 0 });
    symblab.insert({ ')', 0 });
 
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            mat[i][j] = 'N';
        }
    }
 
 
 
    cin >> s;
    cin >> vbnum;
 
    for (int i = 1; i <= vbnum; i++)
    {
        char vb;
        int n;
        cin >> vb >> n;
        mp.insert({ vb, n });
    }
 
    int res = buildTree(s);
 
    /*for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << " : " << "l: " << tree[i].l << " " << "r: " << tree[i].r << endl;
    }*/
 
    int ht = depth(root);
 
    //rootPos(ht);
 
    r_x = 0;
    r_y = (fps(2, ht) - 1) / 2;
 
    backTranverse(root, r_x, r_y);
 
    bfs(ht);
 
    cout << endl;
 
    for (int i = 0; i < ht * 2 - 1; i++)
    {
        int j = fps(2, ht - 1) * 2 - 1;
        while (mat[i][j] == 'N')
        {
            j--;    
        }
 
        printLen[i] = j;
 
        while (j >= 0)
        {
            if (mat[i][j] == 'N') mat[i][j] = ' ';
            j--;
        }
    }
    
 
 
 
    for (int i = 0; i < ht * 2 - 1; i++)
    {
        for (int j = 0; j <= printLen[i]; j++)
        {
            cout << mat[i][j];
        }
        cout << endl;
    }
 
    cout << endl << res << endl;
 
    return 0;
}
×
