#include "ServerManager.h"

ServerManager::ServerManager(int n)
{
	this->totNum = 0;
	for (int i = 1; i <= n; i++)
	{
		Server sv = Server();
		sv.seqNum = i;
		this->servergroup[totNum] = sv;
		
		totNum++;
	}
}

void ServerManager::connectBroker(Broker* b)
{
	this->bk = b;
}

int ServerManager::addsv()
{
	if (this->bk->addServer())
	{
		for (int i = 1; i <= this->bk->addServer(); i++)
		{
			totNum++;
			Server sv = Server();
			sv.seqNum = i;
			this->servergroup[totNum] = sv;
		}
	}

	return this->bk->addServer();
}