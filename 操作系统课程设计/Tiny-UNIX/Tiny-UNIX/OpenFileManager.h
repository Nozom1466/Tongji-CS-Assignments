#pragma once
#ifndef OPEN_FILE_MANAGER_H
#define OPEN_FILE_MANAGER_H

#include "INode.h"
//#include "File.h"
#include "FileSystem.h"


/* Forward Declaration */
class OpenFileTable;
class InodeTable;

/* ����2������ʵ��������OpenFileManager.cpp�ļ��� */
//extern InodeTable g_InodeTable;
//extern OpenFileTable g_OpenFileTable;

class File
{
public:
	/* Enumerate */
	enum FileFlags
	{
		FREAD = 0x1,			/* ���������� */
		FWRITE = 0x2,			/* д�������� */
		FPIPE = 0x4				/* �ܵ����� */
	};

	/* Functions */
public:
	/* Constructors */
	File();
	/* Destructors */
	~File();


	/* Member */
	unsigned int f_flag;		/* �Դ��ļ��Ķ���д����Ҫ�� */
	int		f_count;			/* ��ǰ���ø��ļ����ƿ�Ľ������� */
	Inode* f_inode;			/* ָ����ļ����ڴ�Inodeָ�� */
	int		f_offset;			/* �ļ���дλ��ָ�� */
};

/*
 * ���ļ�������(OpenFileManager)����
 * �ں��жԴ��ļ������Ĺ���Ϊ����
 * ���ļ������ں����ݽṹ֮��Ĺ���
 * ��ϵ��
 * ������ϵָ����u���д��ļ�������ָ��
 * ���ļ����е�File���ļ����ƽṹ��
 * �Լ���File�ṹָ���ļ���Ӧ���ڴ�Inode��
 */
class OpenFileTable
{
public:
	/* static consts */
	//static const int NINODE	= 100;	/* �ڴ�Inode������ */
	static const int NFILE = 100;	/* ���ļ����ƿ�File�ṹ������ */

	/* Functions */
public:
	/* Constructors */
	OpenFileTable();
	/* Destructors */
	~OpenFileTable();

	// /* 
	 // * @comment �����û�ϵͳ�����ṩ���ļ�����������fd��
	 // * �ҵ���Ӧ�Ĵ��ļ����ƿ�File�ṹ
	 // */
	// File* GetF(int fd);
	/*
	 * @comment ��ϵͳ���ļ����з���һ�����е�File�ṹ
	 */
	File* FAlloc();
	/*
	 * @comment �Դ��ļ����ƿ�File�ṹ�����ü���f_count��1��
	 * �����ü���f_countΪ0�����ͷ�File�ṹ��
	 */
	void CloseF(File* pFile);

	/* Members */
public:
	File m_File[NFILE];			/* ϵͳ���ļ���Ϊ���н��̹������̴��ļ���������
								�а���ָ����ļ����ж�ӦFile�ṹ��ָ�롣*/
};

/*
 * �ڴ�Inode��(class InodeTable)
 * �����ڴ�Inode�ķ�����ͷš�
 */
class InodeTable
{
	/* static consts */
public:
	static const int NINODE = 100;	/* �ڴ�Inode������ */

	/* Functions */
public:
	/* Constructors */
	InodeTable();
	/* Destructors */
	~InodeTable();

	/*
	 * @comment ��ʼ����g_FileSystem���������
	 */
	void Initialize();
	/*
	 * @comment ����ָ���豸��dev�����Inode��Ż�ȡ��Ӧ
	 * Inode�������Inode�Ѿ����ڴ��У��������������ظ��ڴ�Inode��
	 * ��������ڴ��У���������ڴ�����������ظ��ڴ�Inode
	 */
	Inode* IGet(int inumber);
	/*
	 * @comment ���ٸ��ڴ�Inode�����ü����������Inode�Ѿ�û��Ŀ¼��ָ������
	 * ���޽������ø�Inode�����ͷŴ��ļ�ռ�õĴ��̿顣
	 */
	void IPut(Inode* pNode);

	/*
	 * @comment �����б��޸Ĺ����ڴ�Inode���µ���Ӧ���Inode��
	 */
	void UpdateInodeTable();

	/*
	 * @comment ����豸dev�ϱ��Ϊinumber�����inode�Ƿ����ڴ濽����
	 * ������򷵻ظ��ڴ�Inode���ڴ�Inode���е�����
	 */
	int IsLoaded(int inumber);
	/*
	 * @comment ���ڴ�Inode����Ѱ��һ�����е��ڴ�Inode
	 */
	Inode* GetFreeInode();

	/* Members */
public:
	Inode m_Inode[NINODE];		/* �ڴ�Inode���飬ÿ�����ļ�����ռ��һ���ڴ�Inode */
};


class OpenFiles
{
	/* static members */
public:
	static const int NOFILES = 15;	/* ��������򿪵�����ļ��� */
	File* k_OpenFileTable[NOFILES];		/* File�����ָ�����飬ָ��ϵͳ���ļ����е�File���� */

	/* Functions */
public:
	/* Constructors */
	OpenFiles();
	/* Destructors */
	~OpenFiles();

	/*
	 * @comment ����������ļ�ʱ���ڴ��ļ����������з���һ�����б���
	 */
	int AllocFreeSlot();

	/*
	 * @comment Dupϵͳ����ʱ���ƴ��ļ����������е�������
	 */
	int	Clone(int fd);

	/*
	 * @comment �����û�ϵͳ�����ṩ���ļ�����������fd��
	 * �ҵ���Ӧ�Ĵ��ļ����ƿ�File�ṹ
	 */
	File* GetF(int fd);
	/*
	 * @comment Ϊ�ѷ��䵽�Ŀ���������fd���ѷ���Ĵ��ļ�����
	 * ����File������������ϵ
	 */
	void SetF(int fd, File* pFile);


};


#endif
