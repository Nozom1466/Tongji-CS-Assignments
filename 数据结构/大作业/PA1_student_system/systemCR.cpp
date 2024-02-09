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
	cout << "*-------" << setw(18) << "1.��������ѧ����Ϣ" << "-------*" << endl;
	cout << "*-------" << setw(18) << "2.�������пγ���Ϣ" << "-------*" << endl;
	cout << "*-------" << setw(18) << "3.����ѧ����¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "4.ɾ��ѧ����¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "5.����ѧ����¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "6.���ҿγ̼�¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "7.ɾ���γ̼�¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "8.����γ̼�¼" << "-------*" << endl;
	cout << "*-------" << setw(18) << "9.ѧ��ע��γ�" << "-------*" << endl;
	cout << "*-------" << setw(18) << "10.ѧ���˳��γ�" << "-------*" << endl;
	cout << "*-------" << setw(18) << "11.�γ��˿�ѧ��" << "-------*" << endl;
	cout << "*-------" << setw(18) << "12.���ѡ������" << "-------*" << endl;
	cout << "*-------" << setw(18) << "13.����γ��б�" << "-------*" << endl;
	cout << "*-------" << setw(18) << "14.�˳�ѡ�γ���" << "-------*" << endl;
	cout << "********" << setw(18) << "******************" << "******** " << endl;
	cout << setiosflags(ios::right);


}

bool checkStu(string id, string name, string gender)
{
	if (id.size() != 7)
	{
		cout << setw(34) << "ѧ��λ������,Ӧ����7λ" << endl;
		return false;
	}

	else if (id[0] == 0)
	{
		cout << setw(34) << "ͬѧ�����ǵڼ����ѽ��զѧ�ŵ�һλ��0��" << endl;
		return false;
	}

	else if (name.size() > 120)
	{
		cout << setw(34) << "ͬѧ���������ֹ������ȱϼ����Ķ���" << endl;
		return true;
	}

	else if (!(gender == "��" || gender == "Ů" || gender == "male" || gender == "female" || gender == "����" || gender == "neutral gender"))
	{
		cout << setw(34) << "�����Ա�" << endl;
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
		cout << setw(34) << "ѧ��λ������,Ӧ����7λ" << endl;
		return false;
	}

	else if (id[0] == 0)
	{
		cout << setw(34) << "ͬѧ�����ǵڼ����ѽ��զѧ�ŵ�һλ��0��" << endl;
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
			cout << setw(34) << "ѧ�ֱ����Ǵ�����" << endl;
			return false;
		}
	}

	double sc = atof(credit.c_str());

	if (id.size() > 10)
	{
		cout << setw(34) << "�γ̺�̫��" << endl;
		return false;
	}
	else if (name.size() > 120)
	{
		cout << setw(34) << "�γ���̫��" << endl;
		return false;
	}

	else if (upLimitOfcredit - sc < 1e-6 || sc - bottomLimitOfcredit < 1e-6)
	{
		cout << setw(34) << "ѧ�ַ�Χ���ԣ����鷶Χ" << endl;
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
		cout << setw(34) << "�γ̺�̫��" << endl;
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
		cout << setw(34) << "����̫������������" << endl;
		cout << setw(34) << "����С��" << N_stu << "����������:" << endl;
		cin >> s;
	}

	return s;
}

void init(NameTables& nameTable)
{
	/*��ʼ��ѧ��*/
	ifstream stufile;
	stufile.open(StudentNameTable, ios::in);
	if (!stufile.is_open())
	{
		cout << "�ļ�������" << endl;
		stufile.close();
		return;
	}

	string id, name, gender;

	while (stufile >> id && stufile >> name && stufile >> gender)
	{
		/*���ѧ��*/
		Student stu = Student(id, name, gender);
		nameTable.studentHashTable.insert(stu);

		/*���ѧ���γ̱�*/
		ifstream schedule;
		string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
		schedule.open(path, ios::in);

		if (!schedule.is_open())
		{
			cout << "�ļ�������" << endl;
			schedule.close();
			return;
		}

		string criId;
		while (schedule >> criId)
		{
			stu.addItem(criId);
		}

	}

	/*��ʼ���γ�*/
	ifstream crifile;
	crifile.open(CurriculumNameTable, ios::in);
	if (!crifile.is_open())
	{
		cout << "�ļ�������" << endl;
		crifile.close();
		return;
	}

	//string id, name, credit;
	string credit;

	while (crifile >> id && crifile >> name && crifile >> credit)
	{
		/*��ӿγ�*/
		Curriculum cri = Curriculum(id, name, credit);
		nameTable.curriculumHashTable.insert(cri);

		/*�����Ա����*/
		ifstream registers;
		string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
		registers.open(path, ios::in);

		if (!registers.is_open())
		{
			cout << "�ļ�������" << endl;
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
	fstream in(filePath, ios::in);//ԭ�ļ�
	fstream out("dataBase\\test.txt", ios::out | ios::trunc);//�м��ļ�
	string id, name, info;
	while (in >> id && in >> name && in >> info)
	{
		if (id == idToDel)//�Ƚ�test.txtÿһ�е����ݺ�Ҫɾ�����Ƿ�һ�£�һ�¾�����(����Ϊɶ���������¿�ͷ�ķ���)
			continue;
		out << id << " " << name << " " << info << "\n";//��һ�µ�����д��temp.txt�У�ע�⻻��
	}
	in.close();//�ر���
	out.close();

	fstream outfile("dataBase\\test.txt", ios::in);
	fstream infile(filePath, ios::out);
	while (outfile >> id && outfile >> name && outfile >> info) //��temp.txt������д��test.txt
	{
		infile << id << " " << name << " " << info << "\n";
	}
	string pat = "dataBase\\test.txt";
	const char* path = pat.c_str();
	remove(path);//ɾ��temp.txt
	outfile.close();//�ر���
	infile.close();

}

void BatchImportStudentInfo(NameTables& nametable)
{
	cout << "������ѧ������(С��" << N_stu << ")" << endl;
	int n;
	cin >> n;

	n = checkStuNum(n);

	for (int i = 1; i <= n; i++)
	{
		cout << "���� ѧ�� ���� �Ա�" << endl;
		string id, name, gender;
		cin >> id >> name >> gender;

		/*��������Ϸ���*/
		while (!checkStu(id, name, gender))
		{
			cout << "����������" << endl;
			cout << "���� ѧ�� ���� �Ա�" << endl;
			cin >> id >> name >> gender;
		}

		Student stu = nametable.studentHashTable.find(id);
		if (stu.id == " ")
		{
			stu = Student(id, name, gender);
			nametable.studentHashTable.insert(stu);

			/*�����γ̱��ļ�*/
			ofstream ofs;
			string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
			ofs.open(path, ios::out);
			ofs.close();

			/*��ѧ���ܱ����������*/
			ofstream namefile;
			namefile.open(StudentNameTable, ios::app);
			namefile << id << " " << name << " " << gender << endl;
			namefile.close();
		}

		else
		{
			cout << setw(34) << "�Ѿ��д���" << endl;
			cout << setw(34) << "�˴β���,��������" << endl;
			i -= 1;
		}
	}
}

void BatchImportCurriculumInfo(NameTables& nametable)
{
	cout << "������γ�����" << endl;
	int n;
	cin >> n;

	n = checkStuNum(n);

	for (int i = 1; i <= n; i++)
	{
		cout << "���� �κ� ���� ѧ��" << endl;
		string id, name, credit;
		cin >> id >> name >> credit;

		while (!checkCri(id, name, credit))
		{
			cout << "����������" << endl;
			cout << "���� �κ� ���� ѧ��" << endl;
			cin >> id >> name >> credit;
		}



		Curriculum cri = nametable.curriculumHashTable.find(id);
		if (cri.id == " ")
		{
			cri = Curriculum(id, name, credit);
			nametable.curriculumHashTable.insert(cri);

			/*�����γ̱��ļ�*/
			ofstream ofs;
			string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
			ofs.open(path, ios::out);
			ofs.close();

			/*�ڿγ��ܱ����������*/
			ofstream namefile;
			namefile.open(CurriculumNameTable, ios::app);
			namefile << id << " " << name << " " << credit << endl;
			namefile.close();
		}
		else
		{
			cout << setw(34) << "�Ѿ��д˿�" << endl;
		}
	}
}

void FindStudent(NameTables& nametable)
{
	cout << "������ҵ�ѧ����ѧ�ţ�" << endl;
	string id;
	cin >> id;

	while (!checkStuId(id))
	{
		cout << setw(34) << "����������" << endl;
		cin >> id;
	}

	nametable.studentHashTable.find(id);
}

void DeleteStudent(NameTables& nametable)
{
	cout << "����ѧ��ѧ��" << endl;
	string s;
	cin >> s;

	while (!checkStuId(s))
	{
		cout << "����������" << endl;
		cin >> s;
	}

	Student stu = nametable.studentHashTable.deleteItem(s);

	if (stu.id != " ")
	{
		/*��ɾ���γ̱��ļ�*/
		string temp_p = "dataBase\\StudentFiles\\" + s + "Schedule.txt";
		const char* path = temp_p.c_str();
		remove(path);

		/*��ɾ���ܱ�ѧ��*/
		deleteSpecificLine(temp_p, stu.id);


		for (CurriculumNode* ptr = stu.head->next; ptr != NULL; ptr = ptr->next)
		{
			string temp_id = ptr->id;
			Curriculum cri = nametable.curriculumHashTable.find(temp_id);
			cri.deleteItem(temp_id);

			/*�γ�ע��������ɾ��ע���ѧ��*/
			ofstream ofs;
			string path = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
			ofs.open(path, ios::out);
			deleteSpecificLine(path, stu.id);
			ofs.close();
		}

		cout << setw(34) << "ɾ���ɹ�" << endl;

	}
	else
	{
		cout << setw(34) << "���޴���" << endl;
	}
}

void InsertStudent(NameTables& nametable)
{
	cout << "����ѧ�� ѧ�� ���� �Ա�" << endl;
	string id, name, gender;
	cin >> id >> name >> gender;

	while (!checkStu(id, name, gender))
	{
		cout << "����������" << endl;
		cout << "���� ѧ�� ���� �Ա�" << endl;
		cin >> id >> name >> gender;
	}
	

	Student stu = nametable.studentHashTable.find(id);
	if (stu.id == " ")
	{
		stu = Student(id, name, gender);
		nametable.studentHashTable.insert(stu);

		/*�����γ̱��ļ�*/
		ofstream ofs;
		string path = "dataBase\\StudentFiles\\" + id + "Schedule.txt";
		ofs.open(path, ios::out);
		ofs.close();

		/*��ѧ���ܱ����������*/
		ofstream namefile;
		namefile.open(StudentNameTable, ios::app);
		namefile << id << " " << name << " " << gender << endl;
		namefile.close();
	}

	else
	{
		cout << setw(34) << "�Ѿ��д���" << endl;
	}
}

void FindCurriculum(NameTables& nametable)
{
	cout << "������ҵĿγ̵Ŀκţ�" << endl;
	string id;
	cin >> id;

	while (!checkCriId(id))
	{
		cout << "����������" << endl;
		cin >> id;
	}

	nametable.curriculumHashTable.find(id);
}

void DeleteCurriculum(NameTables& nametable)
{
	cout << "����γ̿κ�" << endl;
	string s;
	cin >> s;

	while (!checkCriId(s))
	{
		cout << "����������" << endl;
		cin >> s;
	}

	Curriculum cri = nametable.curriculumHashTable.deleteItem(s);

	/*��ɾ���γ�ע���ѧ�������ļ�*/
	string temp_p = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
	const char* path = temp_p.c_str();
	remove(path);

	/*��ɾ���ܱ�γ�*/
	deleteSpecificLine(temp_p, cri.id);


	if (cri.id != " ")
	{
		for (StudentNode* ptr = cri.head->next; ptr != NULL; ptr = ptr->next)
		{
			string temp_id = ptr->id;
			Student stu = nametable.studentHashTable.find(temp_id);
			stu.deleteItem(temp_id);

			/*�γ�ע��������ɾ��ע���ѧ��*/
			ofstream ofs;
			string path = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
			ofs.open(path, ios::out);
			deleteSpecificLine(path, cri.id);
			ofs.close();
		}

	}
	else
	{
		cout << setw(34) << "���޴˿�" << endl;
	}
}

void InsertCurriclum(NameTables& nametable)
{
	cout << "����γ� �κ� ���� ѧ��" << endl;
	string id, name, credit;
	cin >> id >> name >> credit;

	while (!checkCri(id, name, credit))
	{
		cout << "����������" << endl;
		cout << "���� �κ� ���� ѧ��" << endl;
		cin >> id >> name >> credit;
	}

	Curriculum cri = nametable.curriculumHashTable.find(id);
	if (cri.id == " ")
	{
		cri = Curriculum(id, name, credit);
		nametable.curriculumHashTable.insert(cri);

		/*�����γ̱��ļ�*/
		ofstream ofs;
		string path = "dataBase\\CurriculumFiles\\" + id + "Register.txt";
		ofs.open(path, ios::out);
		ofs.close();

		/*�ڿγ��ܱ����������*/
		ofstream namefile;
		namefile.open(CurriculumNameTable, ios::app);
		namefile << id << " " << name << " " << credit << endl;
		namefile.close();
	}
	else
	{
		cout << setw(34) << "�Ѿ��д˿�" << endl;
	}

}

void StudentRegisterForCurriclum(NameTables& nametable)
{
	cout << "����ѧ�� ѧ�� ���� �Ա�male / female��" << endl;
	string id, name, gender;
	cin >> id >> name >> gender;

	while (!checkStu(id, name, gender))
	{
		cout << "����������" << endl;
		cout << "���� ѧ�� ���� �Ա�male / female��" << endl;
		cin >> id >> name >> gender;
	}

	Student stu = nametable.studentHashTable.find(id);
	if (stu.id != " ")
	{
		cout << "����γ̿κ�" << endl;
		string id;
		cin >> id;

		while (!checkCriId(id))
		{
			cout << "����������" << endl;
			cin >> id;
		}

		Curriculum cri = nametable.curriculumHashTable.find(id);
		if (cri.id == " ")
		{
			cout << setw(34) << "û�д˿�" << endl;
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
		cout << setw(34) << "���޴���" << endl;
	}
}

void StudentQuitCurriculum(NameTables& nametable)
{
	cout << "����ѧ��ѧ��" << endl;
	string s;
	cin >> s;

	while (!checkStuId(s))
	{
		cout << "����������" << endl;
		cin >> s;
	}

	Student stu = nametable.studentHashTable.find(s);
	if (stu.id != " ")
	{
		cout << "����γ̿κ�" << endl;
		string criId;
		cin >> criId;

		while (!checkCriId(criId))
		{
			cout << "����������" << endl;
			cin >> criId;
		}

		Curriculum cri = nametable.curriculumHashTable.find(criId);
		if (cri.id != " ")
		{
			stu.deleteItem(cri.id);
			cri.deleteItem(stu.id);

			/*�ļ�*/
			ofstream schedule;
			string path1 = "dataBase\\StudentFiles\\" + stu.id + "Schedule.txt";
			deleteSpecificLine(path1, cri.id);


			ofstream registers;
			string path2 = "dataBase\\CurriculumFiles\\" + cri.id + "Register.txt";
			deleteSpecificLine(path2, stu.id);
		}

		else
		{
			cout << setw(34) << "���޴˿�" << endl;
		}

	}
	else
	{
		cout << setw(34) << "���޴���" << endl;
	}
}

void CurriculumEliminateStudent(NameTables& nametable)
{
	cout << "����γ̿κ�" << endl;
	string s;
	cin >> s;

	while (!checkCriId(s))
	{
		cout << "����������" << endl;
		cin >> s;
	}


	Curriculum cri = nametable.curriculumHashTable.find(s);
	if (cri.id != " ")
	{
		cout << "����ѧ��ѧ��" << endl;
		string stuId;
		cin >> stuId;

		while (!checkStuId(stuId))
		{
			cout << "����������" << endl;
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
			cout << setw(34) << "���޴���" << endl;
		}

	}
	else
	{
		cout << setw(34) << "���޴˿�" << endl;
	}

}

void PrintRegesters(NameTables& nametable)
{
	cout << "����γ̿κţ���" << endl;
	string id;
	cin >> id;

	while (!checkCriId(id))
	{
		cout << "����������" << endl;
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
	cout << "����ѧ��ѧ�ţ�" << endl;
	string id;
	cin >> id;

	while (!checkStuId(id))
	{
		cout << "����������" << endl;
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
		cout << "�����빦����ţ�" << endl;
		cout << "=������=" << "==  ====  ====  ==" << "=�����=" << endl;
		cin >> choice;

		switch (choice)
		{
		case 1:/*��������ѧ����Ϣ*/
		{
			BatchImportStudentInfo(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 2:/*�������пγ���Ϣ*/
		{
			BatchImportCurriculumInfo(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 3:/*����ѧ����¼*/
		{
			FindStudent(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 4:/*ɾ��ѧ����¼*/
		{
			DeleteStudent(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 5:/*����ѧ����¼*/
		{
			InsertStudent(nametable);
			system("pause");
			system("cls");

			break;
		}

		case 6:/*���ҿγ̼�¼*/
		{
			FindCurriculum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 7:/*ɾ���γ̼�¼*/
		{
			DeleteCurriculum(nametable);
			system("pause");
			system("cls");
			break;

		}

		case 8:/*����γ̼�¼*/
		{
			InsertCurriclum(nametable);
			system("pause");
			system("cls");
			break;

		}

		case 9:/*ѧ��ע��γ�*/
		{
			StudentRegisterForCurriclum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 10: /*ѧ���˳��γ�*/
		{
			StudentQuitCurriculum(nametable);
			system("pause");
			system("cls");
			break;
		}

		case 11:/*�γ��˿�ѧ��*/
		{
			CurriculumEliminateStudent(nametable);
			system("pause");

			system("cls");
			break;
		}

		case 12:/*���ѡ������*/
		{
			PrintRegesters(nametable);
			system("pause");
			system("cls");
			break;

		}
		case 13:/*����γ��б�*/
		{
			PrintSchedules(nametable);
			system("pause");
			system("cls");
			break;

		}

		default:
		{
			cout << setw(34) << "ϵͳ�ѹر�" << endl;
			return 0;
			break;
		}


		}
		system("cls");
	}				   
	return 0;
	
}

