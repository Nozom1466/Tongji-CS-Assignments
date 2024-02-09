#include "student.h"
#include "curriculum.h"


Student::Student() {}

/*headָ�������ڴ�*/
Student::Student(string id = " ", string name = " ", string gender = " ")
{
	this->id = id;
	this->name = name;
	this->gender = gender;
	this->head = (CurriculumNode*)malloc(sizeof(CurriculumNode));
	//this->head = head;
	this->head->next = NULL;
	
}

/*fix me : ���ݲ�ͬ��Ԫ�ز��ҿγ�*//*�ݶ�Ϊ�������Ʋ���*/
/*�ɹ�ɾ�����ſ�*/

int Student::deleteItem(string s)
{
	/*
	* @param s : ѧ��ѧ��
	*/
	for (CurriculumNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		//����Ŀ��ڵ��ǰһ���ڵ�
		if (ptr->next->id == s)
		{
			CurriculumNode* q = ptr->next;
			ptr->next = q->next;
			free(q);
			cout << setw(34) << "��ɾ��" << endl;
			return 1;
		}
	}
	return 0;

}

int Student:: addItem(string id)
{
	/*����*/
	CurriculumNode* rear = new(nothrow)CurriculumNode;
	rear = this->head;
	if (rear == NULL) exit(1);
	for (CurriculumNode* ptr = this->head; ptr->next != NULL; ptr = ptr->next)
	{
		if (ptr->next->id == id)
		{
			cout << setw(34) << "�Ѿ����ڴ˿�" << endl;
			return 0;
		}
		rear = ptr;
	}

	CurriculumNode* p = new(nothrow)CurriculumNode;
	if (p == NULL) exit(1);
	p->id = id;
	p->next = rear->next;
	rear->next = p;
	cout << setw(34) << "����ɹ�" << endl;
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


