//Paste your code here
//NOTE: If you use java, your Main class must be named solution
#include <iostream>
using namespace std;
int n;
int q[100005];
long long ans;
long long tmp[100009];
 
 
void ms(int q[], int l, int r)
{
    if (l >= r) return;
    int mid = l + r >> 1;
    ms(q, l, mid), ms(q, mid + 1, r);
 
    int k = 0, i = l, j = mid + 1;
    while (i <= mid && j <= r)
    {
        if (q[i] <= q[j])
        {
            tmp[k++] = q[i++];
        }
 
        else tmp[k++] = q[j++], ans += (mid - i + 1);
    }
 
    while (i <= mid) tmp[k++] = q[i++];
    while (j <= r) tmp[k++] = q[j++];
 
    for (int i = l, j = 0; i <= r; i++, j++) q[i] = tmp[j];
}
 
 
 
int main()
{
    int ope = -1;
    while (1)
    {
        cin >> ope;
        if (ope == 0)
            break;
 
        ans = 0;
        for (int i = 0; i < ope; i++)
        {
            cin >> q[i];
        }
        ms(q, 0, ope - 1);
        cout << ans << endl;
    }
 
    return 0;
}
¡Á
