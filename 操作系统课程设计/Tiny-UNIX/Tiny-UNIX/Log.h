#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#define LOG_PATH "./log.txt"

class Log {
public:

	static void log(string message);

};