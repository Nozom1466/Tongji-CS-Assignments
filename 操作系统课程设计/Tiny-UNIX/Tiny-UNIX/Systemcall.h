#pragma once
#include "Kernel.h"

class Systemcall {
private:
	static Systemcall syscall;
public:
	friend class Kernel;

	Systemcall();
	~Systemcall();

	static Systemcall* getSystemcallInst();

	int Sys_ChDir(char* pathname);

	int Sys_ChDir1(char* pathname);

	int Sys_Creat(char* pathname, int mode, int verbose);

	int Sys_Pwd();

	int Sys_Mkdir(char* pathname, int verbose);

	int Sys_Rename(char* from, char* to);

	int Sys_Close(int fd);

	int Sys_Ls();

	int Sys_Delete(char* pathname);

	int Sys_Format();

	int Sys_OpenFile(char* pathname, int mode, int verbose);

	int Sys_CloseFile(int fd);

	int Sys_WriteFile(int writeFd, char* buf, int nbytes, int verbose);

	int Sys_ReadFile(int readFd, int nbytes, int verbose);

	int Sys_SeekFilePointer(int seekFd, int offset, int SeekMode);

	int Sys_Cp(char* from, char* to);

	int Sys_Tree(char* path);

	int Sys_Scp(char* mode, char* from, char* to);

	int Sys_Login(char* username);

	int Sys_Logout(char* username);

};