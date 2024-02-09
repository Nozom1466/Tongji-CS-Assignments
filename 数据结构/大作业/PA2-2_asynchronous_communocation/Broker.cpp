#include "Broker.h"
#include "ServerManager.h"

Broker::Broker(string name, int max)
{
	this->producerLock = false;
	this->consumerLock = false;
	this->name = name;
	this->maxCap = max;
	
}

bool Broker::receiveRequest()
{
	if (this->producerLock == false)
	{
		this->enqueue(ReSource().Request());
		cout << "                                                                       Request is Submitted!" << endl;
		return true;
	}
	return false;
}

Order Broker::popRequest()
{
	Order temp = this->getHead();
	this->dequeue();
	return temp;
}

bool Broker::invokeServer()
{
	return true;  //lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll
}

void Broker::_lockProducer()
{
	if (this->cnt == this->maxCap)
	{
		this->producerLock = true;
	}
	else
	{
		this->producerLock = false;
	}
}

void Broker::_lockConsumer()
{
	if (this->isEmpty())
	{
		this->consumerLock = true;
	}
	else
	{
		this->consumerLock = false;
	}

}

int Broker::returnMesseage()
{
	return 1;
}

void Broker::showInfo()
{
	if (this->isEmpty())
	{
		cout << "ERROR : Queue is empty." << endl;
	}
	else
	{
		
		for (Node<Order>* ptr = this->head; ptr != NULL; ptr = ptr->prev)
		{
			cout << "(" << ptr->item.seqN << ")" << '\t' << ptr->item.username << '\t' << ptr->item.request << endl;
		}
	}
}


int Broker::addServer()
{
	if (this->size() > 75 * this->maxCap / 100)
	{
		return (this->size() - this->maxCap * 75 / 100) / (this->maxCap * 25 / 100 / 5);
	}
	return 0;
}


