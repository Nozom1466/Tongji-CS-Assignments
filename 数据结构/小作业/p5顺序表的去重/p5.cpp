//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
 
#include <iostream>
#include <vector>
using namespace std;
int n;
const int N = 1000010;
long long a;
vector<long long> vis;
 
 
int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        scanf("%lld", &a);
        int flg = 0;
        for (int i = 0; i < vis.size(); i++)
        {
            if (vis[i] == a) flg = 1;
        }
        
        if (!flg) 
        {
            vis.push_back(a);
        }
        
    }
    
    for (int i = 0; i < vis.size(); i++)
    {
        printf("%lld ", vis[i]);
    }
 
    return 0;
}
¡Á
