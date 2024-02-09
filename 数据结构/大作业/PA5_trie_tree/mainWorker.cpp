#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>

#include "TrieTree.h"
#include "cmd_console_tools.h"
using namespace std;

/*
1. 对于命令行的删除还是有一点问题，主要是删除的时候会把命令行提示符>>删除一个
2. 命令行加载一个之后再加载一个会出现删除不掉上一个TrieTree的问题，会发生覆盖
3. 最后一个数据结构大作业，这学期太tm恶心了
*/



string commandSet[] = { "load", "dic", "stat", "sort"};			

int readFile(string path, string& out)
{
	ifstream readFrom;
	readFrom.open(path, ios::in);
	if (readFrom.is_open() == 0)
		return 0;
	string temp = "";
	while (readFrom.good())
	{
		getline(readFrom, temp);
		//cout << temp.size() << endl;
		if (temp.length() && temp[temp.length() - 1] == '-')
			temp = temp.substr(0, temp.length() - 1);		//直接将每一行末尾的连字符去掉，对行中间的连字符保留
		if (temp.length())
			out += temp;
	}
	readFrom.close();
	return 1;

}

int buildTree(string src, TrieTree* tree)
{
	cout << endl << endl;
	for (int i = 0; i < src.length(); i++)
	{
		string temp = "";

		while (i < src.length() && (src[i] >= 'A' && src[i] <= 'Z' || src[i] >= 'a' && src[i] <= 'z'))  // || src[i] == '-'  //连字符字典序遍历就失效了
		{
			
			temp += src[i++];
		}

		if (temp.size())
		{
			tree->Insert(temp);
		}
	}

	return 1;
}

int command(string& ope)
{
	cout << ">>>>";
	char ch = ' ';
	if ((ch = _getch()) == '\n')
		return -1;					
	else
	{
		ope += ch;
		cout << ch;
	}

	while (1)
	{

		ch = _getch();
		//cout << ch + 0 << endl;
		if (ch == 13)
		{
			cout << endl;
			return 1;
		}
		
		int x = 0, y = 0;
		cct_getxy(x, y);
		if (ch == 8 && ope.length() && x >= 4)
		{
			ope = ope.substr(0, ope.length() - 1);
			
			cct_gotoxy(x - 1, y);
			cout << " ";
			cct_gotoxy(x - 1, y);
			continue;
		}
		else if (ch == 8 && x < 4)
		{
			cct_gotoxy(x + 1, y);
		}
			
		cout << ch;
		ope += ch;
	}
	
}

int convert(string com)
{
	string out = "";
	for (int i = 0; i < com.size(); i++)
	{
		while (com[i] >= 65 && com[i] <= 90 || com[i] >= 97 && com[i] <= 122)
		{
			if (com[i] >= 65 && com[i] <= 90)
				com[i] += 32;
			out += com[i++];
		}

		if (out == commandSet[0])
			return i;
		else if (out == commandSet[1])
			return -2;
		else if (out == commandSet[2])
			return -3;
		else if (out == commandSet[3])
			return -4;
	}

	return -1;

}


int main(int argc, char*argv[])
{
	cout << ">> " << argv[0] << endl;
	cout << ">> " << "usage:" << endl;
	cout << setiosflags(ios::left);
	cout << "----------------------------------------------------------------------------" << endl;
	cout << ">> " << setw(10) << "load" <<  ": 加载" << endl;
	cout << ">> " << setw(10) << "dic"  << ": 单词表" << endl;
	cout << ">> " << setw(10) << "stat"  << ": 字典序查看频率" << endl;
	cout << ">> " << setw(10) << "sort"  << ": 按单词频率排序" << endl;
	cout << resetiosflags(ios::left);






	//字典树初始化
	TrieTree* tree = new TrieTree();


	//文件内容
	string res = "";
	
	while (1)
	{
		string ope = "";
		command(ope);
		if (ope.size() == 1 && (ope[0] == 'q' || ope[0] == 'Q'))
		{
			cout << ">>>> ";
			return 0;

		}
		
		int temp = convert(ope);
		if (temp >= 0)
		{
			tree->preDestructor();
			tree = new TrieTree();

			string path = "";
			for (int j = temp + 1; j < ope.size(); j++)
			{
				if (ope[j] == ' ')
					break;
				path += ope[j];
			}

			
			
			if (readFile(path, res))
			{
				//建树
				buildTree(res, tree);
				//排序
				
				tree->counter(tree->root_);
				
				cout << "成功加载" << endl;

			}
			else
			{
				cout << "文件打开失败" << endl;
				continue;
			}

		}
		else if (temp == -1)
		{
			continue;
		}
		else if (temp == -2)
		{
			
			cout << setiosflags(ios::left) << endl;
			for (int i = 0; i < tree->num; i++)
			{
				cout << setw(20) << i + 1 << tree->freqTable[i].first << endl;
			}
			cout << resetiosflags(ios::left);
		}
		else if (temp == -3)
		{
			cout << setiosflags(ios::left);
			cout << setw(25) << "WORD" << "|" << setw(25) << "FREQUNCY" << "|PAIR" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			
			for (int i = 0; i < tree->num; i++)
			{
				cout << setw(25) << tree->freqTable[i].first << "|" << setw(25) << tree->freqTable[i].second << 
				"|<" << tree->freqTable[i].first << "," << tree->freqTable[i].second << ">" << endl;
			}
			cout << resetiosflags(ios::left);

		}
		else if (temp == -4)
		{
			cout << setiosflags(ios::left);
			cout << setw(25) << "WORD" << "|" << setw(25) << "FREQUNCY" << "|PAIR" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			pair<string, int>* sorter = new pair<string, int>[2000000];
			for (int i = 0; i < tree->num; i++)
			{
				sorter[i] = tree->freqTable[i];
			}
			sort(sorter, sorter + tree->num, TrieTree::_Pred());

			for (int i = 0; i < tree->num; i++)
			{
				
				cout << setw(25) << sorter[i].first << "|" << setw(25) << sorter[i].second <<
					"|<" << sorter[i].first << "," << sorter[i].second << ">" << endl;
			}
			cout << resetiosflags(ios::left);
			delete[] sorter;
			
		}

	}
	return 0;
}

