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
	
	//��ʼ��������С�����Դ
	Broker broker = Broker();
	ReSource source = ReSource();
	

	//����������������
	int numServer;
	cout << "Enter the number of Server" << endl;
	cin >> numServer;
	ServerManager svg = ServerManager(numServer);  //���÷���������������������
	svg.connectBroker(&broker);

	
	//���������뻺���������
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
			//ģ���������
			for (int i = 1; i <= rand() % numServer; i++)
			{
				if (!broker.receiveRequest())
				{
					break;
				}

				broker._lockProducer();
			}

			//������ﵽ������е�һ������ʱ�����һЩ������Ӧ�Ժ��
			int added = svg.addsv();
			numServer += added;

			//����ӵķ������뻺����н�������
			for (int i = 0; i < svg.totNum; i++)
			{
				svg.servergroup[i].connectBroker(&broker);
				//cout << svg.servergroup[i].bk->name << endl;
			}


			//��ʾ��ǰ��������е�����
			broker.showInfo();
			cout << endl;


			//����������������������
			broker._lockConsumer();
			broker._lockProducer();

			
			//ģ��Server��������
			for (int i = 1; i <= rand()% numServer; i++)
			{
				int num = rand() % numServer;
				svg.servergroup[num].work();

				//������
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


