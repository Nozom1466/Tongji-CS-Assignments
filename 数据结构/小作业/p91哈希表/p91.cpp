//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
using namespace std;
int n, p, m;
 
 
const int N = 10015;
 
int primes[N], cnt;     // primes[]存储所有素数
bool st[N];         // st[x]存储x是否被筛掉
 
void get_primes(int n)
{
    for (int i = 2; i <= n; i++)
    {
        if (!st[i]) primes[cnt++] = i;
        for (int j = 0; primes[j] <= n / i; j++)
        {
            st[primes[j] * i] = true;
            if (i % primes[j] == 0) break;
        }
    }
}
 
int hashTable[N] = { 0 };
 
int vis[N] = { 0 };
 
int hasher(int mod, string s)
{
    int key = 0;
    for (int i = 1; i <= s.size(); i++)
    {
        key = (key * 37 + s[i - 1]) % mod;
        //cout << key << "**********************" << endl;
    }
 
    return key;
}
 
int finder(int oriKey, int mod)
{
    if (vis[oriKey] == 0)
        return oriKey;
    else
    {
        int found = 0;
        int base = 1;
        int times = 0;
        while (!found)
        {
            if (times > mod)
                break;
 
            if (vis[(oriKey + base * base) % mod] == 0)
            {
                found = 1;
                return (oriKey + base * base) % mod;
            }
            else if (vis[(oriKey - base * base) % mod] == 0)
            {
                found = 1;
                return (oriKey - base * base) % mod;
            }
            else
            {
                base += 1;
                times += 1;
            }
        }
 
        return -1;
    }
}
 
 
int Square(int n) {
    return (n * n);
}
 
 
int findIt(string& str) {
    int count = hasher(m, str);
    for (int i = 0; i <= m; i++) {
        int index = ((count + Square((i + 1) / 2 % m) * (i % 2 ? 1 : -1)) % m + m) % m;
        if (vis[index] == 0) {
            //list_[index] = str;
            return index;
        }
    }
    return -1;
}
 
 
 
int main()
{
    get_primes(10010);
    cin >> n >> p;
    for (int i = 0; i < 10000; i++)
    {
        if (primes[i] >= p)
        {
            m = primes[i];
            break;
        }
    }
 
 
    string temp;
    while (n)
    {
        cin >> temp;
        //int key = hasher(m, temp);
        //cout << key << endl;
        int res = findIt(temp);
        if (res == -1)
            cout << "- ";
        else
        {
            cout << res << " ";
            vis[res] = 1;
        }
        n--;
    }
 
 
 
 
    return 0;
}
×
