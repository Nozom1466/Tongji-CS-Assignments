#include "Stack.hpp"
#include "StackData.h"
#include <iostream>
using namespace std;
const int N = 1e7 + 5;
long long address[N];

long long factorial(Stack<Data>& st, long long n)
{
	long long num = n;
	if (n < 0)
	{
		cout << "Factorial Range Error." << endl;
		return -1;
	}



	while (n > 1)
	{
		Data tmp;
		tmp.setter_num(n);
		tmp.setter_address(n);
		st.push(tmp);
		n--;
		
	}

	long long ans = 1;

	while (!st.isEmpty())
	{
		Data out = st.top();
		st.pop();
		ans *= out.getter_num();
		address[out.getter_address()] = ans;
	}

	return address[num];
}




int main()
{
	address[1] = 1;
	address[0] = 1;

	Stack<Data>* factorialStack = new Stack<Data>();
	long long n1;
	cout << "Enter the number to begin stack factorial process" << endl;
	cin >> n1;
	long long answer = factorial(*factorialStack, n1);


	cout << answer << endl;
	

	return 0;
}



/*
StackOverFlow : 4699


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


