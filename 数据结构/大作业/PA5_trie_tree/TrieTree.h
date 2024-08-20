#pragma once
#include <iostream>
#include <algorithm>
#include <stdio.h>
using namespace std;


//Mode of Search()


class TrieTree
{
public:
	struct Node
	{
		char ch;
		bool end;
		string word = "";
		Node* next[30];		//均转换为26个字母，一共就只有最多26个孩子节点
		int idx;
		int freq;
		int vis;
		Node(char ch1=' ', bool ende = false, string worde = "", int index=0, int fre=0, int visited=0) : ch(ch1), end(ende), word(worde), idx(index), freq(fre), vis(visited) {
			for (int i = 0; i < 30; i++)
			{
				next[i] = NULL;
			}
		}

		
	};

	TrieTree();
	
	int Insert(string str);

	int Search(string str, int& f, int mode);

	int findWord(string str, int& f);

	int findPrefix(string prefix, int& f);

	class _Pred
	{
	public:
		bool operator()(pair<string, int> p1, pair<string, int> p2)
		{
			return p1.second > p2.second;
		}
	};

	void counter(Node* root);
	                               
	void deleter(Node* root);

	void preDestructor();

	~TrieTree();
	
	Node* root_;

	pair<string, int>* freqTable = new pair<string, int>[2000000];				//已知英语单词超过1000000个

	int num;                                                                          

};