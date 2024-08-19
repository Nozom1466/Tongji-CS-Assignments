#include "OpenFileManager.h"
#include "Kernel.h"

File::File() {
	this->f_count = 0;
	this->f_flag = 0;
	this->f_offset = 0;
	this->f_inode = NULL;
}

File::~File() {

}

InodeTable::InodeTable() {

}

InodeTable::~InodeTable() {

}

Inode* InodeTable::IGet(int inumber) {
	Inode* pInode;
	Kernel* k = Kernel::getKernel();
	BufferManager* bufMgr = k->getBufferManager();

	while (true)
	{
		/* 检查外存Inode是否有内存拷贝 */
		int index = this->IsLoaded(inumber);
		if (index >= 0)	/* 找到内存拷贝 */
		{
			pInode = &(this->m_Inode[index]);

			/*
			 * 程序执行到这里表示：内存Inode高速缓存中找到相应内存Inode，
			 * 增加其引用计数，增设ILOCK标志并返回之
			 */
			pInode->i_count++;

			return pInode;
		}
		else	/* 没有Inode的内存拷贝，则分配一个空闲内存Inode */
		{
			pInode = this->GetFreeInode();
			/* 若内存Inode表已满，分配空闲Inode失败 */
			if (pInode == NULL) {
				Log::log("[ERROR] Inode Table Overflow!");
				return NULL;
			}
			else	/* 分配空闲Inode成功，将外存Inode读入新分配的内存Inode */
			{
				/* 设置新的设备号、外存Inode编号，增加引用计数，对索引节点上锁 */
				pInode->i_number = inumber;
				pInode->i_count++;
				//pInode->i_lastr = -1;

				/* 将该外存Inode读入缓冲区 */
				Buf* pBuf = bufMgr->Bread(FileSystem::INODE_ZONE_START_SECTOR + inumber / FileSystem::INODE_NUMBER_PER_SECTOR);

				/* 如果发生I/O错误 */
				if (pBuf->b_flags & Buf::B_ERROR)
				{
					/* 释放缓存 */
					bufMgr->Brelse(pBuf);
					/* 释放占据的内存Inode */
					this->IPut(pInode);
					Log::log("[ERROR] FROM IGet: I/O Error.");
					return NULL;
				}

				/* 将缓冲区中的外存Inode信息拷贝到新分配的内存Inode中 */
				pInode->ICopy(pBuf, inumber);
				/* 释放缓存 */
				bufMgr->Brelse(pBuf);
				return pInode;
			}
		}
	}

	return nullptr;
}

void InodeTable::IPut(Inode* pNode) {
	/* 当前进程为引用该内存Inode的唯一进程，且准备释放该内存Inode */
	if (pNode->i_count == 1)
	{
		/*
		 * 上锁，因为在整个释放过程中可能因为磁盘操作而使得该进程睡眠，
		 * 此时有可能另一个进程会对该内存Inode进行操作，这将有可能导致错误。
		 */
		pNode->i_flag |= Inode::ILOCK;

		/* 该文件已经没有目录路径指向它 */
		if (pNode->i_nlink <= 0)
		{
			/* 释放该文件占据的数据盘块 */
			pNode->ITrunc();
			pNode->i_mode = 0;
			/* 释放对应的外存Inode */
			Kernel::getKernel()->getFileSystem()->IFree(pNode->i_number);
		}

		/* 更新外存Inode信息 */
		time_t now;
		time(&now);
		pNode->IUpdate(now);

		/* 清除内存Inode的所有标志位 */
		pNode->i_flag = 0;
		/* 这是内存inode空闲的标志之一，另一个是i_count == 0 */
		pNode->i_number = -1;
	}

	/* 减少内存Inode的引用计数，唤醒等待进程 */
	pNode->i_count--;
}

void InodeTable::UpdateInodeTable()
{
	for (int i = 0; i < InodeTable::NINODE; i++)
	{
		/* count不等于0，count == 0意味着该内存Inode未被任何打开文件引用，无需同步 */
		if (this->m_Inode[i].i_count != 0) {
			time_t now;
			time(&now);
			this->m_Inode[i].IUpdate(now);
		}
			
	}
}

void FileSystem::Update()
{
	SuperBlock* spb = Kernel::getKernel()->getSuperBlock();
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Buf* pBuf;

	/* 如果该SuperBlock内存副本没有被修改，直接管理inode和空闲盘块被上锁或该文件系统是只读文件系统 */
	if (spb->s_fmod == 0) {
		return;;
	}

	/* 清SuperBlock修改标志 */
	spb->s_fmod = 0;
	/* 写入SuperBlock最后存访时间 */
	time_t now;
	time(&now);
	spb->s_time = now;

	/*
	 * 为将要写回到磁盘上去的SuperBlock申请一块缓存，由于缓存块大小为512字节，
	 * SuperBlock大小为1024字节，占据2个连续的扇区，所以需要2次写入操作。
	 */
	for (int j = 0; j < 2; j++)
	{
		/* 第一次p指向SuperBlock的第0字节，第二次p指向第512字节 */
		int* p = (int*)spb + j * 128;

		/* 将要写入到设备dev上的SUPER_BLOCK_SECTOR_NUMBER + j扇区中去 */
		pBuf = bufMgr->GetBlk(FileSystem::SUPER_BLOCK_SECTOR_NUMBER + j);

		/* 将SuperBlock中第0 - 511字节写入缓存区 */
		Toolkit::copy(p, (int*)pBuf->b_addr, 128);

		/* 将缓冲区中的数据写到磁盘上 */
		bufMgr->Bwrite(pBuf);
	}

	/* 同步修改过的内存Inode到对应外存Inode */
	InodeTable* k_InodeTable = Kernel::getKernel()->getInodeTable();
	k_InodeTable->UpdateInodeTable();

	/* 将延迟写的缓存块写到磁盘上 */
	bufMgr->Bflush();
}

Inode* FileSystem::IAlloc()
{
	
	Buf* pBuf;
	Inode* pNode;
	int ino;	/* 分配到的空闲外存Inode编号 */

	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Kernel* k = Kernel::getKernel();
	InodeTable* k_InodeTable = Kernel::getKernel()->getInodeTable();

	SuperBlock* sb = k->getSuperBlock();

	/*
	 * SuperBlock直接管理的空闲Inode索引表已空，
	 * 必须到磁盘上搜索空闲Inode。先对inode列表上锁，
	 * 因为在以下程序中会进行读盘操作可能会导致进程切换，
	 * 其他进程有可能访问该索引表，将会导致不一致性。
	 */
	if (sb->s_ninode <= 0)
	{

		/* 外存Inode编号从0开始，这不同于Unix V6中外存Inode从1开始编号 */
		ino = -1;

		/* 依次读入磁盘Inode区中的磁盘块，搜索其中空闲外存Inode，记入空闲Inode索引表 */
		for (int i = 0; i < sb->s_isize; i++)
		{
			pBuf = bufMgr->Bread(FileSystem::INODE_ZONE_START_SECTOR + i);

			/* 获取缓冲区首址 */
			int* p = (int*)pBuf->b_addr;

			/* 检查该缓冲区中每个外存Inode的i_mode != 0，表示已经被占用 */
			for (int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; j++)
			{
				ino++;

				int mode = *(p + j * sizeof(DiskInode) / sizeof(int));

				/* 该外存Inode已被占用，不能记入空闲Inode索引表 */
				if (mode != 0)
				{
					continue;
				}

				/*
				 * 如果外存inode的i_mode==0，此时并不能确定
				 * 该inode是空闲的，因为有可能是内存inode没有写到
				 * 磁盘上,所以要继续搜索内存inode中是否有相应的项
				 */
				if (k_InodeTable->IsLoaded(ino) == -1)
				{
					/* 该外存Inode没有对应的内存拷贝，将其记入空闲Inode索引表 */
					sb->s_inode[sb->s_ninode++] = ino;

					/* 如果空闲索引表已经装满，则不继续搜索 */
					if (sb->s_ninode >= 100)
					{
						break;
					}
				}
			}

			/* 至此已读完当前磁盘块，释放相应的缓存 */
			bufMgr->Brelse(pBuf);

			/* 如果空闲索引表已经装满，则不继续搜索 */
			if (sb->s_ninode >= 100)
			{
				break;
			}
		}

		/* 如果在磁盘上没有搜索到任何可用外存Inode，返回NULL */
		if (sb->s_ninode <= 0)
		{
			Log::log("[ERROR] FROM Creat: No Space left on device!");
			return NULL;
		}
	}

	/*
	 * 上面部分已经保证，除非系统中没有可用外存Inode，
	 * 否则空闲Inode索引表中必定会记录可用外存Inode的编号。
	 */
	while (true)
	{
		/* 从索引表“栈顶”获取空闲外存Inode编号 */
		ino = sb->s_inode[--sb->s_ninode];

		/* 将空闲Inode读入内存 */
		pNode = k_InodeTable->IGet(ino);
		/* 未能分配到内存inode */
		if (NULL == pNode)
		{
			return NULL;
		}

		/* 如果该Inode空闲,清空Inode中的数据 */
		if (0 == pNode->i_mode)
		{
			pNode->Clean();
			/* 设置SuperBlock被修改标志 */
			sb->s_fmod = 1;
			return pNode;
		}
		else	/* 如果该Inode已被占用 */
		{
			k_InodeTable->IPut(pNode);
			continue;	/* while循环 */
		}
	}
	return NULL;	/* GCC likes it! */
}

void FileSystem::IFree(int number) {
	SuperBlock* sb = Kernel::getKernel()->getSuperBlock();


	/*
	 * 如果超级块直接管理的空闲外存Inode超过100个，
	 * 同样让释放的外存Inode散落在磁盘Inode区中。
	 */
	if (sb->s_ninode >= 100)
	{
		return;
	}

	sb->s_inode[sb->s_ninode++] = number;

	/* 设置SuperBlock被修改标志 */
	sb->s_fmod = 1;
}

int InodeTable::IsLoaded(int inumber) {

	for (int i = 0; i < InodeTable::NINODE; i++)
	{
		if (this->m_Inode[i].i_number == inumber && this->m_Inode[i].i_count != 0)
		{
			return i;
		}
	}
	return -1;
}

Inode* InodeTable::GetFreeInode() {
	for (int i = 0; i < InodeTable::NINODE; i++) {
		/* 如果该内存Inode引用计数为零，则该Inode表示空闲 */
		if (this->m_Inode[i].i_count == 0) {
			return &(this->m_Inode[i]);
		}
	}
	return NULL;	/* 寻找失败 */
}

void InodeTable::Initialize() {
	//this->m_FileSystem = Kernel::getKernel()->getFileSystem();
}

OpenFileTable::OpenFileTable() {

}

OpenFileTable::~OpenFileTable() {

}

File* OpenFileTable::FAlloc()
{
	int fd;
	Kernel* k = Kernel::getKernel();

	/* 在内核打开文件描述符表中获取一个空闲项 */
	fd = k->getKernelOpenFiles()->AllocFreeSlot();

	if (fd < 0)  /* 如果寻找空闲项失败 */
	{
		return NULL;
	}

	for (int i = 0; i < OpenFileTable::NFILE; i++)
	{
		/* f_count==0表示该项空闲 */
		if (this->m_File[i].f_count == 0)
		{
			/* 建立描述符和File结构的勾连关系 */
			k->getKernelOpenFiles()->SetF(fd, &this->m_File[i]);
			/* 增加对file结构的引用计数 */
			this->m_File[i].f_count++;
			/* 清空文件读、写位置 */
			this->m_File[i].f_offset = 0;
			return (&this->m_File[i]);
		}
	}
	cout << "NOTFOUND" << endl;
	return NULL;
}

void OpenFileTable::CloseF(File* pFile)
{
	/* 引用计数f_count将减为0，则释放File结构 */
	if (pFile->f_count <= 1)
	{
		Kernel::getKernel()->getInodeTable()->IPut(pFile->f_inode);
	}


	/* 引用当前File的进程数减1 */
	pFile->f_count--;
}

OpenFiles::OpenFiles() {
	for (int i = 0; i < OpenFiles::NOFILES; i++) {
		SetF(i, NULL);
	}
}

OpenFiles::~OpenFiles() {

}

int OpenFiles::AllocFreeSlot()
{
	int i;
	for (i = 0; i < OpenFiles::NOFILES; i++)
	{
		/* 进程打开文件描述符表中找到空闲项，则返回之 */
		if (this->k_OpenFileTable[i] == NULL)
		{
			/* 系统调用返回值 */
			Kernel::getKernel()->systemcallRet = i;
			return i;
		}
	}
	Kernel::getKernel()->systemcallRet = -1;   /* Open1，需要一个标志。当打开文件结构创建失败时，可以回收系统资源*/
	return -1;
}

File* OpenFiles::GetF(int fd)
{
	File* pFile;
	Kernel* k = Kernel::getKernel();

	/* 如果打开文件描述符的值超出了范围 */
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		k->errorCode = Kernel::BADF;
		return NULL;
	}

	if ((pFile = this->k_OpenFileTable[fd]) == NULL)
	{
		k->errorCode = Kernel::BADF;
		return NULL;
	}
	return pFile;   /* 即使pFile==NULL也返回它，由调用GetF的函数来判断返回值 */
}

void OpenFiles::SetF(int fd, File* pFile) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
		return;
	/* 进程打开文件描述符指向系统打开文件表中相应的File结构 */
	this->k_OpenFileTable[fd] = pFile;
}
