
#include "curriculum.h"
#include "student.h"
#include "nameTables.h"

Curriculum::Curriculum() {}
Curriculum::Curriculum(string id = " ", string name = " ", string credit = " ")
{
	this->id = id;
	this->name = name;
	this->credit = credit;
	StudentNode* head = (StudentNode*)malloc(sizeof(StudentNode));
	this->head = head;
	this->head->next = NULL;
}

int Curriculum::deleteItem(string s)
{
	for (StudentNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		if (ptr->next->id == s)
		{
			StudentNode* q = ptr->next;
			ptr->next = q->next;
			free(q);
			cout << setw(34) << "已删除" << endl;
			return 1;

		}
	}
	return 0;
	
}

int Curriculum::addItem(string id)
{
	/*查重*/
	StudentNode* rear = new(nothrow)StudentNode;
	rear = this->head;
	if (rear == NULL) exit(1);
	
	for (StudentNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		if (ptr->next->id == id)
		{
			cout << setw(34) << "学生已经存在" << endl;
			return 0;
		}
		rear = ptr;
	}

	StudentNode* p = new(nothrow)StudentNode;
	if (p == NULL) exit(1);
	p->id = id;
	p->next = rear->next;
	rear->next = p;
	cout << setw(34) << "插入成功" << endl;
	return 1;
}

int Curriculum::find(string s)
{
	for (StudentNode* ptr = this->head; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->id == s)
		{

			//cout << ptr->student.name << " " << ptr->student.id << " " << ptr->student.gender << endl;
			return 1;
		}
	}
	return 0;
}
