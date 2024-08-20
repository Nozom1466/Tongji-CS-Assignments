//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;
 
vector<pair<string, string>> info;
 
string str, num;
string oper;
int n, pos;
int ota, studentcnt;
 
int main()
{
    info.reserve(100010);
    
    cin >> n;
 
    
    for (int i = 0; i < n; ++i)
    {
        cin >> num >> str;
        info.emplace_back(make_pair(num, str));
        ota++;
        studentcnt++;
    }
    while (true)
    {
    BEGIN:
        studentcnt += ota;
        cin >> str;
        if (str == "end") 
        {
            cout << info.size() << endl;
            return 0;
        }
 
 
 
        else if (str == "remove")
        {
            cin >> pos;
            if (pos > info.size() || pos <= 0)
            {
                cout << -1 << endl;
                continue;
            }
            else 
            {
                cout << 0 << endl;
                info.erase(info.begin() + pos - 1);
            }
            ota--;
        }
  
 
        else if (str == "insert")
        {
            cin >> pos;
            if (pos > info.size() + 1 || pos <= 0)
            {
                cout << -1 << endl;
                continue;
            }
            else
            {
                cout << 0 << endl;
                cin >> num >> str;
                info.insert(info.begin() + pos - 1, make_pair(num, str));
            }
        }
 
        if (studentcnt == 0)
        {
            ota--;
            studentcnt++;
        }
 
        else if (str == "check") 
        {
 
            cin >> str;
            if (str == "no") 
            {
                cin >> num;
                for (auto it = info.begin(); it != info.end(); ++it)
                {
                    if ((*it).first == num) 
                    {
                        cout << it - info.begin() + 1 << " " << num.c_str() << " ";
                        cout << (*it).second.c_str() << endl;
 
                
                        goto BEGIN;
                    }
                }
                cout << -1 << endl;
            }
 
 
 
            else 
            {
                cin >> str;
                for (auto it = info.begin(); it != info.end(); ++it)
                {
                    if ((*it).second == str) 
                    {
                        cout << it - info.begin() + 1 << " " << (*it).first.c_str() << " ";
                        cout << str.c_str() << endl;
 
                        goto BEGIN;
                    }
                }
                cout << -1 << endl;
            }
        }
    }
    return 0;
}
¡Á
