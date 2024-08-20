//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
int p;
 
 
vector<int> mul(vector<int> A, vector<int> B)
{
    vector<int> C;
    for (int i = 1; i <= 500; i++)
    {
        C.push_back(0);
    }
 
    //int t = 0;
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < B.size(); j++)
        {
            if (i + j < 500)
            {
                C[i + j] += A[i] * B[j];
                if (i + j + 1 < 500)
                    C[i + j + 1] += (C[i + j] / 10);
                C[i + j] %= 10;
            }
        }
    }
 
    return C;
}
 
vector<int> mul1(vector<int> A, int b)
{
    vector<int> C;
 
    int t = 0;
    for (int i = 0; i < A.size() || t; i++)
    {
        if (i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t /= 10;
    }
 
    while (C.size() > 1 && C.back() == 0) C.pop_back();
 
    return C;
}
 
 
 
vector<int> add(vector<int> A, vector<int> B)
{
    vector<int> C;
 
    int t = 0;
 
    for (int i = 0; i < A.size() || i < B.size(); i++)
    {
        if (i < A.size()) t += A[i];
        if (i < B.size()) t += B[i];
        C.push_back(t % 10);
        t /= 10;
    }
 
    if (t) C.push_back(1);
 
    while (C.size() > 1 && C.back() == 0) C.pop_back();
    return C;
 
 
 
}
 
 
 
vector<int> fps(vector<int> A, int p)
{
    vector<int> ans;
    ans.push_back(1);
 
    while (p)
    {
        if (p & 1) ans = mul(ans, A);
        A = mul(A, A);
        p >>= 1;
    }
    return ans;
}
 
 
string base;
int n;
 
 
 
int main()
{
    cin >> n >> base;
    vector<int> bas;
 
 
 
    for (int i = base.size() - 1; i >= 0; i--) bas.push_back(base[i] - '0');
 
    vector<int> res;
    res.push_back(0);
    vector<int> num;
 
    
 
 
    for (int i = 1; i <= n; i++)
    {
        num.clear();
        num.push_back(i);
 
        /*auto C = mul(fps(bas, i), num);
        for (int i = 0; i < 10; i++)
        {
            cout << C[i] << " ";
        }
        C.clear();
        cout << endl;*/
 
        
        res = add(res, mul(fps(bas, i), num));
    }
 
    while (res.size() > 1 && res.back() == 0) res.pop_back();
 
 
    for (int i = res.size() - 1; i >= 0; i--)
    {
        cout << res[i];
    }
 
    return 0;
}
¡Á
