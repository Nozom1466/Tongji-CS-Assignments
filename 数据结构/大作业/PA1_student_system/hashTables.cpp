#include "nameTables.h"

void NameTables::deleteMembers(string id)
{
	cout << "1 ɾ��ѧ�� 2 ɾ���γ�" << endl;
	int sel = 0;
	cin >> sel;

	if (sel == 1)
	{
		cout << "����ѧ��ѧ��" << endl;
		string id;
		cin >> id;
		Student stu;
		this->studentHashTable.deleteItem(id);
		/*����student���õ�����*/
		for (CurriculumNode* ptr = stu.head; ptr != NULL; ptr = ptr->next)
		{
			Curriculum cri;
			this->curriculumHashTable.deleteItem(ptr->id);
		}


	}
	else
	{
		cout << "����γ̿κ�" << endl;
		string id;
		cin >> id;
		Curriculum cri;
		this->curriculumHashTable.deleteItem(id);
		/*����student���õ�����*/
		for (StudentNode* ptr = cri.head; ptr != NULL; ptr = ptr->next)
		{
			Student stu;
			this->studentHashTable.deleteItem(ptr->id);
		}

	}

}


