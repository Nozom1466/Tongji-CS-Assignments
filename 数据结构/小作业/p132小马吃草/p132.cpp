//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
#include <queue>
#include <vector>
#include <string.h>
using namespace std;
 
const int N = 5000;
 
typedef pair<int, int> PII;
 
int n;      // �������
int h[N], w[N], e[N], ne[N], idx;       // �ڽӱ�洢���б�
int dist[N];        // �洢���е㵽1�ŵ�ľ���
bool st[N];     // �洢ÿ�������̾����Ƿ���ȷ��
 
 
void add(int a, int b, int weight)
{
    e[idx] = b;
    ne[idx] = h[a];
    w[idx] = weight;
    h[a] = idx++;
 
    e[idx] = a;
    ne[idx] = h[b];
    w[idx] = weight;
    h[b] = idx++;
 
 
}
 
// sta�ŵ㵽n�ŵ����̾��룬��������ڣ��򷵻�-1
void dijkstra(int sta)
{
    memset(dist, 0x3f, sizeof (dist));
    //memset(dist, 0, sizeof(st));
    dist[sta] = 0;
    priority_queue<PII, vector<PII>, greater<PII>> heap;
    heap.push({ 0, sta });      // first�洢���룬second�洢�ڵ���
 
    while (heap.size())
    {
        auto t = heap.top();
        heap.pop();
 
        int ver = t.second, distance = t.first;
        
        if (st[ver]) continue;
       //st[ver] = true;
 
 
        for (int i = h[ver]; i != -1; i = ne[i])
        {
            int j = e[i];
            if (dist[j] > distance + w[i])
            {
                dist[j] = distance + w[i];
                heap.push({ dist[j], j });
            }
        }
    }
 
    //if (dist[n] == 0x3f3f3f3f) return -1;
    
}
 
int way[1005][1005];
 
 
 
int main()
{
    int m;
    cin >> n >> m;
 
    memset(h, -1, sizeof(h));
 
    for (int i = 1; i <= m; i++)
    {
        int x_i, y_i, w_i;
        cin >> x_i >> y_i >> w_i;
        add(x_i, y_i, w_i);
    }
 
    /*for (int i = 0; i <= m; i++)
    {
        cout << w[i] << " ";
    }*/
 
 
    int h, r;
    cin >> h >> r;
 
    vector<int> gr;
 
    for (int i = 1; i <= h; i++)
    {
        int grass;
        cin >> grass;
        gr.push_back(grass);
        dijkstra(grass);
        for (int i = 1; i <= n; i++)
        {
            way[grass][i] = dist[i];
        }
    }
 
 
    for (int i = 1; i <= r; i++)
    {
        int start_j, end_j;
        cin >> start_j >> end_j;
 
        int temp = 0x3f3f3f3f;
        for (int j = 0; j < gr.size(); j++)
        {
           
            temp = (temp > way[gr[j]][start_j] + way[gr[j]][end_j]) ? way[gr[j]][start_j] + way[gr[j]][end_j] : temp;
           
        }
 
     
        cout << temp << endl;
    }
 
    return 0;
}
��
