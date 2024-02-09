//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
class FreqStack {
    map<int, int> cnt;
    vector<stack<int>> stacks;
public:
    void push(int val) {
        if (cnt[val] == stacks.size()) // 这个元素的频率已经是目前最多的，现在又出现了一次
            stacks.push_back({}); // 那么必须创建一个新栈
        stacks[cnt[val]].push(val);
        ++cnt[val]; // 更新频率
    }
 
    int pop() {
        int val = stacks.back().top(); // 弹出最右侧栈的栈顶
        stacks.back().pop();
        if (stacks.back().empty()) // 栈为空
            stacks.pop_back(); // 删除
        --cnt[val]; // 更新频率
        return val;
    }
};
