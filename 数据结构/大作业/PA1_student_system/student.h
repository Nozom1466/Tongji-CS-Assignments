#pragma once
#include <iostream>
#include "abstract.h"
//#include "curriculum.h"
#include <iomanip>
using namespace std;

class StudentNode;
class CurriculumNode;
class Student : public abstract
{
public:
	Student();
	Student(string id, string name, string gender);

	int deleteItem(string s);

	int addItem(string s);

	int find(string s);


	CurriculumNode* head;
	string gender;
};

class StudentNode
{
public:
	string id;
	StudentNode* next; 
};

