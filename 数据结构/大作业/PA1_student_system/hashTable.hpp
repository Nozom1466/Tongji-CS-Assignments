#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include "GlobalFile.h"
#include <cstdlib>
#include <iomanip>
//#include <fstream>
using namespace std;



template<class T>
class HashTable
{
public:
	HashTable();

	HashTable(int N)  //��ϣ����������
	{
		this->N = N;
		//this->idx = 0;
		/*����*/
		for (int i = 0; i < N; i++)
		{
			h[i] = -1;
			ne[i] = 0;
			/*e[i]�ĳ�ʼ��*/
			T ori;
			e[i] = ori;
		}

	}

	int BKDRHash(string s)
	{
		unsigned int seed = 31;
		unsigned int key = 0;

		for (int i = 0; i < s.size(); i++)
		{
			key = key * seed + s[i];
		}

		return (key & 0x7fffffff) % N;

	}

	int insert(T x)
	{
		int k = BKDRHash(x.id);
		e[idx] = x;
		ne[idx] = h[k];
		h[k] = idx++;


		cout << setw(34) << "��ɲ���" << endl;
		return 1;
	}

	T find(string id)
	{
		int k = BKDRHash(id);
		for (int i = h[k]; i != -1; i = ne[i])
		{
			if (e[i].id == id)
			{
				cout << e[i].name << " " << e[i].id << endl;
				return e[i];
			}
		}
		cout << setw(34) << "���޴���" << endl;
		T temp = T();
		return temp;
	}

	/*���������Ա�ģ��Ļ���ɾ��ʱ�������������Ա������ºܶ�����*/
	T deleteItem(string id)
	{
		int k = BKDRHash(id);
		int pos = h[k];
		for (int i = h[k]; i != -1; i = ne[i])
		{
			if (e[i].id == id)
			{
				ne[pos] = ne[i];
				if (i == h[k]) h[k] = -1;
				return e[i];
			}
			pos = i;
		}
		T temp = T();
		return temp;
	}


	//int h[1e5], ne[1e5], idx;

	int N;
	/*int idx; */
	//int* h = (int*)malloc(N * sizeof(int));
	int idx = 0;
	T* e = new T[1e5];
	int* h = new int[1e5];
	int* ne = new int[1e5];


	
	//T* e = (T*)malloc(1e5 * sizeof(T));
	/*int* ne = (int*)malloc(N * sizeof(int)); */

};
