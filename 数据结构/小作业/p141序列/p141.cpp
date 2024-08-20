//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
 
const int N = 2010;
 
int n, m, t;
 
int a[N], b[N], c[N];
 
void merge()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (int i = 0; i < n; i++)
    {
        heap.push({a[0] + b[i], 0});
    }
 
    for (int i = 0; i < n; i++)
    {
        pair<int, int> temp = heap.top();
        heap.pop();
        c[i] = temp.first;
        heap.push({ temp.first - a[temp.second] + a[temp.second + 1], temp.second + 1 });
    }
    for (int i = 0; i < n; i++)
    {
        a[i] = c[i];
    }
}
 
 
int main()
{
    
    cin >> t;
    while (t--)
    {
        cin >> m >> n;
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &a[i]);
        }
 
        sort(a, a + n);
        for (int i = 0; i < m - 1; i++)
        {
            for (int i = 0; i < n; i++)
            {
                scanf("%d", &b[i]);
            }
            merge();
        }
        for (int i = 0; i < n; i++)
        {
            cout << a[i] << " ";
        }
        cout << endl;
    }
 
 
    return 0;
}
¡Á
