#pragma once
#include <iostream>
#include <iomanip>
#include "abstract.h"
//#include "student.h"
using namespace std;
class CurriculumNode;
class StudentNode;
class Curriculum : public abstract
{
public:
	Curriculum();

	Curriculum(string id, string name, string credit);

	int deleteItem(string s);

	int addItem(string s);

	int  find(string s);

	StudentNode* head;
	string credit;
};

class CurriculumNode
{
public:
	string id;
	CurriculumNode* next;
};