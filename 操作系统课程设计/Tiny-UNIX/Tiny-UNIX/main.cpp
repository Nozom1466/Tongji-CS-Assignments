#define _CRT_SECURE_NO_WORNINGS
#include <iomanip>
#include "Kernel.h"
#include "Params.h"

Kernel Kernel::inst;
Systemcall Systemcall::syscall;


int main() {
	Kernel* kernel = Kernel::getKernel();
	Systemcall* syscall = Systemcall::getSystemcallInst();
	kernel->init(syscall);
	
	while (true) {
		cout << "[Tiny-UNIX FileSystem" << kernel->curdir << "] $";

		// 实现不区分大小写#################################################
		char input[100000];
		cin.getline(input, 100000);
		vector<string> result = Toolkit::parseCmd(input);

		string op;
		if (result.size()) {
			op = result[0];
			Log::log("[INPUT] " + op + "\t#Param: " + to_string(result.size() - 1));

			if (result.size() - 1 != COMMAND::comList[op].numParam) {
				cout << "Operand Param Error ..." << endl;
				Log::log("[ERROR] Operand Param Error ...");
				continue;
			}

			if (op == "help") {
				cout << "----------------------UNIX-FileSystem-----------------------" << endl;
				cout << setiosflags(ios::left) 
					 << setw(10) << "COMMAND"	<< setw(16) << setiosflags(ios::left)
					 << setw(40) << "USAGE"		<< setw(40) << setiosflags(ios::left)
					 << "INSTRUCTION" << endl;
				for (auto& it : COMMAND::comList) {
					Comd com = it.second;
					cout << setw(10)  << com.name	<< setw(16) << setiosflags(ios::left)
						 << setw(40)  << com.usage	<< setw(40) << setiosflags(ios::left)
						 << com.instruction
						 << endl;
				}
				cout << "----------------------UNIX-FileSystem-----------------------" << endl;
			}
			else if (op == "fformat") {
				syscall->Sys_Format();
			}
			else if (op == "ls") {
				syscall->Sys_Ls();
			}
			else if (op == "mkdir") {
				syscall->Sys_Mkdir((char*)result[1].c_str(), 1);
			}
			else if (op == "fcreate") {
				syscall->Sys_Creat((char*)result[1].c_str(), 0x1FF, 1);
			}
			else if (op == "fopen") {
				syscall->Sys_OpenFile((char*)result[1].c_str(), 0x1FF, 1);
			}
			else if (op == "fclose") {
				syscall->Sys_CloseFile(atoi((char*)result[1].c_str()));
			}
			else if (op == "fread") {
				syscall->Sys_ReadFile(atoi((char*)result[1].c_str()), atoi((char*)result[2].c_str()), 1);
			}
			else if (op == "fwrite") {
				syscall->Sys_WriteFile(atoi((char*)result[1].c_str()), 
					                   (char*)result[3].c_str(),
									   atoi((char*)result[2].c_str()), 1);
			}
			else if (op == "flseek") {
				syscall->Sys_SeekFilePointer(atoi((char*)result[1].c_str()), 
											 atoi((char*)result[2].c_str()), 
											 atoi((char*)result[3].c_str()));
			}
			else if (op == "fdelete") {
				syscall->Sys_Delete((char*)result[1].c_str());
			}
			else if (op == "cd") {
				syscall->Sys_ChDir((char*)result[1].c_str());
			}
			else if (op == "cp") {
				syscall->Sys_Cp((char*)result[1].c_str(), (char*)result[2].c_str());
			}
			else if (op == "frename") {
				syscall->Sys_Rename((char*)result[1].c_str(), (char*)result[2].c_str());
			}
			else if (op == "ftree") {
				syscall->Sys_Tree((char*)result[1].c_str());
			}
			else if (op == "pwd") {
				syscall->Sys_Pwd();
			}
			else if (op == "scp") {
				syscall->Sys_Scp((char*)result[1].c_str(), (char*)result[2].c_str(), (char*)result[3].c_str());
			}
			else if (op == "login") {
				syscall->Sys_Login((char*)result[1].c_str());
			}
			else if (op == "logout") {
				cout << "Not Implemented Yet." << endl;
			}
			else if (op == "exit") {
				break;
			}
			else {
				cout << "Command not found!" << endl;
				Log::log("[ERROR] Command not found!");
			}
		}
	}
	Log::log("[INFO ] File System closed.");

	return 0;
}