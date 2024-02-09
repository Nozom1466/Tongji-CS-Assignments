#include "Server.h"

Server::Server(bool iM, int seq)
{
	this->idleMode = iM;
	this->seqNum = seq;

}

bool Server::handle()
{
	cout << "NO." << setw(5) << this->seqNum << "Server is working on request:"
		<< setw(15) << "(" << this->temp.seqN << ")" << this->temp.request << endl;

	return true;
}

bool Server::connectBroker(Broker* tpbk)
{
	this->bk = tpbk;
	return true;
}

bool Server::fetch()
{
	if (this->bk->consumerLock == false)
	{
		this->temp = this->bk->popRequest();
		return true;
	}
	return false;
	
}

bool Server::work()
{
	if (this->fetch())
	{
		this->handle();
		return true;
	}

	return false;
	
}


