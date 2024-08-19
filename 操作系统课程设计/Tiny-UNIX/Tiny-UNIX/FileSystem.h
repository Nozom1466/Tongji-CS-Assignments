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
 * 文件系统存储资源管理块(Super Block)的定义。
 */
class SuperBlock {
public:
	SuperBlock();
	~SuperBlock();
public:
	const static int MAX_NUMBER_FREE = 100;
	const static int MAX_NUMBER_INODE = 100;

public:
	int		s_isize;		/* 外存Inode区占用的盘块数 */
	int		s_fsize;		/* 盘块总数 */

	int		s_nfree;		/* 直接管理的空闲盘块数量 */
	int		s_free[MAX_NUMBER_FREE];	/* 直接管理的空闲盘块索引表 */

	int		s_ninode;		/* 直接管理的空闲外存Inode数量 */
	int		s_inode[MAX_NUMBER_INODE];	/* 直接管理的空闲外存Inode索引表 */

	int		s_fmod;			/* 内存中super block副本被修改标志，意味着需要更新外存对应的Super Block */
	int		s_time;			/* 最近一次更新时间 */
	int		padding[50];	/* 填充使SuperBlock块大小等于1024字节，占据2个扇区 */
};

/*
 * 文件系统类(FileSystem)管理文件存储设备中
 * 的各类存储资源，磁盘块、外存INode的分配、
 * 释放。
 */
class FileSystem
{
public:
	/* static consts */
	static const int NMOUNT = 5;						/* 系统中用于挂载子文件系统的装配块数量 */

	static const int SUPER_BLOCK_SECTOR_NUMBER = 200;	/* 定义SuperBlock位于磁盘上的扇区号，占据200，201两个扇区。 */

	static const int ROOTINO = 0;						/* 文件系统根目录外存Inode编号 */

	static const int INODE_NUMBER_PER_SECTOR = 8;		/* 外存INode对象长度为64字节，每个磁盘块可以存放512/64 = 8个外存Inode */
	static const int INODE_ZONE_START_SECTOR = 202;		/* 外存Inode区位于磁盘上的起始扇区号 */
	static const int INODE_ZONE_SIZE = 1024 - 202;		/* 磁盘上外存Inode区占据的扇区数 */

	static const int DATA_ZONE_START_SECTOR = 1024;		/* 数据区的起始扇区号 */
	static const int DATA_ZONE_END_SECTOR = 18000 - 1;	/* 数据区的结束扇区号 */
	static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR;	/* 数据区占据的扇区数量 */

public:
	FileSystem();
	~FileSystem();

	void Initialize(); // 初始化成员变量

	void LoadSuperBlock(); // 系统初始化读入 Superblock

	SuperBlock* GetFS(short dev);

	void Update();  //将SuperBlock对象的内存副本更新到存储设备的SuperBlock中去

	Inode* IAlloc();  // 在存储设备dev上分配一个空闲外存INode，一般用于创建新的文件。

	void IFree(int number); // 释放存储设备dev上编号为number的外存INode，一般用于删除文件。

	Buf* Alloc(); // 在存储设备dev上分配空闲磁盘块

	void Free(int blkno); // 释放存储设备dev上编号为blkno的磁盘块
};

#endif
