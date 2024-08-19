#include "Log.h"

void Log::log(string message) {
	ofstream outfile;
	outfile.open(LOG_PATH, ios::app);
	outfile << message << endl;
	outfile.close();
}