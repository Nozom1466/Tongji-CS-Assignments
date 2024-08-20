//Paste your code here
//NOTE: If you use java, your Main class must be named solution
//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
 
/*第1行一个整数m，表示第一个一元多项式的长度
第2行有2m项，p1 e1 p2 e2 ...，中间以空格分割,表
示第1个多项式系数和指数
第3行一个整数n,表示第二个一元多项式的项数
第4行有2n项，p1 e1 p2 e2 ...，中间以空格分割,表
示第2个多项式系数和指数
第5行一个整数，若为0,执行加法运算并输出结果，
若为1，执行乘法运算并输出结果；
若为2，输出一行加法结果和一行乘法的结果。*/
 
int n, m;
int operation;
const int N = 3000;
vector<pair<int, int>> a1, a2, res_sum, res_mul;
 
 
bool cmp(pair<int, int> p1, pair<int, int> p2)
{
    return p1.second < p2.second;
}
 
 
vector<pair<int, int>> pluser(vector<pair<int, int>> a1, vector<pair<int, int>> a2)
{
    vector<pair<int, int>> res_sum;
    int ptr1 = 0, ptr2 = 0;
    while (ptr1 < a1.size() && ptr2 < a2.size())
    {
        if (a1[ptr1].second == a2[ptr2].second)
        {
            res_sum.push_back({ a1[ptr1].first + a2[ptr2].first, a1[ptr1].second });
            ptr1++, ptr2++;
        }
 
        else if (a1[ptr1].second < a2[ptr2].second)
        {
            res_sum.push_back(a1[ptr1++]);
        }
 
        else
        {
            res_sum.push_back(a2[ptr2++]);
        }
    }
 
    for (int i = ptr1; i < a1.size(); i++)
    {
        res_sum.push_back(a1[i]);
    }
 
 
    for (int i = ptr2; i < a2.size(); i++)
    {
        res_sum.push_back(a2[i]);
    }
 
    return res_sum;
}
 
 
vector<pair<int, int>> multi(vector<pair<int, int>> a1, vector<pair<int, int>> a2)
{
    vector<pair<int, int>> res_mul;
    for (int i = 0; i < a1.size(); i++)
    {
        vector<pair<int, int>> res;
        for (int j = 0; j < a2.size(); j++)
        {
            res.push_back({a1[i].first * a2[j].first, a1[i].second + a2[j].second});
            
        }
        res_mul = pluser(res_mul, res);
    }
    return res_mul;
}
 
 
int main()
{
    
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        int cof, ep;
        scanf("%d%d", &cof, &ep);
        a1.push_back(make_pair(cof, ep));
    }
 
 
    scanf("%d", &m);
    for (int i = 1; i <= m; i++)
    {
        int cof, ep;
        scanf("%d%d", &cof, &ep);
        a2.push_back(make_pair(cof, ep));
    }
 
 
    sort(a1.begin(), a1.end(), cmp);
    sort(a2.begin(), a2.end(), cmp);
 
    vector<pair<int, int>> res_sum = pluser(a1, a2);
 
    vector<pair<int, int>> res_mul = multi(a1, a2);
 
    sort(res_mul.begin(), res_mul.end(), cmp);
 
    cin >> operation;
 
   
    
    if (operation == 0)
    {
        if (res_sum[0].first == 0 && res_sum[0].second == 0) return 0;
        for (int i = 0; i < res_sum.size(); i++)
        {
            if (res_sum[i].first)
                cout << res_sum[i].first << " " << res_sum[i].second << " ";
        }
    }
 
    if (operation == 1)
    {
        if (res_mul[0].first == 0 && res_mul[0].second == 0) return 0;
        for (int i = 0; i < res_mul.size(); i++)
        {
            cout << res_mul[i].first << " " << res_mul[i].second << " ";
        }
    }
    
    if (operation == 2)
    {
        
        for (int i = 0; i < res_sum.size(); i++)
        {
            if (res_sum[0].first == 0 && res_sum[0].second == 0) break;
            if (res_sum[i].first)
                cout << res_sum[i].first << " " << res_sum[i].second << " ";
        }
        
        cout << endl;
        
        //if (res_mul[0].first == 0 && res_mul[0].second == 0) break;
        for (int i = 0; i < res_mul.size(); i++)
        {
            if (res_mul[0].first == 0 && res_mul[0].second == 0) return 0;
            cout << res_mul[i].first << " " << res_mul[i].second << " ";
        }
    }
    
    return 0;
    
}
×
