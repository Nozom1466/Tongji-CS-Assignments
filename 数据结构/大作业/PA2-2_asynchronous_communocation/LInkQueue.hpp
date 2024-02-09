#pragma once
#include <iostream>
using namespace std;


template<class T>
struct Node
{
	T item;
	Node* next;
	Node* prev;
};


template<class T>
class LinkQueue
{
public:
	Node<T>* head;
	Node<T>* rear;
	unsigned int cnt;  //计数为非负

	LinkQueue()
	{
		cnt = 0;
		Node<T>* anode = new Node<T>;
		anode->next = NULL;
		anode->prev = NULL;
		//声明两个节点
		head = anode;
		rear = anode;
	}

	~LinkQueue()
	{
		Node<T>* p = head;
		for (int i = 0; i < cnt; i++)
		{
			dequeue();
		}
	}

	void dequeue()
	{
		if (isEmpty())
		{
			cout << "ERROR : Queue is Empty." << endl;
		}

		else if (cnt == 1)
		{
			cnt--;
		}

		else
		{
			Node<T>* p = head;
			head = p->prev;
			delete p;
			cnt--;
		}
	}

	void enqueue(T item)
	{
		if (cnt == 0)
		{
			rear->item = item;
			cnt++;
		}

		else
		{
			//这个链表是往左边延申的
			Node<T>* anode = new Node<T>;
			anode->item = item;
			anode->next = rear;
			rear->prev = anode;
			rear = anode;
			rear->prev = NULL;
			cnt++;
		}
	}

	unsigned int size()
	{
		return cnt;
	}

	bool isEmpty()
	{
		return cnt == 0;
	}

	T getHead()
	{
		return head->item;
	}

	void showInfo()
	{
		if (isEmpty())
		{
			cout << "ERROR : Queue is empty." << endl;
		}
		else
		{
			for (Node<T>* ptr = head; ptr != NULL; ptr = ptr->prev)
			{
				cout << ptr->item << endl;
			}
		}
	}

};

