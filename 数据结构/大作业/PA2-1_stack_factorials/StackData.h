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
	long long n;  //函数的输入参数
	long long returnAddress;  //函数的返回地址  //需要，这样小于等于n的阶乘都可以找到

	
}Data;


ostream& operator<<(ostream& cout, Data& data)
{
	cout << data.getter_num() << endl;
	return cout;
}