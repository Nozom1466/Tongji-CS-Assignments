//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <deque>
using namespace std;
int maxa;
deque<int> stEle;
deque<int> stMax;
string s = " ";
 
 
int main()
{
    cin >> maxa;
    while (1)
    {
        cin >> s;
        if (s == "quit")
        {
            if (stEle.empty())
            {
                cout << "Queue is Empty" << endl;
                continue;
            }
            while (!stEle.empty())
            {
                cout << stEle.front() << " ";
                stEle.pop_front();
            }
            break;
        }
 
 
        else if (s == "enqueue")
        {
            int e;
            cin >> e;
 
            
            if (stEle.size() >= maxa)
            {
                cout << "Queue is  Full" << endl;
                continue;
            }
            stEle.push_back(e);
            if (stMax.empty())
            {
                stMax.push_back(e);
                continue;
            }
            while (!stMax.empty() && stMax.back() < e)
            {
                stMax.pop_back();
            }
            stMax.push_back(e);
        }
 
        else if (s == "dequeue")
        {
            if (stEle.empty())
            {
                cout << "Queue is Empty" << endl;
                continue;
            }
            cout << stEle.front() << endl;
 
            /*cout << "***********" << endl;
            for (int i = 0; i < stMax.size(); i++)
            {
                cout << stMax[i] << " ";
            }
            cout << endl << "*****************" << endl;*/
            if (stEle.front() == stMax.front())
            {
                //cout << 1111111111 << endl;
                stMax.pop_front();
            }
 
            stEle.pop_front();
 
        }
 
        else if (s == "max")
        {
            if (stEle.empty())
            {
                cout << "Queue is Empty" << endl;
                continue;
            }
            cout << stMax.front() << endl;
        }
    }
 
 
    return 0;
}
 
 
 
 
 
 
 
 
 
/*
4
enqueue 10
enqueue 5
enqueue 4
enqueue 6
dequeue
max
dequeue
max
quit
 
 
 
 
 
 
*/
¡Á
