#pragma once
#include <iostream>
#include <string>
using namespace std;


class Order
{
public:
	Order(string name = "unknown", string req = "none", int time = 0, int seq=0)
	{
		this->username = name;
		this->request = req;
		this->waitingTime = time;
		this->seqN = seq;
	}

	

	string username;
	string request;
	int waitingTime;
	int seqN;
};

//ostream& operator<< (ostream& cout, Order& od)
//{
//	cout <<"(" << od.seqN << ")" << od.username << " " << od.request << endl;
//	return cout;
//}