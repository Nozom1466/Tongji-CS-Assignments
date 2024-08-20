//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <algorithm>
#include <stack>
using namespace std;
const int N = 1e6 + 5;
int start;
 
 
 
//           ((())()(((())()())()(()(())(()()())(()()())()(()((()(())())(
 
 
int main()
{
    string s;
    cin >> s;
    
    int n = s.length();
    if (n == 1 || n == 0)
    {
        cout << 0 << " " << 0;
        return 0;
    }
    
    stack<int> stk;
    
    stk.push(-1);
    
    int ret = 0;
    for (int i = 0; s[i]; ++i) {
        if (s[i] == '(') 
        {
            stk.push(i);
        }
        else 
        {
            stk.pop();
            if (stk.empty()) stk.push(i);
            else 
            {
                if (i - stk.top() > ret)
                {
                    ret = i - stk.top();
                    start = stk.top() + 1;
                }
                
            }
        }
    }
    cout << ret << " " << start << endl;
    
 
    return 0;
}
¡Á
