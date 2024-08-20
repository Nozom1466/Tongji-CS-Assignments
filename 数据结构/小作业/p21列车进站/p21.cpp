//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <stack>
using namespace std;
string stand, s;
 
 
 
int main()
{
    cin >> stand;
    while (cin >> s)
    {
        stack<char> st;
        int ptr = 0;
        /*i : stand , ptr : s*/
        for (int i = 0; i < s.size(); i++)
        {
 
            if (stand[i] == s[ptr])
            {
                ptr++;
                continue;
            }
 
            else
            {
                if (!st.empty())
                {
                    if (s[ptr] == st.top())
                    {
                        st.pop();
                        ptr++;
                        i--;
 
                    }
 
                    else
                    {
                        st.push(stand[i]);
                    }
                }
 
                else
                {
                    st.push(stand[i]);
                }
            }
 
        }
        if (ptr >= s.size()) ptr--;
 
        int flg = 1;
        while (!st.empty())
        {
            if (st.top() != s[ptr++])
            {
                flg = 0;
            }
            st.pop();
 
        }
        if (!flg) cout << "no" << endl;
        else cout << "yes" << endl;
 
 
 
    }
 
 
 
    return 0;
}
¡Á
