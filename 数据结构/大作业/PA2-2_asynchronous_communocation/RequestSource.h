#pragma once
#include "Order.h"
#include <ctime>
#include <string>
#include <iostream>
using namespace std;


class ReSource
{
public:
	ReSource();

	
	string randStr();
	
	string randRequest();

	Order Request();

	string orderList[4] = { "pay", "collect", "browse", "search" };
	
};