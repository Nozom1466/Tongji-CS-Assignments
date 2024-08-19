#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "INode.h"
#include "Buf.h"
#include "BufferManager.h"
#include "Log.h"



/*
* LOG:
*	DELETE `Mount` class & correlated functions
*/




/*
 * �ļ�ϵͳ�洢��Դ�����(Super Block)�Ķ��塣
 */
class SuperBlock {
public:
	SuperBlock();
	~SuperBlock();
public:
	const static int MAX_NUMBER_FREE = 100;
	const static int MAX_NUMBER_INODE = 100;

public:
	int		s_isize;		/* ���Inode��ռ�õ��̿��� */
	int		s_fsize;		/* �̿����� */

	int		s_nfree;		/* ֱ�ӹ���Ŀ����̿����� */
	int		s_free[MAX_NUMBER_FREE];	/* ֱ�ӹ���Ŀ����̿������� */

	int		s_ninode;		/* ֱ�ӹ���Ŀ������Inode���� */
	int		s_inode[MAX_NUMBER_INODE];	/* ֱ�ӹ���Ŀ������Inode������ */

	int		s_fmod;			/* �ڴ���super block�������޸ı�־����ζ����Ҫ��������Ӧ��Super Block */
	int		s_time;			/* ���һ�θ���ʱ�� */
	int		padding[50];	/* ���ʹSuperBlock���С����1024�ֽڣ�ռ��2������ */
};

/*
 * �ļ�ϵͳ��(FileSystem)�����ļ��洢�豸��
 * �ĸ���洢��Դ�����̿顢���INode�ķ��䡢
 * �ͷš�
 */
class FileSystem
{
public:
	/* static consts */
	static const int NMOUNT = 5;						/* ϵͳ�����ڹ������ļ�ϵͳ��װ������� */

	static const int SUPER_BLOCK_SECTOR_NUMBER = 200;	/* ����SuperBlockλ�ڴ����ϵ������ţ�ռ��200��201���������� */

	static const int ROOTINO = 0;						/* �ļ�ϵͳ��Ŀ¼���Inode��� */

	static const int INODE_NUMBER_PER_SECTOR = 8;		/* ���INode���󳤶�Ϊ64�ֽڣ�ÿ�����̿���Դ��512/64 = 8�����Inode */
	static const int INODE_ZONE_START_SECTOR = 202;		/* ���Inode��λ�ڴ����ϵ���ʼ������ */
	static const int INODE_ZONE_SIZE = 1024 - 202;		/* ���������Inode��ռ�ݵ������� */

	static const int DATA_ZONE_START_SECTOR = 1024;		/* ����������ʼ������ */
	static const int DATA_ZONE_END_SECTOR = 18000 - 1;	/* �������Ľ��������� */
	static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR;	/* ������ռ�ݵ��������� */

public:
	FileSystem();
	~FileSystem();

	void Initialize(); // ��ʼ����Ա����

	void LoadSuperBlock(); // ϵͳ��ʼ������ Superblock

	SuperBlock* GetFS(short dev);

	void Update();  //��SuperBlock������ڴ渱�����µ��洢�豸��SuperBlock��ȥ

	Inode* IAlloc();  // �ڴ洢�豸dev�Ϸ���һ���������INode��һ�����ڴ����µ��ļ���

	void IFree(int number); // �ͷŴ洢�豸dev�ϱ��Ϊnumber�����INode��һ������ɾ���ļ���

	Buf* Alloc(); // �ڴ洢�豸dev�Ϸ�����д��̿�

	void Free(int blkno); // �ͷŴ洢�豸dev�ϱ��Ϊblkno�Ĵ��̿�
};

#endif
