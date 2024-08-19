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
	char* m_Base;	/* ��ǰ����д�û�Ŀ��������׵�ַ */
	int m_Offset;	/* ��ǰ����д�ļ����ֽ�ƫ���� */
	int m_Count;	/* ��ǰ��ʣ��Ķ���д�ֽ����� */
};

class Kernel {
private:
	static Kernel inst;

	BufferManager* BufMgr;       /* �ں˵ĸ��ٻ������ʵ�� */
	FileSystem* fileSys;         /* �ں˵��ļ�ϵͳʵ�� */
	FileManager* fileMgr;        /* �ں˵��ļ�����ʵ�� */
	InodeTable* k_InodeTable;    /* �ں˵��ڴ�Inode�� */
	OpenFileTable* s_openFiles;  /* ϵͳȫ�ִ��ļ��������� */
	OpenFiles* k_openFiles;      /* �ں˵Ĵ��ļ��������� */
	SuperBlock* spb;              /* ȫ�ֵ�SuperBlock�ڴ渱�� */

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
		NO_ERROR = 0,            /* û�г��� */
		ISDIR = 1,               /* ���ݷ������ļ� */
		NOTDIR = 2,              /* cd������������ļ� */
		NOENT = 3,               /* �ļ������� */
		BADF = 4,                /* �ļ���ʶfd���� */
		NOOUTENT = 5,            /* �ⲿ�ļ������� */
		NOSPACE = 6,             /* ���̿ռ䲻�� */
		NOACCESS = 7
	};

	// INITIALIZATION
	void init(Systemcall* syscall);								// Initialize kernel
	void initBuffer();							// Initialize buffer
	void initFileSystem();						// Initialize File system
	void refreshSystem();						// Refresh system before invoking system call

	// SYSTEMCALL ARGS
	int systemcallRet;							/* ��¼���ú����ķ���ֵ */   //u_ar0[User::EAX]
	ERROR errorCode;							/* ��Ŵ����� */
	char* pathname;								/* Ŀ��·���� */     //u_arg[0]
	int fd;										/* ��¼�ļ���ʶ */     //u_arg[0]
	char* buf;									/* ָ���д�Ļ����� */   //u_arg[0]
	int nbytes;									/* ��¼��д���ֽ��� */   //u_arg[1]
	int offset;									/* ��¼Seek�Ķ�дָ��λ�� */   //u_arg[1]
	int mode;									/* ��¼�����ļ��ķ�ʽ��seek��ģʽ */   //u_arg[2]

	// FILESYSTEM ARGS
	char* dirPointer;			   				/* ָ��·������ָ��,����nameI���� */
	bool isformat = false;						/* ϵͳ�Ƿ񱻳�ʼ���� */

	Inode* cdir;								/* ָ��ǰĿ¼��Inodeָ�� */
	Inode* pdir;								/* ָ��ǰĿ¼��Ŀ¼��Inodeָ�� */
	char curdir[128];							// Current firector
	
	char dbuf[DirectoryEntry::DIRSIZ];			/* ��ǰ·������ */
	DirectoryEntry dent;						/* ��ǰĿ¼��Ŀ¼�� */
	IOParameter k_IOParam;						/* ��¼��ǰ����д�ļ���ƫ�������û�Ŀ�������ʣ���ֽ������� */
	bool isDir;									/* ��ǰ�����Ƿ����Ŀ¼�ļ� */
	bool isVerbose = true;						/* �Ƿ���Ҫ��� INFO */

	// MULTI USER
	short u_uid = 0;
	short u_gid = 0;
	bool is_login = false;
	unordered_map<string, tuple<string, short, short>> users;
};


