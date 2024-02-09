#include <iostream>
#include <iomanip>
#include "nameTables.h"
#include "student.h"
#include "curriculum.h"
#include <fstream>
#include "GlobalFile.h"
#include <cstdlib>
#include <Windows.h>
#include <cctype>
#include <cstring>
using namespace std;
const int N_stu = 3000;
const int N_cur = 40000;
const double upLimitOfcredit = 11;
const double bottomLimitOfcredit = 0.5;



void openning()
{
	cout << setiosflags(ios::left);
	cout << "********" << "******************" << "******** " << endl;
	cout << "*-------" << setw(18) << "1.输入所有学生信息" << "-------*" << endl;
	cout << "*-------" << setw(18) << "2.输入所有课程信息" << "-------*" << endl;
	cout << "*-------" << setw(18) << "3.查找学生记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "4.删除学生记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "5.插入学生记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "6.查找课程记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "7.删除课程记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "8.插入课程记录" << "-------*" << endl;
	cout << "*-------" << setw(18) << "9.学生注册课程" << "-------*" << endl;
	cout << "*-------" << setw(18) << "10.学生退出课程" << "-------*" << endl;
	cout << "*-------" << setw(18) << "11.课程退课学生" << "-------*" << endl;
	cout << "*-------" << setw(18) << "12.输出选课名单" << "-------*" << endl;
	cout << "*-------" << setw(18) << "13.输出课程列表" << "-------*" << endl;
	cout << "*-------" << setw(18) << "14.退出选课程序" << "-------*" << endl;
	cout << "********" << setw(18) << "******************" << "******** " << endl;
	cout << setiosflags(ios::right);


}

bool checkStu(string id, string name, string gender)
{
	if (id.size() != 7)
	{
		cout << setw(34) << "学号位数不对,应该是7位" << endl;
		return false;
	}

	else if (id[0] == 0)
	{
		cout << setw(34) << "同学，你是第几届的呀，咋学号第一位是0呢" << endl;
		return false;
	}

	else if (name.size() > 120)
	{
		cout << setw(34) << "同学，你这名字够长，比毕加索的都长" << endl;
		return true;
	}

	else if (!(gender == "男" || gender == "女" || gender == "male" || gender == "female" || gender == "中性" || gender == "neutral gender"))
	{
		cout << setw(34) << "请检查性别" << endl;
		return false;
	}

	else
	{
		return true;
	}

}

bool checkStuId(string id)
{
	if (id.size() != 7)
	{
		cout << setw(34) << "学号位数不对,应该是7位" << endl;
		return false;
	}

	else if (id[0] == 0)
	{
		cout << setw(34) << "同学，你是第几届的呀，咋学号第一位是0呢" << endl;
		return false;
	}

	else
	{
		return true;
	}
}

bool checkCri(string id, string name, string credit)
{
	for (int i = 0; i < credit.size(); i++)
	{
		if (!isdigit(credit[i]))
		{
			cout << setw(34) << "学分必须是纯数字" << endl;
			return false;
		}
	}

	double sc = atof(credit.c_str());

	if (id.size() > 10)
	{
		cout << setw(34) << "课程号太长" << endl;
		return false;
	}
	else if (name.size() > 120)
	{
		cout << setw(34) << "课程名太长" << endl;
		return false;
	}

	else if (upLimitOfcredit - sc < 1e-6 || sc - bottomLimitOfcredit < 1e-6)
	{
		cout << setw(34) << "学分范围不对，请检查范围" << endl;
		return false;
	}

	else
	{
		return true;
	}
}

bool checkCriId(string id)
{
	if (id.size() > 10)
	{
		cout << setw(34) << "课程号太长" << endl;
		return false;
	}
	else
	{
		return true;
	}
}

int checkStuNum(int n)
{
	int s = n;
	while (s > N_stu)
	{
		cout << setw(34) << "人数太多啦！！！！" << endl;
		cout << setw(34) << "人数小于" << N_stu << "请重新输入:" << endl;
		cin >> s;
	}

	return s;
}

void init(NameTables& nameTable)
{
	/*初始化学生*/
	ifstream stufile;
	stufile.open(StudentNameTable, ios::in);
	if (!stufile.is_open())
	{
		cout << "文件不存在" << endl;
		stufile.close();
		return;
	}

	string id, name, gender;

	while (stufile >> id && stufile >> name && stufile >> gender)
	{
		/*添加学生*/
		Student stu = Student(id, name, gender);
		nameTable.studentHashTable.insert(stu);

		/*添加学生课程表*/
		ifstream schedule;
		string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
		schedule.open(path, ios::in);

		if (!schedule.is_open())
		{
			cout << "文件不存在" << endl;
			schedule.close();
			return;
		}

		string criId;
		while (schedule >> criId)
		{
			stu.addItem(criId);
		}

	}

	/*初始化课程*/
	ifstream crifile;
	crifile.open(CurriculumNameTable, ios::in);
	if (!crifile.is_open())
	{
		cout << "文件不存在" << endl;
		crifile.close();
		return;
	}

	//string id, name, credit;
	string credit;

	while (crifile >> id && crifile >> name && crifile >> credit)
	{
		/*添加课程*/
		Curriculum cri = Curriculum(id, name, credit);
		nameTable.curriculumHashTable.insert(cri);

		/*添加人员名单*/
		ifstream registers;
		string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
		registers.open(path, ios::in);

		if (!registers.is_open())
		{
			cout << "文件不存在" << endl;
			registers.close();
			return;
		}

		string stuId;
		while (registers >> stuId)
		{
			cri.addItem(stuId);
		}

	}

}

void deleteSpecificLine(string filePath, string idToDel)
{
	fstream in(filePath, ios::in);//原文件
	fstream out("dataBase\\test.txt", ios::out | ios::trunc);//中间文件
	string id, name, info;
	while (in >> id && in >> name && in >> info)
	{
		if (id == idToDel)//比较test.txt每一行的内容和要删除的是否一致，一致就跳过(不懂为啥跳过看文章开头的方法)
			continue;
		out << id << " " << name << " " << info << "\n";//不一致的内容写到temp.txt中，注意换行
	}
	in.close();//关闭流
	out.close();

	fstream outfile("dataBase\\test.txt", ios::in);
	fstream infile(filePath, ios::out);
	while (outfile >> id && outfile >> name && outfile >> info) //将temp.txt的内容写到test.txt
	{
		infile << id << " " << name << " " << info << "\n";
	}
	string pat = "dataBase\\test.txt";
	const char* path = pat.c_str();
	remove(path);//删除temp.txt
	outfile.close();//关闭流
	infile.close();

}

void BatchImportStudentInfo(NameTables& nametable)
{
	cout << "请输入学生人数(小于" << N_stu << ")" << endl;
	int n;
	cin >> n;

	n = checkStuNum(n);

	for (int i = 1; i <= n; i++)
	{
		cout << "输入 学号 姓名 性别" << endl;
		string id, name, gender;
		cin >> id >> name >> gender;

		/*检验输入合法性*/
		while (!checkStu(id, name, gender))
		{
			cout << "请重新输入" << endl;
			cout << "输入 学号 姓名 性别" << endl;
			cin >> id >> name >> gender;
		}

		Student stu = nametable.studentHashTable.find(id);
		if (stu.id == " ")
		{
			stu = Student(id, name, gender);
			nametable.studentHashTable.insert(stu);

			/*创建课程表文件*/
			ofstream ofs;
			string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
			ofs.open(path, ios::out);
			ofs.close();

			/*在学生总表中添加名字*/
			ofstream namefile;
			namefile.open(StudentNameTable, ios::app);
			namefile << id << " " << name << " " << gender << endl;
			namefile.close();
		}

		else
		{
			cout << setw(34) << "已经有此人" << endl;
			cout << setw(34) << "此次不计,重新输入" << endl;
			i -= 1;
		}
	}
}

void BatchImportCurriculumInfo(NameTables& nametable)
{
	cout << "请输入课程数量" << endl;
	int n;
	cin >> n;

	n = checkStuNum(n);

	for (int i = 1; i <= n; i++)
	{
		cout << "输入 课号 名称 学分" << endl;
		string id, name, credit;
		cin >> id >> name >> credit;

		while (!checkCri(id, name, credit))
		{
			cout << "请重新输入" << endl;
			cout << "输入 课号 名称 学分" << endl;
			cin >> id >> name >> credit;
		}



		Curriculum cri = nametable.curriculumHashTable.find(id);
		if (cri.id == " ")
		{
			cri = Curriculum(id, name, credit);
			nametable.curriculumHashTable.insert(cri);

			/*创建课程表文件*/
			ofstream ofs;
			string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
			ofs.open(path, ios::out);
			ofs.close();

			/*在课程总表中添加名字*/
			ofstream namefile;
			namefile.open(CurriculumNameTable, ios::app);
			namefile << id << " " << name << " " << credit << endl;
			namefile.close();
		}
		else
		{
			cout << setw(34) << "已经有此课" << endl;
		}
	}
}

void FindStudent(NameTables& nametable)
{
	cout << "输入查找的学生的学号：" << endl;
	string id;
	cin >> id;

	while (!checkStuId(id))
	{
		cout << setw(34) << "请重新输入" << endl;
		cin >> id;
	}

	nametable.studentHashTable.find(id);
}

void DeleteStudent(NameTables& nametable)
{
	cout << "输入学生学号" << endl;
	string s;
	cin >> s;

	while (!checkStuId(s))
	{
		cout << "请重新输入" << endl;
		cin >> s;
	}

	Student stu = nametable.studentHashTable.deleteItem(s);

	if (stu.id != " ")
	{
		/*先删除课程表文件*/
		string temp_p = "dataBase\\StudentFiles\\" + s + "Schedule.txt";
		const char* path = temp_p.c_str();
		remove(path);

		/*再删除总表学生*/
		deleteSpecificLine(temp_p, stu.id);


		for (CurriculumNode* ptr = stu.head->next; ptr != NULL; ptr = ptr->next)
		{
			string temp_id = ptr->id;
			Curriculum cri = nametable.curriculumHashTable.find(temp_id);
			cri.deleteItem(temp_id);

			/*课程注册名单里删除注册的学生*/
			ofstream ofs;
			string path = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
			ofs.open(path, ios::out);
			deleteSpecificLine(path, stu.id);
			ofs.close();
		}

		cout << setw(34) << "删除成功" << endl;

	}
	else
	{
		cout << setw(34) << "查无此人" << endl;
	}
}

void InsertStudent(NameTables& nametable)
{
	cout << "输入学生 学号 姓名 性别" << endl;
	string id, name, gender;
	cin >> id >> name >> gender;

	while (!checkStu(id, name, gender))
	{
		cout << "请重新输入" << endl;
		cout << "输入 学号 姓名 性别" << endl;
		cin >> id >> name >> gender;
	}
	

	Student stu = nametable.studentHashTable.find(id);
	if (stu.id == " ")
	{
		stu = Student(id, name, gender);
		nametable.studentHashTable.insert(stu);

		/*创建课程表文件*/
		ofstream ofs;
		string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
		ofs.open(path, ios::out);
		ofs.close();

		/*在学生总表中添加名字*/
		ofstream namefile;
		namefile.open(StudentNameTable, ios::app);
		namefile << id << " " << name << " " << gender << endl;
		namefile.close();
	}

	else
	{
		cout << setw(34) << "已经有此人" << endl;
	}
}

void FindCurriculum(NameTables& nametable)
{
	cout << "输入查找的课程的课号：" << endl;
	string id;
	cin >> id;

	while (!checkCriId(id))
	{
		cout << "请重新输入" << endl;
		cin >> id;
	}

	nametable.curriculumHashTable.find(id);
}

void DeleteCurriculum(NameTables& nametable)
{
	cout << "输入课程课号" << endl;
	string s;
	cin >> s;

	while (!checkCriId(s))
	{
		cout << "请重新输入" << endl;
		cin >> s;
	}

	Curriculum cri = nametable.curriculumHashTable.deleteItem(s);

	/*先删除课程注册的学生名单文件*/
	string temp_p = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
	const char* path = temp_p.c_str();
	remove(path);

	/*再删除总表课程*/
	deleteSpecificLine(temp_p, cri.id);


	if (cri.id != " ")
	{
		for (StudentNode* ptr = cri.head->next; ptr != NULL; ptr = ptr->next)
		{
			string temp_id = ptr->id;
			Student stu = nametable.studentHashTable.find(temp_id);
			stu.deleteItem(temp_id);

			/*课程注册名单里删除注册的学生*/
			ofstream ofs;
			string path = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
			ofs.open(path, ios::out);
			deleteSpecificLine(path, cri.id);
			ofs.close();
		}

	}
	else
	{
		cout << setw(34) << "查无此课" << endl;
	}
}

void InsertCurriclum(NameTables& nametable)
{
	cout << "输入课程 课号 名称 学分" << endl;
	string id, name, credit;
	cin >> id >> name >> credit;

	while (!checkCri(id, name, credit))
	{
		cout << "请重新输入" << endl;
		cout << "输入 课号 名称 学分" << endl;
		cin >> id >> name >> credit;
	}

	Curriculum cri = nametable.curriculumHashTable.find(id);
	if (cri.id == " ")
	{
		cri = Curriculum(id, name, credit);
		nametable.curriculumHashTable.insert(cri);

		/*创建课程表文件*/
		ofstream ofs;
		string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
		ofs.open(path, ios::out);
		ofs.close();

		/*在课程总表中添加名字*/
		ofstream namefile;
		namefile.open(CurriculumNameTable, ios::app);
		namefile << id << " " << name << " " << credit << endl;
		namefile.close();
	}
	else
	{
		cout << setw(34) << "已经有此课" << endl;
	}

}

void StudentRegisterForCurriclum(NameTables& nametable)
{
	cout << "输入学生 学号 姓名 性别（male / female）" << endl;
	string id, name, gender;
	cin >> id >> name >> gender;

	while (!checkStu(id, name, gender))
	{
		cout << "请重新输入" << endl;
		cout << "输入 学号 姓名 性别（male / female）" << endl;
		cin >> id >> name >> gender;
	}

	Student stu = nametable.studentHashTable.find(id);
	if (stu.id != " ")
	{
		cout << "输入课程课号" << endl;
		string id;
		cin >> id;

		while (!checkCriId(id))
		{
			cout << "请重新输入" << endl;
			cin >> id;
		}

		Curriculum cri = nametable.curriculumHashTable.find(id);
		if (cri.id == " ")
		{
			cout << setw(34) << "没有此课" << endl;
			return;
		}
		stu.addItem(cri.id);
		cri.addItem(stu.id);

		ofstream schedule;
		string path1 = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
		schedule.open(path1, ios::app);
		schedule << cri.id << " " << cri.name << " " << cri.credit << endl;
		schedule.close();

		ofstream registers;
		string path2 = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
		registers.open(path2, ios::app);
		registers << stu.id << " " << stu.name << " " << stu.gender << endl;
		registers.close();
	}
	else
	{
		cout << setw(34) << "查无此人" << endl;
	}
}

void StudentQuitCurriculum(NameTables& nametable)
{
	cout << "输入学生学号" << endl;
	string s;
	cin >> s;

	while (!checkStuId(s))
	{
		cout << "请重新输入" << endl;
		cin >> s;
	}

	Student stu = nametable.studentHashTable.find(s);
	if (stu.id != " ")
	{
		cout << "输入课程课号" << endl;
		string criId;
		cin >> criId;

		while (!checkCriId(criId))
		{
			cout << "请重新输入" << endl;
			cin >> criId;
		}

		Curriculum cri = nametable.curriculumHashTable.find(criId);
		if (cri.id != " ")
		{
			stu.deleteItem(cri.id);
			cri.deleteItem(stu.id);

			/*文件*/
			ofstream schedule;
			string path1 = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
			deleteSpecificLine(path1, cri.id);


			ofstream registers;
			string path2 = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
			deleteSpecificLine(path2, stu.id);
		}

		else
		{
			cout << setw(34) << "查无此课" << endl;
		}

	}
	else
	{
		cout << setw(34) << "查无此人" << endl;
	}
}

void CurriculumEliminateStudent(NameTables& nametable)
{
	cout << "输入课程课号" << endl;
	string s;
	cin >> s;

	while (!checkCriId(s))
	{
		cout << "请重新输入" << endl;
		cin >> s;
	}


	Curriculum cri = nametable.curriculumHashTable.find(s);
	if (cri.id != " ")
	{
		cout << "输入学生学号" << endl;
		string stuId;
		cin >> stuId;

		while (!checkStuId(stuId))
		{
			cout << "请重新输入" << endl;
			cin >> stuId;
		}

		Student stu = nametable.studentHashTable.find(stuId);
		if (stu.id != " ")
		{
			stu.deleteItem(cri.id);
			cri.deleteItem(stu.id);

			ofstream schedule;
			string path1 = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
			deleteSpecificLine(path1, cri.id);


			ofstream registers;
			string path2 = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
			deleteSpecificLine(path2, stu.id);
		}

		else
		{
			cout << setw(34) << "查无此人" << endl;
		}

	}
	else
	{
		cout << setw(34) << "查无此课" << endl;
	}

}

void PrintRegesters(NameTables& nametable)
{
	cout << "输入课程课号：：" << endl;
	string id;
	cin >> id;

	while (!checkCriId(id))
	{
		cout << "请重新输入" << endl;
		cin >> id;
	}

	Curriculum cri = nametable.curriculumHashTable.find(id);
	for (StudentNode* ptr = cri.head->next; ptr != NULL; ptr = ptr->next)
	{
		string temp_id = ptr->id;
		nametable.studentHashTable.find(temp_id);
	}
}

void PrintSchedules(NameTables& nametable)
{
	cout << "输入学生学号：" << endl;
	string id;
	cin >> id;

	while (!checkStuId(id))
	{
		cout << "请重新输入" << endl;
		cin >> id;
	}

	Student stu = nametable.studentHashTable.find(id);
	for (CurriculumNode* ptr = stu.head->next; ptr != NULL; ptr = ptr->next)
	{
		string temp_id = ptr->id;
		nametable.curriculumHashTable.find(temp_id);
	}
}

int main()
{
	int choice = 0;
	NameTables nametable;
	
	init(nametable);
	system("cls");

	while (1)
	{
		openning();
		cout << "请输入功能序号：" << endl;
		cout << "=输入栏=" << "==  ====  ====  ==" << "=输出栏=" << endl;
		cin >> choice;

		switch (choice)
		{
		case 1:/*输入所有学生信息*/
		{
			BatchImportStudentInfo(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 2:/*输入所有课程信息*/
		{
			BatchImportCurriculumInfo(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 3:/*查找学生记录*/
		{
			FindStudent(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 4:/*删除学生记录*/
		{
			DeleteStudent(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 5:/*插入学生记录*/
		{
			InsertStudent(nametable);
			system("pause");
			system("cls");

			break;
		}

		case 6:/*查找课程记录*/
		{
			FindCurriculum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 7:/*删除课程记录*/
		{
			DeleteCurriculum(nametable);
			system("pause");
			system("cls");
			break;

		}

		case 8:/*插入课程记录*/
		{
			InsertCurriclum(nametable);
			system("pause");
			system("cls");
			break;

		}

		case 9:/*学生注册课程*/
		{
			StudentRegisterForCurriclum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 10: /*学生退出课程*/
		{
			StudentQuitCurriculum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 11:/*课程退课学生*/
		{
			CurriculumEliminateStudent(nametable);
			system("pause");

			system("cls");
			break;
		}

		case 12:/*输出选课名单*/
		{
			PrintRegesters(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 13:/*输出课程列表*/
		{
			PrintSchedules(nametable);
			system("pause");
			system("cls");
			break;

		}

		default:
		{
			cout << setw(34) << "系统已关闭" << endl;
			return 0;
			break;
		}


		}
		system("cls");
	}				   
	return 0;
	
}

