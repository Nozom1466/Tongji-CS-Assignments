//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <algorithm>
using namespace std;
int n, m, tar;
int s[1010], sumer[1010];
 
 
 
 
int main()
{
    cin >> n >> m;
 
 
    for (int i = 1; i <= n; i++)
    {
        cin >> s[i];
    }
    sort(s + 1, s + n + 1);
 
    for (int i = 1; i <= n; i++)
    {
        sumer[i] = sumer[i - 1] + s[i];
    }
    
 
    for (int j = 1; j <= m; j++)
    {
        cin >> tar;
 
        if (tar > sumer[n])
        {
            cout << n << " ";
            continue;
        }
 
        for (int i = 0; i <= n; i++)
        {
            if (sumer[i] > tar)
            {
                cout << i - 1 << " ";
                break;
            }
            
        }
        
    }
}
¡Á
