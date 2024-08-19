#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <climits>
#include <tuple>
#include <unordered_map>
#include <sstream>

#include "BufferManager.h"
#include "FileManager.h"
#include "FileSystem.h"
#include "OpenFileManager.h"
#include "Toolkit.h"
#include "Log.h"
#include "Systemcall.h"


#define DISK_IMG "myDisk.img"
using namespace std;

class IOParameter {
public:
	char* m_Base;	/* 当前读、写用户目标区域的首地址 */
	int m_Offset;	/* 当前读、写文件的字节偏移量 */
	int m_Count;	/* 当前还剩余的读、写字节数量 */
};

class Kernel {
private:
	static Kernel inst;

	BufferManager* BufMgr;       /* 内核的高速缓存管理实例 */
	FileSystem* fileSys;         /* 内核的文件系统实例 */
	FileManager* fileMgr;        /* 内核的文件管理实例 */
	InodeTable* k_InodeTable;    /* 内核的内存Inode表 */
	OpenFileTable* s_openFiles;  /* 系统全局打开文件描述符表 */
	OpenFiles* k_openFiles;      /* 内核的打开文件描述符表 */
	SuperBlock* spb;              /* 全局的SuperBlock内存副本 */

	Kernel();
	
public:
	
	friend class Systemcall;

	~Kernel();

	// INSTANCES
	static Kernel* getKernel();					// Get kernel instance (Singleton Pattern)
	BufferManager* getBufferManager();
	FileSystem* getFileSystem();
	FileManager* getFileManager();
	InodeTable* getInodeTable();
	OpenFileTable* getOpenFileTable();
	OpenFiles* getKernelOpenFiles();
	SuperBlock* getSuperBlock();

	// ERROR CODE
	enum ERROR {
		NO_ERROR = 0,            /* 没有出错 */
		ISDIR = 1,               /* 操纵非数据文件 */
		NOTDIR = 2,              /* cd命令操纵数据文件 */
		NOENT = 3,               /* 文件不存在 */
		BADF = 4,                /* 文件标识fd错误 */
		NOOUTENT = 5,            /* 外部文件不存在 */
		NOSPACE = 6,             /* 磁盘空间不足 */
		NOACCESS = 7
	};

	// INITIALIZATION
	void init(Systemcall* syscall);								// Initialize kernel
	void initBuffer();							// Initialize buffer
	void initFileSystem();						// Initialize File system
	void refreshSystem();						// Refresh system before invoking system call

	// SYSTEMCALL ARGS
	int systemcallRet;							/* 记录调用函数的返回值 */   //u_ar0[User::EAX]
	ERROR errorCode;							/* 存放错误码 */
	char* pathname;								/* 目标路径名 */     //u_arg[0]
	int fd;										/* 记录文件标识 */     //u_arg[0]
	char* buf;									/* 指向读写的缓冲区 */   //u_arg[0]
	int nbytes;									/* 记录读写的字节数 */   //u_arg[1]
	int offset;									/* 记录Seek的读写指针位移 */   //u_arg[1]
	int mode;									/* 记录操纵文件的方式或seek的模式 */   //u_arg[2]

	// FILESYSTEM ARGS
	char* dirPointer;			   				/* 指向路径名的指针,用于nameI函数 */
	bool isformat = false;						/* 系统是否被初始化过 */

	Inode* cdir;								/* 指向当前目录的Inode指针 */
	Inode* pdir;								/* 指向当前目录父目录的Inode指针 */
	char curdir[128];							// Current firector
	
	char dbuf[DirectoryEntry::DIRSIZ];			/* 当前路径分量 */
	DirectoryEntry dent;						/* 当前目录的目录项 */
	IOParameter k_IOParam;						/* 记录当前读、写文件的偏移量，用户目标区域和剩余字节数参数 */
	bool isDir;									/* 当前操作是否针对目录文件 */
	bool isVerbose = true;						/* 是否需要输出 INFO */

	// MULTI USER
	short u_uid = 0;
	short u_gid = 0;
	bool is_login = false;
	unordered_map<string, tuple<string, short, short>> users;
};


