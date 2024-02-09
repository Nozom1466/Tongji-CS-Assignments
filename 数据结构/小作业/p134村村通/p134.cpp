//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int n, m;
int mat[105][105];
int father[105];
 
int find(int n)
{
 
    if (father[n] != n) return father[n] = find(father[n]);
    else return father[n];
}
 
 
void unite (int n1, int n2)
{
    int pRoot = find(n1);
 
    int qRoot = find(n2);
 
    if (pRoot == qRoot)
    {
        return;
    }
 
    father[pRoot] = qRoot;
}
 
 
 
class Edge
{
public:
    Edge(int n1, int n2, int w)
    {
        this->node1 = n1;
        this->node2 = n2;
        this->weight = w;
        this->vis = 0;
    }
 
    int node1;
    int node2;
    int weight;
    int vis;
};
 
 
class mC
{
public:
    bool operator()(Edge v1, Edge v2)
    {
        return v1.weight < v2.weight;
    }
};
 
 
vector<Edge> edge;
 
 
int main()
{
    for (int i = 0; i < 105; i++)
    {
        father[i] = i;
    }
 
    cin >> n;
 
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> mat[i][j];
        }
    }
 
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j < i; j++)
        {
            Edge e = Edge(i, j, mat[i][j]);
            //e.vis = 1;
            edge.push_back(e);
        }
    }
     
    cin >> m;
 
    sort(edge.begin(), edge.end(), mC());
 
    /*for (int i = 0; i < edge.size(); i++)
    {
        cout << edge[i].weight << " ";
    }
    cout << endl;*/
 
 
 
    for (int i = 1; i <= m; i++)
    {
        int x, y;
 
        cin >> x >> y;
 
        for (int j = 0; j < edge.size(); j++)
        {
            if (edge[j].node1 == x && edge[j].node2 == y || edge[j].node1 == y && edge[j].node2 == x)
            {
                edge[j].vis = 1;
                break;
            }
        }
 
        unite(x, y);
    }
 
    int weight = 0;
    int cnt = 0;
    //cout << edge.size() << endl;
    for (int i = 0; i < edge.size() && cnt != n - 1; i++)
    {
        if (!edge[i].vis)
        {
            if (find(edge[i].node1) != find(edge[i].node2))
            {
                unite(edge[i].node1, edge[i].node2);
                weight += edge[i].weight;
                cnt++;
            }
        }
    }
 
    cout << weight;
    return 0;
}
¡Á
