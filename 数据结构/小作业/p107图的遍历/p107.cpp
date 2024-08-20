//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string.h>
using namespace std;
int n, m;
int vis[10010];
vector<int> linkt[10010];
vector<int> temp;
queue<int> q;
 
 
void dfs(int seq) 
{
    
    if (vis[seq]) return;
 
    vis[seq] = 1;
 
    temp.push_back(seq);
    
    for (int i = 0; i < linkt[seq].size(); i++)
    {     
        dfs(linkt[seq][i]);
        
    }
    
}
 
 
void bfs(int n)
{
    q.push(n);
    temp.push_back(n);
    while (!q.empty())
    {
        int temp1 = q.front();
        vis[temp1] = 1;
        q.pop();
        for (int i = 0; i < linkt[temp1].size(); i++)
        {
            if (!vis[linkt[temp1][i]])
            {
                temp.push_back(linkt[temp1][i]);
                
                vis[linkt[temp1][i]] = 1;
                q.push(linkt[temp1][i]);
            }
        }
    }
}
 
 
 
 
int main() 
{
    cin >> n >> m;
 
    int minl = 0;
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        linkt[x].push_back(y);
        linkt[y].push_back(x);
 
 
    }
 
 
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
 
            dfs(i);
            cout << "{";
            for (int j = 0; j < temp.size() - 1; j++)
            {
                cout << temp[j] << " ";
            }
            cout << temp[temp.size() - 1];
            cout << "}";
            temp.clear();
        }
    }
    cout << endl;
 
    memset(vis, 0, sizeof(vis));
 
    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            bfs(i);
            cout << "{";
            for (int j = 0; j < temp.size() - 1; j++)
            {
                cout << temp[j] << " ";
            }
            cout << temp[temp.size() - 1];
            cout << "}";
            temp.clear();
        }
    }
    //cout << endl;
 
    return 0;
}
 
/*
 
 
13 13
0 1
0 2
0 5
0 11
1 12
3 4
6 7
6 8
6 10
7 10
9 11
9 12
11 12
*/
¡Á
