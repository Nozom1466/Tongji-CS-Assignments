//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;
string s;
int bdp = -1;
 
void dfs(int& i, int fatherdepth)
{
    bdp = max(bdp, fatherdepth);
    int cnt = 0;
 
    while (s[i])
    {
        if (s[i] == 'd')
        {
            dfs(++i, fatherdepth + cnt + 1);  
            cnt++;
        }
        else
        {
            ++i;
            return;
        }
    }
}
 
int main()
{
    int cnt = 0;
    while (1)
    {
        int h = 0;
        cin >> s;
 
        if (s == "#") break;
 
 
        //求原来树的深度
        stack<char> st;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == 'd')
            {
                st.push(s[i]);
            }
 
            else
            {
                
                int t = st.size();
                h = max(h, t);
                st.pop();
                
            }
        }
 
        
        //求转换后的树的深度
        int ptr = 0;
        dfs(ptr, 0);
        cout << "Tree " << ++cnt << ": " << h << " => " << bdp << endl;
 
        bdp = -1;
 
    }
    
 
 
    return 0;
}
 
/*
 
 
dudduduudu
 
ddddduuuuu
 
dddduduuuu
 
dddduuduuu
 
#*/
×
