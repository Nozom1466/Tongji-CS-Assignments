//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
class FreqStack {
    map<int, int> cnt;
    vector<stack<int>> stacks;
public:
    void push(int val) {
        if (cnt[val] == stacks.size()) // ���Ԫ�ص�Ƶ���Ѿ���Ŀǰ���ģ������ֳ�����һ��
            stacks.push_back({}); // ��ô���봴��һ����ջ
        stacks[cnt[val]].push(val);
        ++cnt[val]; // ����Ƶ��
    }
 
    int pop() {
        int val = stacks.back().top(); // �������Ҳ�ջ��ջ��
        stacks.back().pop();
        if (stacks.back().empty()) // ջΪ��
            stacks.pop_back(); // ɾ��
        --cnt[val]; // ����Ƶ��
        return val;
    }
};
