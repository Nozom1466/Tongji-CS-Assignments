#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;



typedef struct Data
{
public:
	long long getter_num()
	{
		return n;
	}

	long long getter_address()
	{
		return returnAddress;
	}

	void setter_num(long long num)
	{
		n = num;
	}
	void setter_address(long long add)
	{
		returnAddress = add;
	}

private:
	long long n;  //�������������
	long long returnAddress;  //�����ķ��ص�ַ  //��Ҫ������С�ڵ���n�Ľ׳˶������ҵ�

	
}Data;


ostream& operator<<(ostream& cout, Data& data)
{
	cout << data.getter_num() << endl;
	return cout;
}