#pragma once
#include "LinkQueue.hpp"
#include "Order.h"
#include "RequestSource.h"


class Broker : public LinkQueue<Order>
{

public:
	Broker(string name="local", int max=500);

	bool receiveRequest();

	Order popRequest();
	
	bool invokeServer();
	
	void _lockProducer();
	
	void _lockConsumer();

	int returnMesseage();

	void showInfo();

	int addServer();
	

	bool producerLock;
	bool consumerLock;
	int maxCap;
	string name;

};