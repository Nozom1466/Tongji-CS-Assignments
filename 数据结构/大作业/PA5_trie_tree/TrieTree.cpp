#include "TrieTree.h"

TrieTree::TrieTree()
{
	this->root_ = new Node();
	this->num = 0;
}

int TrieTree::Insert(string str)
{
	Node* p = this->root_;
	
	//ת��ΪСд��ĸ
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= 65 && str[i] <= 90)
			str[i] += 32;
	}

	int found = 0;
	for (int i = 0; i < str.length(); i++)
	{ 
		found = 0;
		//*********************************************************************MODIFIED**************************************************************

		for (int j = 0; j < 26 ; j++)
		{
			if (p->next[j] != NULL && p->next[j]->ch == str[i])
			{
				//cout << p->next[j]->ch << endl;

				found = 1;
				p = p->next[j];
				break;
			}
		}
		//*********************************************************************MODIFIED**************************************************************


		if (!found)
		{
			Node* node = new Node(str[i], false, "", 0);
			if (node == NULL)
				return 0;
			//*********************************************************************MODIFIED**************************************************************
			
			p->next[str[i] - 'a'] = node;
			p->idx++;
			//p->next[p->idx++] = node;
			//cout << p->next[p->idx - 1]->ch << endl;
			//cout << p->word;
			//p = p->next[str[i] - 'a'];
			p = node;
			//*********************************************************************MODIFIED**************************************************************
			
		}

		
	}

	p->end = true;
	p->word = str;
	//cout << p->word << endl;
	p->freq++;
	return 1;
}

int TrieTree::Search(string str, int& f, int mode)
{
	//ת��ΪСд��ĸ
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= 65 && str[i] <= 90)
			str[i] += 32;
	}

	Node* p = this->root_;
	if (p == NULL)
		return 0;
	int found = 0;
	for (int i = 0; i < str.length(); i++)
	{
		found = 0;
		//*********************************************************************MODIFIED**************************************************************

		for (int j = 0; j < 26; j++)
		{
			if ( p->next[j] != NULL && p->next[j]->ch == str[i])
			{
				found = 1;
				p = p->next[j];
				break;
			}
		}

		//*********************************************************************MODIFIED**************************************************************

		if (!found)
		{
			//cout << i << endl;
			f = -1;
			return 0;
		}
	}

	//goo <-> google

	//cout << "-----------" << p->freq << endl;
	if (p->end == 0 && mode)
	{
		f = -1;
		return 0;
	}

	f = p->freq;
	//cout << "-----------" << outNode->freq << endl;

	return 1;


}

int TrieTree::findWord(string str, int& f)
{
	return this->Search(str, f, 1);
	
}

int TrieTree::findPrefix(string prefix, int& f)
{
	return this->Search(prefix, f, 0);
}

//��Ԫν��



void TrieTree::counter(Node* root)
{
	if (root->idx == 0)
	{
		this->freqTable[this->num++] = { root->word , root->freq };
		return;
	}

	if (root->end)
	{

		this->freqTable[this->num++] = { root->word , root->freq };
	}

	//*********************************************************************MODIFIED**************************************************************
	
	for (int i = 0; i < 26; i++)
	{
		//cout << i << endl;
		if (root->next[i] != NULL)
			this->counter(root->next[i]);
	}
	//*********************************************************************MODIFIED**************************************************************

	//*********************************************************************MODIFIED**************************************************************

	//sort(this->freqTable, this->freqTable + num, _Pred());
	//*********************************************************************MODIFIED**************************************************************

}

void TrieTree::deleter(Node* root)
{
	for (int i = 0; i < 26; i++)
	{
		if (root->next[i] != NULL)
			deleter(root->next[i]);
	}
	delete root;
	
}

void TrieTree::preDestructor()
{
	//�������ɾ���ڵ㣬ɾ���洢�б�
	this->deleter(this->root_);
	delete[] this->freqTable;

}

TrieTree::~TrieTree()
{
	this->preDestructor();
	
}

