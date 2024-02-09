#pragma once
#include "AbstractStack.h"
#include <iostream>
#include <stdio.h>
using namespace std;


template<class dataType>
struct node
{
	node();
	node(const dataType& d, node<dataType>* n = NULL)
	{
		data = d;
		next = n;
	}
	dataType data;
	node<dataType>* next;

};


template<class dataType>
class Stack : public MyStack<dataType>
{
public:
	Stack()
	{
		top_p = NULL;
		cout << "New LinkedStack Created!" << endl;
	}

	bool isEmpty()
	{
		return top_p == NULL;
	}

	void push(const dataType& d)
	{
		node<dataType>* add = new node<dataType>(d, top_p);
		top_p = add;
		cout << "Push data successfully" << endl;
	}

	dataType pop()
	{
		if (isEmpty())
		{
			throw 0;
		}

		dataType d = top_p->data;
		node<dataType>* tmp = top_p;
		top_p = top_p->next;
		delete tmp;
		return d;
	}

	dataType top()
	{
		if (isEmpty())
		{
			throw 0;
		}

		return top_p->data;
	}

	~Stack()
	{
		while (top_p != NULL)
		{
			node<dataType>* tmp = top_p;
			top_p = top_p->next;
			delete tmp;
		}
	}

	
	node<dataType>* top_p;

};




//StackOverFlow: 4699




/*
#include <iostream>
using namespace std;

long long factorial(long long n)
{
	if (n == 1) return 1;

	return factorial(n - 1) * n;
}




int main()
{
	int aout = 1;
	for (int i = 1; ; i++)
	{
		aout = i;
		try
		{
			cout << factorial(i) << " " << i << endl;
		}

		catch(...)
		{
			cout << "Error" << endl;
			cout << i << endl;
			return 0;
		}
	}




	return 0;
}


*/