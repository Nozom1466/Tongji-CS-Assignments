//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
#define MAXPLACE 4005
 
 
 
int main()
{
    int n;    
    cin >> n;
    
    for (int i = 1; i <= n; i++)
    {
        int ans[MAXPLACE] = {0};
        int oria[2005];
        int orib[2005];
        char s1[2005], s2[2005];
        
        
        cin >> s1 >> s2;
 
        if (!(s1[0] - '0') || !(s2[0] - '0'))
        {
            cout << 0;
            return 0;
        }
    
        
        int len_a = strlen(s1);
        int len_b = strlen(s2);
    
        for (int i = 0; i < len_a; i++)
        {
            oria[len_a - 1 - i] = s1[i] - '0';
        }
    
        for (int i = 0; i < len_b; i++)
        {
            orib[len_b - 1 - i] = s2[i] - '0';
        }
    
        
        for (int i = 0; i < len_a; i++)
        {
            for (int j = 0; j < len_b; j++)
            {
                ans[i + j] += oria[i] * orib[j];
                ans[i + j + 1] += ans[i + j] / 10;
                ans[i + j] %= 10;
            }
        }
        
    
        int place = len_a + len_b;
        if (!ans[len_a + len_b - 1]) place -= 1;
    
    
        for (int i = place - 1; i >= 0; i--)
        {
            cout << ans[i];
        }
        cout << endl;
 
    }
    
}
¡Á
