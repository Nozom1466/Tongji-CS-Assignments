#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;
const int N = 256;

//frequncy table
int cnt[N] = { 0 };

//Haffman tree Node
struct Node
{
	int weight;
	char data;
	int leftChild;
	int rightChild;
	int parent;
}tree[512];

//count for all leaf nodes
int ptr = 0;  

//mapping table
map<unsigned char, string> mp;
map<string, unsigned char> mp_decoding;

//decodeing reference
int cntRef = 0;
string encoded[N];

//content of compressed file
string fc = "";

//read file
void readFile(string path)
{
	FILE* fp;
	fp = fopen(path.c_str(), "rb");  //read the file in biary form

	unsigned char ch;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		
	
		//cout << ch  + 0<< " ";
		cnt[ch + 0]++;
	}
	cout << endl << endl;;

	fclose(fp);
}

//_Pred of tree[]
bool cmp(const Node& a, const Node& b) 
{
	return a.weight < b.weight;
}

//Haffman tree leaves
void placeTreeLeaves()
{
	
	for (int i = 0; i < N; i++)
	{
		if (cnt[i])
		{
			Node* nd = new Node;
			nd->data = (unsigned char)i;										//char(i);
			nd->leftChild = nd->rightChild = nd->parent = -1;
			nd->weight = cnt[i];
			tree[ptr++] = *nd;
		}
	}
	sort(tree, tree + ptr, cmp);
}

//select the two smallest position
pair<int, int> select(int l, int r)
{
	int min1 = -1;
	int min2 = -1;

	int min1_w = 0x7fffffff;
	int min2_w = 0x7fffffff;

	for (int i = l; i < r; i++)
	{
		if (tree[i].parent == -1)
		{
			if (tree[i].weight < min1_w)
			{
				//update the smallest weight
				min2_w = min1_w;
				min1_w = tree[i].weight;

				//update index
				min1 = min2;
				min2 = i;

			}

			else if (tree[i].weight < min2_w)
			{
				min2_w = tree[i].weight;
				min1 = i;
			}
		}
	}

	return { min1, min2 };
	//cout << min1 << " " << min2;
}

//Haffman encoding
void setUpHaffmanTree()
{
	int m = 2 * ptr - 1;
	for (int i = ptr; i < m; i++)
	{
		pair<int, int> childs = select(0, i);

		//info for non-leaf nodes
		tree[i].leftChild = childs.first;
		tree[i].rightChild = childs.second;
		tree[i].weight = tree[childs.first].weight + tree[childs.second].weight;
		tree[i].parent = -1;

		//info for leaf nodes
		tree[childs.first].parent = i;
		tree[childs.second].parent = i;
		
	}
}

//find a sequence of results of 2 squares
int fps(int a, int p)
{
	int ans = 1;
	while (p)
	{
		if (p & 1) ans *= a;
		a *= a;
		p >>= 1;
	}

	return ans;
}

//create reference table (map)
void encoding()
{
	for (int i = ptr - 1; i >= 0; i--)  //for (int i = ptr - 1; i >= 0; i--)
	{
		string s = "";
		unsigned char ch = tree[i].data;
		int child = i;
		while (tree[child].parent != -1)
		{
			int parent = tree[child].parent;
			if (tree[parent].leftChild == child)
			{
				s = '0' + s;
			}
			else if (tree[parent].rightChild == child)
			{
				s = '1' + s;
			}
			child = parent;
		}
		mp.insert({ ch, s });
		mp_decoding.insert({ s, ch });
		encoded[cntRef++] = s;
	}
}

//create encoded.txt file
void makeCompressedFile(string readPath, string tarPath)
{
	FILE *fout, *fin;
	fout = fopen(readPath.c_str(), "rb");
	fin = fopen(tarPath.c_str(), "w");

	//cout << endl << "Haffman encoding:" << endl;
	while (!feof(fout))
	{
		fputs(mp[fgetc(fout)].c_str(), fin);
		//cout << mp[fgetc(fout)];
	}

	fclose(fout);
	fclose(fin);
	cout << endl << "Successully compressed." << endl;
}

int redundent = 0;
//change the encoding to character
void encodingPress(string cmPath)
{
	string temp_path = "pro.txt";
	int temp = 0;

	FILE* fout, * fin;
	fout = fopen(cmPath.c_str(), "rb");
	fin = fopen(temp_path.c_str(), "w");

	unsigned char ch;
	int cnt = 0;
	int tot = 0;
	while (!feof(fout))
	{
		temp = (fgetc(fout) - '0') ? temp + fps(2, 7 - cnt) :temp;
		cnt++;

		if (cnt == 8)
		{
			fputc((unsigned char)temp, fin);
			fc += (unsigned char)temp;
			//cout << temp << endl;
			temp = 0;
			cnt = 0;
		}
		tot++;
	}
	//cout << "**********************************" << cnt << endl;

	if (cnt)
	{
		while (cnt != 8)
		{
			redundent++;
			cnt++;
			temp += fps(2, 8 - cnt);

		}
		
		fputc((unsigned char)temp, fin);
		fc += (unsigned char)temp;
		
	}
	//cout << "+++++++++++++++++++++" << redundent << endl;
	
	fclose(fout);
	fclose(fin);
}

//check if the encoding sequence is in the reference table
bool check(string s)
{
	for (int i = 0; i < cntRef; i++)
	{
		if (encoded[i] == s) return true;
	}
	return false;
}

//convert a char to its binary form string
string charTobChar(unsigned char ch)
{
	//cout << "************" << setbase(16) << ch + 0 << endl;
	string s = "00000000";
	
	for (int i = 7; i >= 0; i--)
	{
		if (ch & 1) s[i] = '1';
		ch >>= 1;
	}
	//cout << s << endl;
	return s;
}

//reshape encoding file
void makeEncodingFile(string pressed)
{
	//string fc;
	/*ifstream inputStream;
	stringstream strStream;


	inputStream.open(pressed);
	strStream << inputStream.rdbuf();
	inputStream.close();
	fc = strStream.str();

	cout << fc.size() << endl;*/

	/*FILE* fp;
	fp = fopen(pressed.c_str(), "r");

	int cnt = 0;
	while (!feof(fp))
	{
		cnt++;
		fc += fgetc(fp);
	}
	cout << "*********************" << cnt << endl;*/

	/*fstream fin;
	fin.open(pressed);
	istreambuf_iterator<char> beg(fin), end;

	string fc(beg, end);
	fin.close();*/

	//cout << fc << " *          ****************************" << endl;
	//cout << fc.size() << endl;

	fstream tar;
	tar.open("encoding.txt", ios::out);

	string temp = "";

	for (int i = 0; i < fc.size(); i++)
	{
		temp += charTobChar(fc[i]);

		//cout << charTobChar(fc[i]) << " ";
	}
	cout << endl;

	temp = temp.substr(0, temp.length() - redundent - 1);

	//cout << temp << endl;

	//cout << temp << " ************************" << endl;

	tar << temp;
	tar.close();


}

//Haffman decoding
void decoding(string readPath, string tarPath)
{
	FILE* fout, * fin;
	fout = fopen(readPath.c_str(), "rb");
	fin = fopen(tarPath.c_str(), "wb");

	string temp = "";
	while (!feof(fout))
	{
		temp += fgetc(fout);
		if (check(temp))
		{
			fputc(mp_decoding[temp], fin);
			temp = "";
		}

	}

	fclose(fout);
	fclose(fin);
}

//draw file extension from the given path
string drawExtension(string path)
{
	string temp = "";
	for (int i = path.size() - 1; i >= 0; i--)
	{
		if (path[i] == '.')
		{
			temp = '.' + temp;
			break;
		}
		else temp = path[i] + temp;

	}

	return temp;
}

int main()
{
	string path;
	cout << "Please enter the file to Compress:" << endl; /*test.txt*/
	cin >> path;

	readFile(path);

	placeTreeLeaves();


	setUpHaffmanTree();

	cout << endl;

	for (int i = 0; i < 2 * ptr - 1; i++)
	{
		cout << i << " " << tree[i].weight << " " << tree[i].parent << " " << tree[i].leftChild << " " << tree[i].rightChild << endl;
	}

	cout << endl;

	encoding();

	string cmPath = "encoded.txt";
	//cout << "Please enter the path of encoded file:" << endl; /*encoded.txt*/
	//cin >> cmPath;

	makeCompressedFile(path, cmPath);

	encodingPress(cmPath);

	for (int i = 0; i < ptr; i++)
	{
		cout << tree[i].data + 0 << " " << mp[tree[i].data] << endl;
	}

	makeEncodingFile("pro.txt");  //changes \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	
	int choice = 0;
	cout << "decode the compressed file?" << endl << "1 : YES" << endl << "0 : NO" << endl;
	cin >> choice;
	if (choice)
	{
		string decPath = "decoded" + drawExtension(path);
		decoding("encoding.txt", decPath);
	}

	return 0;
}


/*TEST CODES*/


//check Haffman tree
/*
for (int i = 0; i < 2 * ptr - 1; i++)
	{
		cout << i << " " << tree[i].weight << " " << tree[i].parent << " " << tree[i].leftChild << " " << tree[i].rightChild << endl;
	}

	cout << endl;


*/


//check encoding mapping
/*
for (int i = 0; i < ptr; i++)
	{
		cout << tree[i].data + 0 << " " << mp[tree[i].data] << endl;
	}

*/

//check reference table
/*
for (int i = 0; i < cntRef; i++)
	{
		cout << encoded[i] << endl;
	}


*/