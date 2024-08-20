//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
using namespace std;
#include <vector>
vector<long long> M;
vector<long long> F;
int n;
long long ans1[10000];
long long ans2[10000];
 
int  main()
{
    for (int i = 1; i <= 10000; i++)
    {
        /*scanf("%d", &n);
        if (n < 0) break;*/
        n = i;
 
        M.push_back(0);
        M.push_back(1);
        F.push_back(0);
        F.push_back(0);
    
        for (int i = 2; i <= n; i++)
        {
            F[i] = M[i - 1];
            M[i] = M[i - 1] + F[i - 1] + 1;
        }
        
        ans1[i] = M[n];
        ans2[i] = 2 * M[n - 1] + F[n - 1] + 2;
        //printf("%lld %lld", M[n], 2 * M[n - 1] + F[n - 1] + 2);
    }
    
    
    while (1)
    {
        scanf("%d", &n);
        if (n < 0) break;
        printf("%lld %lld\n", ans1[n], ans2[n]); 
    }
    
    
    return 0;
}
 
¡Á
