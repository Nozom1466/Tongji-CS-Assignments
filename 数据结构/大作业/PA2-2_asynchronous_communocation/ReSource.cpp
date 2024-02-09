#include "RequestSource.h"

ReSource::ReSource()
{
	
}


string ReSource::randStr()
{
	string temp = "";
	temp += (char)(rand() % 26 + 'A');
	for (int i = 1; i <= 4; i++)
	{
		temp += (char)(rand() % 26 + 'a');
	}

	return temp;
}

string ReSource::randRequest()
{
	return this->orderList[rand() % 4];
}



Order ReSource::Request()
{
	return Order(this->randStr(), randRequest(), 0, rand()%1000);

}

