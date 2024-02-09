//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
#include <map>
#include <cstring>
#include <string>
using namespace std;
//char s[N];
string s;
map<char, int> mp;
 
 
void doMath(char op, stack<int>& stshu)
{
    if (op == '!')
    {
        int temp = !stshu.top();
        stshu.pop();
        stshu.push(temp);
    }
    else if (op == '&')
    {
        int a = stshu.top();
        stshu.pop();
        int b = stshu.top();
        stshu.pop();
        stshu.push(a & b);
    }
    else if (op == '|')
    {
        int a = stshu.top();
        stshu.pop();
        int b = stshu.top();
        stshu.pop();
        stshu.push(a | b);
 
    }
}
 
/*V&V|V|V&F&V&V*/
 
 
int main()
{
    mp.insert({ '(', 0 });
    mp.insert({ '|', 1 });
    mp.insert({ '&', 2 });
    mp.insert({ '!', 3 });
    mp.insert({ ')', 0 });
 
    int cnt = 0;
    while (getline(cin,s))
    {
        //if (!s.size()) continue;
        stack<int> stshu;
        stack<char> stfuhao;
 
        char buhuiba;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ' ' || s[i] != 'V' && s[i] != 'F' && s[i] != '&' && s[i] != '|' && s[i] != '(' && s[i] != '!' && s[i] != ')') continue;
            /*数字入栈*/
            if (s[i] == 'V')
            {
                stshu.push(1);
                
            }
            else if (s[i] == 'F')
            {
                stshu.push(0);
            }
 
            /*符号入栈*/
            else
            {
                /*左括号直接进栈*/
                if (s[i] == '(')
                {
                    stfuhao.push('(');
                    continue;
                }
                
                /*栈为空，直接进*/
                else if (stfuhao.empty())
                {
                    stfuhao.push(s[i]);
                    continue;
                }
 
                else
                {
                    /*如果是右括号，直到遇到左括号之前，一直做运算，最后把左括号删掉*/
                    if (s[i] == ')')
                    {
                        while (stfuhao.top() != '(')
                        {
                            char tmp = stfuhao.top();
                            stfuhao.pop();
                            doMath(tmp, stshu);
                        }
 
                        stfuhao.pop();
                        continue;
                    }
 
 
                    else
                    {
                        while (!stfuhao.empty() && mp[s[i]] < mp[stfuhao.top()])  //短路运算
                        {
                            char fu = stfuhao.top();
                            doMath(fu, stshu);
                            stfuhao.pop();
                        }
                        stfuhao.push(s[i]);
 
                        continue;
                        
                    }
                    
                }
 
 
            }
 
 
 
 
        }
 
        while (!stfuhao.empty())
        {
            char opr = stfuhao.top();
            stfuhao.pop();
            doMath(opr, stshu);
        }
 
 
 
        if (stshu.top() == 1) cout << "Expression " << ++cnt << ": " << 'V' << endl;
        else cout << "Expression " << ++cnt << ": " << 'F' << endl;
 
 
 
 
    }
 
 
 
    return 0;
}
×
