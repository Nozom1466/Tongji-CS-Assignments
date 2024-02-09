//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
using namespace std;
double income;
double ans;
 
int main()
{
    for (int i = 1; i <= 12; ++i)
    {
        scanf("%lf", &income);
        ans += income;
    }
 
    printf("гд%.2lf\n", 1.0 * ans / 12);
 
 
 
    return 0;
}
б┴
