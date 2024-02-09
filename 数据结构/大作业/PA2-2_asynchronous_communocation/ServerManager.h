#pragma once
#include "Server.h"
#include "Broker.h"






class ServerManager
{
public:
	ServerManager(int n=5);

	void connectBroker(Broker* b);

	int addsv();


	int totNum;
	Server servergroup[1000];
	Broker* bk;
	
};