#include <iostream>
#include "Broker.h"
#include "Order.h"
#include "RequestSource.h"
#include "Server.h"
#include "ServerManager.h"
#include <ctime>
#include <windows.h>
using namespace std;



int main()
{
	srand(time(nullptr));
	
	//初始化缓冲队列、需求源
	Broker broker = Broker();
	ReSource source = ReSource();
	

	//创建服务器管理器
	int numServer;
	cout << "Enter the number of Server" << endl;
	cin >> numServer;
	ServerManager svg = ServerManager(numServer);  //利用服务器管理器船舰服务器
	svg.connectBroker(&broker);

	
	//将服务器与缓冲队列连接
	for (int i = 0; i < svg.totNum; i++)
	{
		svg.servergroup[i].connectBroker(&broker);
		//cout << svg.servergroup[i].bk->name << endl;
	}


	int choice = 0;
	cout << "Enter your choice:" << endl << "0: quit " << endl << "1: simulation" << endl;
	cin >> choice;
	if (choice == 1)
	{
		while (1)
		{
			//模拟需求进入
			for (int i = 1; i <= rand() % numServer; i++)
			{
				if (!broker.receiveRequest())
				{
					break;
				}

				broker._lockProducer();
			}

			//当需求达到缓冲队列的一定比例时，添加一些服务器应对洪峰
			int added = svg.addsv();
			numServer += added;

			//将添加的服务器与缓冲队列进行连接
			for (int i = 0; i < svg.totNum; i++)
			{
				svg.servergroup[i].connectBroker(&broker);
				//cout << svg.servergroup[i].bk->name << endl;
			}


			//显示当前缓冲队列中的需求
			broker.showInfo();
			cout << endl;


			//更新生产者锁和消费者锁
			broker._lockConsumer();
			broker._lockProducer();

			
			//模拟Server处理需求
			for (int i = 1; i <= rand()% numServer; i++)
			{
				int num = rand() % numServer;
				svg.servergroup[num].work();

				//更新锁
				broker._lockConsumer();
				broker._lockProducer();
				Sleep(400);

				//cout << broker.producerLock << broker.consumerLock<< endl;
			}
			cout << endl;
			
		}
	}
	
	


	return 0;
}


