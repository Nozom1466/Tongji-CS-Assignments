#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Toolkit {
public:

	static vector<string> parseCmd(char* raw);

	static void copy(int* from, int* to, int cnt);

	static void IOMove(char* from, char* to, int cnt);

	static int strLength(char* str);

	static void stringCopy(const char* src, char* dst);

	static struct tm* convertTime(int raw);
};