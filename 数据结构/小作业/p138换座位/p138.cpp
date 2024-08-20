//Paste your code here
//NOTE: If you use java, your Main class must be named solution
class Solution {
public:
    int solve(std::vector<vector<std::string>>& old_chart, std::vector<std::vector<std::string>>& new_chart)
    {
        vector<vector<int>> vis;
        vis.resize(210);
        for (int i = 0; i < 200; i++)
        {
            vis[i].resize(210);
        }
 
 
 
        int step = 0;
        int n = old_chart.size();
        int m = old_chart[0].size();
 
 
        map<string, pair<int, int>> mp;
 
 
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                mp.insert({ new_chart[i][j], {i, j} });
            } 
        }
 
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                while (old_chart[i][j] != new_chart[i][j])
                {
                    string temp = old_chart[i][j];
                                     
                    int x = mp[temp].first;
                    int y = mp[temp].second;
                    swap(old_chart[x][y], old_chart[i][j]);
                    step++;
                    //vis[x][y] = 1;
                        
                        
                    
                }
                //vis[i][j] = 1;
            }
        }
 
        return step;
 
    }
};
¡Á
