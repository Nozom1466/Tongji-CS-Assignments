//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
int k, n, m;
vector<vector<int>> row;
int inDegreeR[1050] = {0};
int visR[1050] = { 0 };
 
vector<vector<int>> col;
int inDegreeC[1050] = { 0 };
int visC[1050] = { 0 };
 
int pos[1050][2] = {0};
int out[1050][1050] = {0};
 
 
int main()
{
    row.resize(1050);
    col.resize(1050);
 
    cin >> k >> n >> m;
 
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        row[x].push_back(y);
        inDegreeR[y]++;
    }
 
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        col[x].push_back(y);
        inDegreeC[y]++;
    }
 
 
 
    stack<int> R0;
    vector<int> res;
 
    for (int i = 1; i <= k; i++)
    {
        if (inDegreeR[i] == 0)
        {
            R0.push(i);
        }
    }
 
 
    while (!R0.empty())
    {
        int temp = R0.top();
        R0.pop();
        res.push_back(temp);
 
        visR[temp] = 1;
        
        for (int i = 0; i < row[temp].size(); i++)
        {
            inDegreeR[row[temp][i]]--;
        }
 
        for (int i = 1; i <= k; i++)
        {
            if (inDegreeR[i] == 0 && !visR[i])
            {
                R0.push(i);
            }
        }
    }
 
    if (res.size() != k)
    {
        cout << -1 << endl;
        return 0;
    }
 
 
    for (int i = 0; i < res.size(); i++)
    {
        //cout << res[i] << " ";
        pos[res[i]][0] = i;
    }
 
    //cout << endl;
 
 
 
 
 
 
    stack<int> C0;
    vector<int> resc;
 
    for (int i = 1; i <= k; i++)
    {
        if (inDegreeC[i] == 0)
        {
            C0.push(i);
        }
    }
 
 
    while (!C0.empty())
    {
        int temp = C0.top();
        C0.pop();
        resc.push_back(temp);
 
        visC[temp] = 1;
 
        for (int i = 0; i < col[temp].size(); i++)
        {
            inDegreeC[col[temp][i]]--;
        }
 
        for (int i = 1; i <= k; i++)
        {
            if (inDegreeC[i] == 0 && !visC[i])
            {
                C0.push(i);
            }
        }
    }
 
    if (resc.size() != k)
    {
        cout << -1 << endl;
        return 0;
    }
 
 
    for (int i = 0; i < resc.size(); i++)
    {
        //cout << resc[i] << " ";
        pos[resc[i]][1] = i;
 
    }
 
    cout << endl;
    for (int i = 1; i <= k; i++)
    {
        out[pos[i][0]][pos[i][1]] = i;
    }
 
    vector<vector<int>> ans;
 
 
    for (int i = 0; i < k; i++)
    {
        vector<int> temp;
        for (int j = 0; j < k; j++)
        {
            
            cout << out[i][j] << " ";
            temp.push_back(out[i][j]);
        }
        ans.push_back(temp);
        cout << endl;
    }
 
    /*cout << endl << endl;
 
    for (int i = 1; i <= k; i++)
    {
        cout << pos[i][0] << " " << pos[i][1] << endl;
    }*/
 
    
 
    return 0;
}
¡Á
