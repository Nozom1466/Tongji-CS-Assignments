#pragma once
#include <iomanip>
#include "Order.h"
#include "Broker.h"
#include "LinkQueue.hpp"

class Server
{
public:

	Server(bool iM = true, int seq = -1);
		
	bool handle();

	bool connectBroker(Broker* tpbk);

	bool fetch();

	bool work();
			
	
	bool idleMode;
	int seqNum;
	Broker* bk;
	Order temp;

};
