#include "nameTables.h"

void NameTables::deleteMembers(string id)
{
	cout << "1 删除学生 2 删除课程" << endl;
	int sel = 0;
	cin >> sel;

	if (sel == 1)
	{
		cout << "输入学生学号" << endl;
		string id;
		cin >> id;
		Student stu;
		this->studentHashTable.deleteItem(id);
		/*遍历student内置的链表*/
		for (CurriculumNode* ptr = stu.head; ptr != NULL; ptr = ptr->next)
		{
			Curriculum cri;
			this->curriculumHashTable.deleteItem(ptr->id);
		}


	}
	else
	{
		cout << "输入课程课号" << endl;
		string id;
		cin >> id;
		Curriculum cri;
		this->curriculumHashTable.deleteItem(id);
		/*遍历student内置的链表*/
		for (StudentNode* ptr = cri.head; ptr != NULL; ptr = ptr->next)
		{
			Student stu;
			this->studentHashTable.deleteItem(ptr->id);
		}

	}

}


