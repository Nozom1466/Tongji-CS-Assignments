#include "LInkQueue.hpp"
using namespace std;

int main11()
{
	LinkQueue<int> q = LinkQueue<int>();

	int a = 9;
	q.enqueue(a);
	q.enqueue(a + 2);
	q.enqueue(a - 1);
	q.enqueue(a * 2);

	//判空测试
	cout << q.isEmpty() << endl;

	//获取长度测试
	cout << q.size() << endl;

	//出队测试
	q.dequeue();
	q.dequeue();

	cout << q.size() << endl;

	//获取队列第一名测试
	cout << q.getHead() << endl;


	//展示信息测试
	q.showInfo();
	
	
	
	
	return 0;
}