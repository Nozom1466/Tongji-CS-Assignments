//Paste your code here
//NOTE: If you use java, your Main class must be named solution
 
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <deque>
using namespace std;
int n;
string inst;
map<int, int> mp;
int colorList[] = { 65 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 74, 81, 75 };
 
 
 
class Poker
{
public:
    Poker(string mcolor, int mnum)
    {
        this->color = mcolor;
        this->num = mnum;
    }
 
    string color;
    int num;
};
 
 
 
deque<Poker> piler;
 
void append(Poker poker)
{
    piler.push_back(poker);
}
 
 
bool cmp(Poker p1, Poker p2)
{
    return mp[p1.num] < mp[p2.num];
}
 
 
void extract(string color)
{
    deque<Poker> tmp;
    for (int i = 0; i < piler.size(); i++)
    {
        if (piler[i].color == color)
        {
            tmp.push_back(piler[i]);
            piler.erase(piler.begin() + i);
            i--;
        }
        
    }
 
    /*for (deque<Poker>::iterator it = piler.begin(); it != piler.end(); it++);
    {
        if (it->color == color)
        {
            tmp.push_back(*it);
            it = piler.erase(it);
            it--;
        }
    }*/
 
    sort(tmp.begin(), tmp.end(), cmp);
 
    /*for (int i = tmp.size() - 1; i >= 0; i--)
    {
        cout << tmp[i].color << " " << tmp[i].num << " ";
    }
    cout << endl;*/
 
    for (int i = tmp.size() - 1; i >= 0; i--)
    {
        piler.push_front(tmp[i]);
    }
}
 
 
 
 
void revert()
{
    deque<Poker> newPiler;
 
    while (piler.size())
    {
        newPiler.push_back(piler.back());
        piler.pop_back();
    }
 
    piler = newPiler;
 
}
 
 
Poker pop()
{
    if (!piler.size())
    {
        return Poker("NULL", '0');
    }
 
    else
    {
        Poker p = piler.front();
        piler.begin() = piler.erase(piler.begin());
        return p;
    }
}
 
 
int getAscii(string s)
{
    if (s == "J") return 74;
    if (s == "Q") return 81;
    if (s == "K") return 75;
    if (s == "A") return 65;
 
    if (s == "10") return 10;
    if (s == "2") return 2;
    if (s == "3") return 3;
    if (s == "4") return 4;
    if (s == "5") return 5;
    if (s == "6") return 6;
    if (s == "7") return 7;
    if (s == "8") return 8;
    if (s == "9") return 9;
}
 
 
 
 
int main()
{
    for (int i = 0; i < 13; i++)
    {
        mp.insert({ colorList[i], i + 1 });
    }
 
 
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> inst;
        if (inst == "Pop")
        {
            Poker poker = pop();
            if (poker.color == "NULL")
            {
                cout << "NULL" << endl;
            }
            else
            {
                if (poker.num < 11)
                {
                    cout << poker.color << " " << poker.num << endl;
                }
 
                else
                {
                    cout << poker.color << " "  << (char)poker.num << endl;
                }
 
 
            }
 
        }
 
        else if (inst == "Append")
        {
            string color;
            string num1;
            
 
            cin >> color >> num1;
            int num = getAscii(num1);
 
 
 
            append(Poker(color, num));
 
        }
 
        else if (inst == "Revert")
        {
            revert();
        }
 
 
        else if (inst == "Extract")
        {
            string color;
            cin >> color;
            extract(color);
        }
 
    }
 
    if (!piler.size()) cout << "NULL";
    else
    {
        for (int i = 0; i < piler.size(); i++)
        {
            if (piler[i].num < 11)
            {
                cout << piler[i].color << " " << piler[i].num << endl;
            }
 
            else
            {
                cout << piler[i].color << " " << (char)piler[i].num << endl;
            }
        }
    }
 
 
 
    return 0;
}
 
/*
10
Pop
Append Heart 6
Append Club 2
Pop
Append Heart 2
Revert
Append Heart 3
Append Heart J
Append Heart 2
Extract Club
 
 
9
Pop
Append Heart 6
Append Club 2
Pop
Append Heart 2
Revert
Append Heart 3
Append Heart J
Append Heart 2
 
11
Pop
Append Heart 6
Append Club 2
Append Club A
Pop
Append Heart 2
Revert
Append Heart 3
Append Heart J
Append Heart 2
Extract Club
 
 
*/
¡Á
