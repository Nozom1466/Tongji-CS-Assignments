//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <queue>
using namespace std;
int n, m;
queue<pair<int, int>> q;
const int N = 1000;
int vis[N][N];
int mp[N][N];
int xd[] = { 0, 0, -1, 1 };
int yd[] = { 1, -1, 0, 0 };
int flg;
 
bool check(int x, int y)
{
    if (x == n || x == 1 || y == m || y == 1) return 0;
    else return 1;
}
 
int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> mp[i][j];
        }
    }
 
    int cnt = 0;
 
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (mp[i][j] && !vis[i][j])
            {
                q.push({ i, j });
                vis[i][j] = 1;
                flg = 1;
                if (check(i,j)) flg = 0;
                
                while (q.size())
                {
                    pair<int, int> cod = q.front();
                    q.pop();
                    for (int i = 0; i < 4; i++)
                    {
                        int nx = cod.first + xd[i];
                        int ny = cod.second + yd[i];
                        if (nx <= n && nx >= 1 && ny <= m && ny >= 1 && mp[nx][ny] && !vis[nx][ny])
                        {
                            vis[nx][ny] = 1;
                            q.push({ nx, ny });
                            if (check(nx, ny)) flg = 0;
                        }
                    }
 
                }
 
                if (!flg)
                {
                    cnt++;
                }
 
            }
            
            
        }
    }
    cout << cnt;
 
 
 
    return 0;
}
¡Á
