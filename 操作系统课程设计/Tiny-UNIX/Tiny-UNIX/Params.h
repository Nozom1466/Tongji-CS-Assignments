#pragma once
#include <string>
#include <unordered_map>
using namespace std;


class Comd {
public:
	string name;
	string usage;
	string instruction;
	int numParam;
	Comd() : name(), usage(), instruction(), numParam() {}
	Comd(string name, string usage, string instruction, int numParam) : name(name), usage(usage), instruction(instruction), numParam(numParam) {}
};

class COMMAND {
public:
	static unordered_map<string, Comd> comList;
};

unordered_map<string, Comd> COMMAND::comList = {
	{"help",    Comd("help",    "help",								"Display overview of functionalities",  0)},
	{"fformat", Comd("fformat", "fformat",							"Format file volume",                   0)},
	{"ls",      Comd("ls",      "ls",								"List directory",                       0)},
	{"mkdir",   Comd("mkdir",   "mkdir <dirname>",					"Create directory",                     1)},
	{"fcreate", Comd("fcreate", "fcreate <filename>",				"Create file",                          1)},
	{"fopen",   Comd("fopen",   "fopen <filename>",					"Open file",                            1)},
	{"fclose",  Comd("fclose",  "fclose <fd>",						"Close file",                           1)},
	{"fread",   Comd("fread",   "fread <fd> <nbytes>",				"Read file",                            2)},
	{"fwrite",  Comd("fwrite",  "fwrite <fd> <nbytes> <string>",	"Write file",                           3)},
	{"flseek",  Comd("flseek",  "flseek <fd> <offset> <seekmode>",	"Move file pointer, seekmode: {0:set, 1:addcur, 2:addend, 3-5:512unit}",                    3)},
	{"fdelete", Comd("fdelete", "fdelete <filename>",				"Delete file",                          1)},
	{"cd",      Comd("cd",      "cd <dirname>",						"Change current directory",             1)},
	{"cp",      Comd("cp",      "cp <file1> <file2>",				"Copy file",                            2)},
	{"frename", Comd("frename", "frename <file1> <file2>",			"Rename file",                          2)},
	{"ftree",   Comd("ftree",   "ftree <dirname>",					"Display directory tree",               1)},
	{"pwd",     Comd("pwd",     "pwd",								"Display current directory",            0)},
	{"login",   Comd("login",   "login <username>",					"Login",                                1)},
	{"logout",  Comd("logout",  "logout <username>",				"Logout",                               1)},
	{"scp",		Comd("scp",		"scp [-u/-d] <from> <to>",			"Upload / Download file from file system",                                3)},
	{"exit",    Comd("exit",    "exit",								"Exit",                                 0)},
};
