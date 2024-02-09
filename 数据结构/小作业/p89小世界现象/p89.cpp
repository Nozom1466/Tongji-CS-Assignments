//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string.h>
#include <iomanip>
using namespace std;
int n, m;
vector<int> linkt[10010];
vector<int> temp;
 
 
 
 
int bfs(int n)
{
    queue<int> q;
 
    queue<int> tag;
    int vis[2050] = { 0 };
    q.push(n);
    
    int dph = 0;
    int cnt = 0;
    tag.push(1);
 
    while (!q.empty())
    {
 
        
        int temp1 = q.front();
        q.pop();
        vis[temp1] = 1;
 
        dph = tag.front();
        tag.pop();
 
        if (dph > 6) break;
 
 
        for (int i = 0; i < linkt[temp1].size(); i++)
        {
            if (!vis[linkt[temp1][i]])
            {
                cnt++;
                q.push(linkt[temp1][i]);
                vis[linkt[temp1][i]] = 1;
                tag.push(dph + 1);
            }
        }            
 
 
 
 
    }
 
    return cnt + 1;
}
 
 
 
 
int main() 
{
    cin >> n >> m;
 
    int minl = 0;
    for (int i = 1; i <= m; i++)
    {
        int x, y;
        cin >> x >> y;
        linkt[x].push_back(y);
        linkt[y].push_back(x);
    }
 
 
    for (int i = 1; i <= n; i++)
    {
        //cout << i << ": " << (float)(100.0 * bfs(i) / n)  << "%" << endl;
        printf("%d: %.2lf", i, (100.0 * bfs(i) / n));
        cout << "%" << endl;
        //while (!tag.empty()) tag.pop();
    }
 
 
 
 
 
 
    
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
