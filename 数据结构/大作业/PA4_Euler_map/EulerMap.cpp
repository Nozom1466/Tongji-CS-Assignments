#include <iostream>
#include <string>
using namespace std;
int map[10][10];
int n = 5;
int cnt = 0;

void dfs(int x, int k, string s)
{
	if (k >= 8)
	{
		cnt++;
		cout << s << endl;
		return;
	}

	for (int y = 1; y <= 5; y++)
	{
		if (map[x][y] == 1)
		{
			map[x][y] = 0;
			map[y][x] = 0;

			if (k == 0)
				dfs(y, k + 1, s  + to_string(y));
			else
				dfs(y, k + 1, s + " -> " + to_string(y));


			map[x][y] = 1;
			map[y][x] = 1;
		}
	}
}



int main()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (i == j) map[i][j] = 0;
			else map[i][j] = 1;
		}
	}
	map[1][4] = 0;
	map[4][1] = 0;
	map[2][4] = 0;
	map[4][2] = 0;

	string s = "1 -> ";
	dfs(1, 0, s);


	cout << endl << "一笔画总数为：" << cnt << endl;
	return 0;
}