//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
using namespace std;
int n, m;
int ans[10000000];
 
 
 
int main()
{
    scanf("%d%d", &n, &m);
    ans[1] = 0;
    for (int i = 2;i <= n; i++)
    {
        ans[i] = (ans[i - 1] + m) % i;
    }
    
    
    printf("%d ", ans[n]);
    
    
    
} 
¡Á
