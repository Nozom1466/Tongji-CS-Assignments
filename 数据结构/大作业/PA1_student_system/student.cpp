#include "student.h"
#include "curriculum.h"


Student::Student() {}

/*head指针申请内存*/
Student::Student(string id = " ", string name = " ", string gender = " ")
{
	this->id = id;
	this->name = name;
	this->gender = gender;
	this->head = (CurriculumNode*)malloc(sizeof(CurriculumNode));
	//this->head = head;
	this->head->next = NULL;
	
}

/*fix me : 根据不同的元素查找课程*//*暂定为根据名称查找*/
/*成功删除几门课*/

int Student::deleteItem(string s)
{
	/*
	* @param s : 学生学号
	*/
	for (CurriculumNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		//遍历目标节点的前一个节点
		if (ptr->next->id == s)
		{
			CurriculumNode* q = ptr->next;
			ptr->next = q->next;
			free(q);
			cout << setw(34) << "已删除" << endl;
			return 1;
		}
	}
	return 0;

}

int Student:: addItem(string id)
{
	/*查重*/
	CurriculumNode* rear = new(nothrow)CurriculumNode;
	rear = this->head;
	if (rear == NULL) exit(1);
	for (CurriculumNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		if (ptr->next->id == id)
		{
			cout << setw(34) << "已经存在此课" << endl;
			return 0;
		}
		rear = ptr;
	}

	CurriculumNode* p = new(nothrow)CurriculumNode;
	if (p == NULL) exit(1);
	p->id = id;
	p->next = rear->next;
	rear->next = p;
	cout << setw(34) << "插入成功" << endl;
	return 1;
}

int Student::find(string s)
{
	for (CurriculumNode* ptr = this->head; ptr != NULL; ptr = ptr->next)
	{
		if (ptr->id == s)
		{
			return 1;
		} 
	}
	return 0;
}


