#include "Toolkit.h"


vector<string> Toolkit::parseCmd(char* raw) {
	vector<string> res;
	char *l = raw, *r = raw;
	while (*r != '\0') {
		if (*l == ' ') {
			l++; r++;
		}
		else {
			while (*r != '\0' && *r != ' ') {
				r++;
			}
			res.push_back(string(l, r - l));
			l = r;
		}
	}
	return res;
}

void Toolkit::copy(int* from, int* to, int cnt) {
	while (cnt--) {
		*to++ = *from++;
	}
}

void Toolkit::IOMove(char* from, char* to, int cnt) {
	while (cnt--) {
		*to++ = *from++;
	}
}

int Toolkit::strLength(char* str) {
	int length = 0;
	char* pChar = str;

	while (*pChar++) {
		length++;
	}

	/* ·µ»Ø×Ö·û´®³¤¶È */
	return length;
}

void Toolkit::stringCopy(const char* src, char* dst) {
	while ((*dst++ = *src++) != 0);
}

struct tm* Toolkit::convertTime(int raw)
{
	time_t tt = raw;
	struct tm* ttime = localtime(&tt);
	return ttime;
}
