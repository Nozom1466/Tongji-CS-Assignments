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

	//�пղ���
	cout << q.isEmpty() << endl;

	//��ȡ���Ȳ���
	cout << q.size() << endl;

	//���Ӳ���
	q.dequeue();
	q.dequeue();

	cout << q.size() << endl;

	//��ȡ���е�һ������
	cout << q.getHead() << endl;


	//չʾ��Ϣ����
	q.showInfo();
	
	
	
	
	return 0;
}