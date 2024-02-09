//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <cstring>
#include <string.h>
using namespace std;
vector<int> anss; 
 
 
/*32*/
int main()
{
    while (1)
    {
        char s[300];
        int ans = 0;
        
        cin.getline(s, 300);
        if (s[0] == '#') break;
        
        for (int i = 0; i < strlen(s); i++)
        {
            if (s[i] == ' ') continue;
            
            if (s[i] >= 'A' && s[i] <= 'Z') ans += ((i + 1) * (s[i] - 'A' + 1));
        
        }
        anss.push_back(ans);
        
    }
    
    for (int i = 0; i < anss.size(); i++)
    {
        cout << anss[i] << endl;
    }
        
    
    
    return 0;
}
¡Á
