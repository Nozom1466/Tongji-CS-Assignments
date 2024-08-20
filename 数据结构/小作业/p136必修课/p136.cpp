//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
 
const int maxn = 200;
vector<int> lastv[maxn], nextv[maxn];
int curtime[maxn], curmin[maxn], curmax[maxn];
int inDeg[maxn], outDeg[maxn];
 
 
int main()
{
    int n, m, temp, cnt = 0, edt = 0;
    cin >> n;
 
    for (int i = 1; i <= n; i++)
    {
        cin >> curtime[i] >> m;
        
        for (int j = 0; j < m; ++j)
        {
            cin >> temp;
            lastv[i].emplace_back(temp);
            nextv[temp].emplace_back(i);
        }
    }
 
    fill(curmax + 1, curmax + n + 1, 1 << 30);
 
    for (int i = 1; i <= n; ++i) 
    {
        inDeg[i] = lastv[i].size();
        outDeg[i] = nextv[i].size();
    }
    queue<int> q;
 
 
 
 
 
 
    for (int i = 1; i <= n; i++) 
    {
        if (lastv[i].empty())
        {
            q.push(i);
        }
    }
    while (cnt < n && !q.empty()) 
    {
        int now = q.front();
        q.pop();
        cnt++;
        edt = max(edt, curmin[now] + curtime[now]);
        for (auto next : nextv[now]) 
        {
            curmin[next] = max(curmin[next], curmin[now] + curtime[now]);
            if (--inDeg[next] == 0) q.push(next);
        }
    }
 
 
 
 
 
 
 
 
 
 
 
 
 
    for (int i = 1; i <= n; i++) 
    {
        if (nextv[i].empty()) 
        {
            curmax[i] = edt - curtime[i];
            q.push(i);
        }
    }
    while (cnt && !q.empty()) 
    {
        int now = q.front();
        q.pop();
        cnt--;
        for (auto last : lastv[now]) 
        {
            curmax[last] = min(curmax[last], curmax[now] - curtime[last]);
            if (--outDeg[last] == 0) q.push(last);
        }
    }
 
    for (int i = 1; i <= n; i++)
    {
        
        cout << curmin[i] + curtime[i] << " " << (curmax[i] == curmin[i]) << endl;
    }
 
    return 0;
}
¡Á
