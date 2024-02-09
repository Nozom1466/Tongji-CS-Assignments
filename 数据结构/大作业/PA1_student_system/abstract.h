#pragma once
#include <iostream>
#include <string>
using namespace std;


class abstract
{
public:
	virtual int deleteItem(string s) = 0;

	virtual int addItem(string s) = 0;

	virtual int find(string s) = 0;
	string name;

	string id = " ";

};
