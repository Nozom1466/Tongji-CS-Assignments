//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
class Solution {
public:
    std::string largestNumber(std::vector<int>& nums) 
    {
        sort(nums.begin(), nums.end(), [](const int &a, const int &b)
            {
                long long q1 = 10, q2 = 10;
                while (q1 <= a) q1 *= 10;
                while (q2 <= b) q2 *= 10;
                return q2 * a + b > q1 * b + a;
            }
        );
        if (nums[0] == 0)
            return "0";
        string rt;
        for (int i = 0; i < nums.size(); i++)
        {
            rt += to_string(nums[i]);
        }
        return rt;
    }
};
¡Á
