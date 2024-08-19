#include "FileManager.h"
#include "Kernel.h"

FileManager::FileManager() {

}

FileManager::~FileManager() {

}

void FileManager::Initialize() {
	Log::log("[SYSTM] Initializing FileManager ...");
	//this->m_FileSystem = Kernel::getKernel()->getFileSystem();

	//this->m_InodeTable = Kernel::getKernel()->getInodeTable();
	//this->m_OpenFileTable = Kernel::getKernel()->getOpenFileTable();

	//this->m_InodeTable->Initialize();

	Log::log("[SYSTM] FileManager Initialized.");
}

void FileManager::Open()
{
	Inode* pInode;
	Kernel* k = Kernel::getKernel();

	pInode = this->NameI(NextChar, FileManager::OPEN);
	/* 没有找到相应的Inode */
	if (pInode == NULL)
		return;
	this->Open1(pInode, k->mode, 0);
}

void FileManager::Creat() {
	Kernel* k = Kernel::getKernel();
	Inode* pInode;
	unsigned int newACCMode = k->mode & (Inode::IRWXU | Inode::IRWXG | Inode::IRWXO);

	/* 搜索目录的模式为1，表示创建；若父目录不可写，出错返回 */
	pInode = this->NameI(NextChar, FileManager::CREATE);
	/* 没有找到相应的Inode，或NameI出错 */
	if (pInode == NULL)
	{
		if (k->errorCode != Kernel::NO_ERROR)
			return;
		/* 创建Inode */
		pInode = this->MakNode(newACCMode & (~Inode::ISVTX));
		/* 创建失败 */
		if (NULL == pInode)
		{
			Log::log("[ERROR] FROM Creat: pInode Allocation failed.");
			return;
		}

		/*
		 * 如果所希望的名字不存在，使用参数trf = 2来调用open1()。
		 * 不需要进行权限检查，因为刚刚建立的文件的权限和传入参数mode
		 * 所表示的权限内容是一样的。
		 */
		this->Open1(pInode, File::FWRITE, 2);
	}
	else
	{
		/* 如果NameI()搜索到已经存在要创建的文件，则清空该文件（用算法ITrunc()）。UID没有改变
		 * 原来UNIX的设计是这样：文件看上去就像新建的文件一样。然而，新文件所有者和许可权方式没变。
		 * 也就是说creat指定的RWX比特无效。
		 * 邓蓉认为这是不合理的，应该改变。
		 * 现在的实现：creat指定的RWX比特有效 */
		this->Open1(pInode, File::FWRITE, 1);
		pInode->i_mode |= newACCMode;
	}
}

/*
* trf == 0由open调用
* trf == 1由creat调用，creat文件的时候搜索到同文件名的文件
* trf == 2由creat调用，creat文件的时候未搜索到同文件名的文件，这是文件创建时更一般的情况
* mode参数：打开文件模式，表示文件操作是 读、写还是读写
*/
void FileManager::Open1(Inode* pInode, int mode, int trf)
{
	Kernel* k = Kernel::getKernel();
	/*
	 * 对所希望的文件已存在的情况下，即trf == 0或trf == 1进行权限检查
	 * 如果所希望的名字不存在，即trf == 2，不需要进行权限检查，因为刚建立
	 * 的文件的权限和传入的参数mode的所表示的权限内容是一样的。
	 */
	if (trf != 2)
	{
		if (mode & File::FREAD)
		{
			/* 检查读权限 */
			this->Access(pInode, Inode::IREAD);
		}
		if (mode & File::FWRITE)
		{
			/* 检查写权限 */
			this->Access(pInode, Inode::IWRITE);
			/* 系统调用去写目录文件是不允许的 */

			if ((pInode->i_mode & Inode::IFMT) == Inode::IFDIR)
			{
				k->errorCode = Kernel::ISDIR;
			}
		}
	}

	if (k->errorCode != Kernel::NO_ERROR)
	{
		cout << "Permission Denied." << endl;
		k->getInodeTable()->IPut(pInode);
		return;
	}

	/* 在creat文件的时候搜索到同文件名的文件，释放该文件所占据的所有盘块 */
	if (trf == 1)
	{
		pInode->ITrunc();
	}

	/* 分配打开文件控制块File结构 */
	File* pFile = k->getOpenFileTable()->FAlloc();
	if (pFile == NULL)
	{
		k->getInodeTable()->IPut(pInode);
		return;
	}
	/* 设置打开文件方式，建立File结构和内存Inode的勾连关系 */
	pFile->f_flag = mode & (File::FREAD | File::FWRITE);
	pFile->f_inode = pInode;


	// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	/* 不能创建目录文件！！！ */
	if (trf != 0 && k->isDir) {
		pInode->i_mode |= Inode::IFDIR;
	}
	// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

	return;
}

void FileManager::Close()
{
	Kernel* k = Kernel::getKernel();
	/* 获取打开文件控制块File结构 */
	File* pFile = k->getKernelOpenFiles()->GetF(k->fd);
	if (pFile == NULL)
		return;
	/* 释放打开文件描述符fd，递减File结构引用计数 */
	k->getKernelOpenFiles()->SetF(k->fd, NULL);
	/* 减少在系统打开文件表中File的引用计数 */
	k->getOpenFileTable()->CloseF(pFile);
}

void FileManager::Seek()
{
	File* pFile;
	Kernel* k = Kernel::getKernel();
	int fd = k->fd;

	pFile = k->getKernelOpenFiles()->GetF(fd);
	if (NULL == pFile)
	{
		return;     /* 若FILE不存在，GetF有设出错码 */
	}

	int offset = k->offset;

	/* 如果seek模式在3 ~ 5之间，则长度单位由字节变为512字节 */
	if (k->mode > 2) {
		offset = offset << 9;
		k->mode -= 3;
	}

	switch (k->mode)
	{
		/* 读写位置设置为offset */
	case 0:
		pFile->f_offset = offset;
		break;
		/* 读写位置加offset(可正可负) */
	case 1:
		pFile->f_offset += offset;
		break;
		/* 读写位置调整为文件长度加offset */
	case 2:
		pFile->f_offset = pFile->f_inode->i_size + offset;
		break;
	}
}


void FileManager::Delete()
{
	Inode* pInode;  //当前目录的INode指针
	Inode* pDeleteInode;  //当前文件的INode指针
	Kernel* k = Kernel::getKernel();

	pDeleteInode = this->NameI(FileManager::NextChar, FileManager::DELETE);
	//没找到需要删除的文件
	if (pDeleteInode == NULL) {
		Log::log("[ERROR] FROM Delete: pDeleteInode not found.");
		return;
	}
		
	pInode = k->getInodeTable()->IGet(k->dent.inode);
	k->k_IOParam.m_Offset -= (DirectoryEntry::DIRSIZ + 4);
	k->k_IOParam.m_Base = (char*)&k->dent;
	k->k_IOParam.m_Count = DirectoryEntry::DIRSIZ + 4;
	k->dent.inode = 0;
	pDeleteInode->WriteI();        //删除的文件写回磁盘
	pInode->i_nlink--;
	pInode->i_flag |= Inode::IUPD; //nlink--
	k->getInodeTable()->IPut(pDeleteInode);
	k->getInodeTable()->IPut(pInode);
}

void FileManager::Rename(string from, string to)
{
	Inode* pInode;  //当前目录的INode指针
	Kernel* k = Kernel::getKernel();
	Buf* pBuf = NULL;
	BufferManager* bufMgr = k->getBufferManager();
	bool found = false;

	pInode = k->cdir;
	k->k_IOParam.m_Offset = 0;
	k->k_IOParam.m_Count = pInode->i_size / sizeof(DirectoryEntry);

	while (k->k_IOParam.m_Count) {

		if (0 == k->k_IOParam.m_Offset % Inode::BLOCK_SIZE) {  //要换盘快了
			if (pBuf) { //缓存有信息
				bufMgr->Brelse(pBuf); //释放
			}
			int phyBlkno = pInode->Bmap(k->k_IOParam.m_Offset / Inode::BLOCK_SIZE); //新的盘块号
			pBuf = bufMgr->Bread(phyBlkno); //缓存读新的盘块
		}

		DirectoryEntry tmp;
		memcpy(&tmp, pBuf->b_addr + (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE), sizeof(k->dent));
		if (strcmp(tmp.name, from.c_str()) == 0) {
			strcpy_s(tmp.name, to.c_str());
			memcpy(pBuf->b_addr + (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE), &tmp, sizeof(k->dent));
			found = true;
		}

		k->k_IOParam.m_Offset += sizeof(DirectoryEntry);
		k->k_IOParam.m_Count--;
	}

	if (!found) {
		k->errorCode = Kernel::NOENT;
	}

	if (pBuf) {
		bufMgr->Bwrite(pBuf);
	}
}

void FileManager::ChDir(int p) {
	Inode* pInode;
	Kernel* k = Kernel::getKernel();

	pInode = this->NameI(FileManager::NextChar, FileManager::OPEN);
	if (p) pInode = k->pdir;
	if (NULL == pInode)
	{
		Log::log("[ERROR] FROM ChDir: NULL pInode.");
		return;
	}
	/* 搜索到的文件不是目录文件 */
	if ((pInode->i_mode & Inode::IFMT) != Inode::IFDIR)
	{
		k->errorCode = Kernel::NOTDIR;
		k->getInodeTable()->IPut(pInode);
		Log::log("[ERROR] FROM ChDir: Target File is not a Directory.");
		return;
	}

	if (this->Access(pInode, Inode::IEXEC))
	{
		k->getInodeTable()->IPut(pInode);
		cout << "No authority to access target Directory." << endl;
		Log::log("[ERROR] FROM ChDir: No authority to access target Directory.");
		k->errorCode = Kernel::NOACCESS;
		return;
	}
	k->getInodeTable()->IPut(k->cdir);
	k->cdir = pInode;
	/* 路径不是从根目录'/'开始，则在现有u.u_curdir后面加上当前路径分量 */
	if (k->pathname[0] != '/')
	{
		int length = Toolkit::strLength(k->curdir);
		if (k->curdir[length - 1] != '/')
		{
			k->curdir[length] = '/';
			length++;
		}
		Toolkit::stringCopy(k->pathname, k->curdir + length);
	}
	else	/* 如果是从根目录'/'开始，则取代原有工作目录 */
	{
		Toolkit::stringCopy(k->pathname, k->curdir);
	}
}

void FileManager::dirDfs(string path, int depth)
{
	vector<string> dirs; /* 目录项 */
	string nextDir;

	Kernel* k = Kernel::getKernel();

	k->k_IOParam.m_Offset = 0;
	k->k_IOParam.m_Count = k->cdir->i_size / (DirectoryEntry::DIRSIZ + 4);
	Buf* pBuf = NULL;

	while (true)
	{
		/* 对目录项已经搜索完毕 */
		if (k->k_IOParam.m_Count == 0)
		{
			if (pBuf != NULL)
			{
				k->getBufferManager()->Brelse(pBuf);
			}
			break;
		}

		/* 已读完目录文件的当前盘块，需要读入下一目录项数据盘块 */
		if (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE == 0)
		{
			if (pBuf != NULL)
			{
				k->getBufferManager()->Brelse(pBuf);
			}
			int phyBlkno = k->cdir->Bmap(k->k_IOParam.m_Offset / Inode::BLOCK_SIZE);
			pBuf = k->getBufferManager()->Bread(phyBlkno);
		}

		/* 没有读完当前目录项盘块，则读取下一目录项至dent */
		int* src = (int*)(pBuf->b_addr + (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE));
		Toolkit::copy(src, (int*)&k->dent, sizeof(DirectoryEntry) / sizeof(int));
		k->k_IOParam.m_Offset += (DirectoryEntry::DIRSIZ + 4);
		k->k_IOParam.m_Count--;
		if (k->dent.inode == 0)
		{
			continue;
		}
		dirs.emplace_back(k->dent.name);
	}


	for (int i = 0; i < dirs.size(); i++) {
		nextDir = (path == "/" ? "" : path) + '/' + dirs[i];

		for (int j = 0; j < depth + 1; j++)
			cout << "|   ";

		cout << "|---" << dirs[i] << endl;
		char nd[128];
		strcpy_s(nd, nextDir.c_str());
		k->isVerbose = false;
		Systemcall::getSystemcallInst()->Sys_ChDir(nd);
		k->isVerbose = true;
		if (k->errorCode != Kernel::NO_ERROR)  /* 访问的是数据文件，不是目录文件 */
		{
			k->errorCode = Kernel::NO_ERROR;
			continue;
		}
		dirDfs(nextDir, depth + 1);
	}
	char nd[128];
	strcpy_s(nd, path.c_str());
	Systemcall::getSystemcallInst()->Sys_ChDir(nd);
	return;
}

char FileManager::NextChar() {
	Kernel* k = Kernel::getKernel();
	/* k->dirp指向pathname中的字符 */
	return *k->dirPointer++;
}

void FileManager::WriteDir(Inode* pInode) {
	Kernel* k = Kernel::getKernel();

	/* 设置目录项中Inode编号部分 */
	k->dent.inode = pInode->i_number;

	/* 设置目录项中pathname分量部分 */
	for (int i = 0; i < DirectoryEntry::DIRSIZ; i++)
	{
		k->dent.name[i] = k->dbuf[i];
	}

	k->k_IOParam.m_Count = DirectoryEntry::DIRSIZ + 4;
	k->k_IOParam.m_Base = (char*)&k->dent;

	/* 将目录项写入父目录文件 */
	k->pdir->WriteI();
	k->getInodeTable()->IPut(k->pdir);
}

int FileManager::Access(Inode* pInode, unsigned int mode) {
	Kernel* k = Kernel::getKernel();

	///*
	// * 对于超级用户，读写任何文件都是允许的
	// * 而要执行某文件时，必须在i_mode有可执行标志
	// */
	if (k->u_uid == 0)
	{
		//if (Inode::IEXEC == mode && (pInode->i_mode & (Inode::IEXEC | (Inode::IEXEC >> 3) | (Inode::IEXEC >> 6))) == 0)
		//{
		//	k->errorCode = Kernel::NOACCESS;
		//	return 1;
		//}
		return 0;	/* Permission Check Succeed! */
	}
	if (k->u_uid != pInode->i_uid)
	{
		mode = mode >> 3;
		if (k->u_gid != pInode->i_gid)
		{
			mode = mode >> 3;
		}
	}
	if ((pInode->i_mode & mode) != 0)
	{
		return 0;
	}

	k->errorCode = Kernel::NOACCESS;
	return 1;

	//return 0;
}

Inode* FileManager::NameI(char(*func)(), enum DirectorySearchMode mode)
{
	Inode* pInode;
	Buf* pBuf;
	char curchar;
	char* pChar;
	int freeEntryOffset;	         /* 以创建文件模式搜索目录时，记录空闲目录项的偏移量 */
	Kernel* k = Kernel::getKernel();
	BufferManager* bufMgr = k->getBufferManager();

	/*
	 * 如果该路径是'/'开头的，从根目录开始搜索，
	 * 否则从进程当前工作目录开始搜索。
	 */
	pInode = k->cdir;
	if ('/' == (curchar = (*func)())) {
		pInode = this->rootDirInode;
	}

	/* 检查该Inode是否正在被使用，以及保证在整个目录搜索过程中该Inode不被释放 */
	k->getInodeTable()->IGet(pInode->i_number);

	/* 允许出现////a//b 这种路径 这种路径等价于/a/b */
	while ('/' == curchar)
	{
		curchar = (*func)();
	}

	/* 如果试图更改和删除当前目录文件则出错 */
	if ('\0' == curchar && mode != FileManager::OPEN) {
		goto out;
	}

	/* 外层循环每次处理pathname中一段路径分量 */
	while (true)
	{
		/* 如果出错则释放当前搜索到的目录文件Inode，并退出 */
		if (k->errorCode != Kernel::NO_ERROR)
		{
			break;     /* goto out; */
		}

		/* 整个路径搜索完毕，返回相应Inode指针。目录搜索成功返回。 */
		if ('\0' == curchar)
		{
			return pInode;
		}

		/* 进行目录搜索权限检查,IEXEC在目录文件中表示搜索权限 */
		//if (this->Access(pInode, Inode::IEXEC))
		//{
		//	cout << "Access Denied, no authority." << endl;
		//	Log::log("[ERROR] FROM NameI: Access Denied, no authority.");
		//	break;	/* 不具备目录搜索权限，goto out; */
		//}

		/* 将Pathname中当前准备进行匹配的路径分量拷贝到Kernel的dbuf[]中 便于和目录项进行比较 */
		pChar = &(k->dbuf[0]);
		while ('/' != curchar && '\0' != curchar && k->errorCode == Kernel::NO_ERROR)
		{
			if (pChar < &(k->dbuf[DirectoryEntry::DIRSIZ]))
			{
				*pChar = curchar;
				pChar++;
			}
			curchar = (*func)();
		}
		/* 将dbuf剩余的部分填充为'\0' */
		while (pChar < &(k->dbuf[DirectoryEntry::DIRSIZ]))
		{
			*pChar = '\0';
			pChar++;
		}

		/* 允许出现////a//b 这种路径 这种路径等价于/a/b */
		while ('/' == curchar)
		{
			curchar = (*func)();
		}

		if (k->errorCode != Kernel::NO_ERROR)
		{
			return NULL;           /* */
		}

		/* 内层循环部分对于dbuf[]中的路径名分量，逐个搜寻匹配的目录项 */
		k->k_IOParam.m_Offset = 0;
		/* 设置为目录项个数 ，含空白的目录项*/
		k->k_IOParam.m_Count = pInode->i_size / (DirectoryEntry::DIRSIZ + 4);   /* 4是外存INode大小 */
		freeEntryOffset = 0;
		pBuf = NULL;

		while (true)
		{
			/* 对目录项已经搜索完毕 */
			if (k->k_IOParam.m_Count == 0)
			{
				if (pBuf != NULL)
				{
					bufMgr->Brelse(pBuf);
				}
				/* 如果是创建新文件 */
				if (FileManager::CREATE == mode && curchar == '\0')
				{
					/* 将父目录Inode指针保存起来，以后写目录项“WriteDir()函数”会用到 */
					k->pdir = pInode;

					if (freeEntryOffset)	/* 此变量存放了空闲目录项位于目录文件中的偏移量 */
					{
						/* 将空闲目录项偏移量存入u区中，写目录项WriteDir()会用到 */
						k->k_IOParam.m_Offset = freeEntryOffset - (DirectoryEntry::DIRSIZ + 4);
					}
					else   /*问题：为何if分支没有置IUPD标志？  这是因为文件的长度没有变呀*/
					{
						pInode->i_flag |= Inode::IUPD;
					}
					/* 找到可以写入的空闲目录项位置，NameI()函数返回 */
					return NULL;
				}

				/* 目录项搜索完毕而没有找到匹配项，释放相关Inode资源,并退出 */
				k->errorCode = Kernel::NOENT;
				goto out;
			}

			/* 已读完目录文件的当前盘块，需要读入下一目录项数据盘块 */
			if (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE == 0)
			{
				if (pBuf != NULL)
				{
					bufMgr->Brelse(pBuf);
				}
				/* 计算要读的物理盘块号 */
				int phyBlkno = pInode->Bmap(k->k_IOParam.m_Offset / Inode::BLOCK_SIZE);
				pBuf = bufMgr->Bread(phyBlkno);
			}

			/* 没有读完当前目录项盘块，则读取下一目录项至dent */
			int* src = (int*)(pBuf->b_addr + (k->k_IOParam.m_Offset % Inode::BLOCK_SIZE));
			Toolkit::copy(src, (int*)&k->dent, sizeof(DirectoryEntry) / sizeof(int));

			k->k_IOParam.m_Offset += (DirectoryEntry::DIRSIZ + 4);
			k->k_IOParam.m_Count--;

			/* 如果是空闲目录项，记录该项位于目录文件中偏移量 */
			if (k->dent.inode == 0)
			{
				if (freeEntryOffset == 0)
				{
					freeEntryOffset = k->k_IOParam.m_Offset;
				}
				/* 跳过空闲目录项，继续比较下一目录项 */
				continue;
			}

			int i;
			for (i = 0; i < DirectoryEntry::DIRSIZ; i++)
			{
				if (k->dbuf[i] != k->dent.name[i])
				{
					break;	/* 匹配至某一字符不符，跳出for循环 */
				}

			}

			if (i < DirectoryEntry::DIRSIZ)
			{
				/* 不是要搜索的目录项，继续匹配下一目录项 */
				continue;
			}
			else
			{
				/* 目录项匹配成功，回到外层While(true)循环 */
				break;
			}
		}

		/*
		 * 从内层目录项匹配循环跳至此处，说明pathname中
		 * 当前路径分量匹配成功了，还需匹配pathname中下一路径
		 * 分量，直至遇到'\0'结束。
		 */
		if (pBuf != NULL)
			bufMgr->Brelse(pBuf);

		/* 如果是删除操作，则返回父目录Inode，而要删除文件的Inode号在dent.inode中 */
		if (FileManager::DELETE == mode && '\0' == curchar)
			return pInode;

		/*
		 * 匹配目录项成功，则释放当前目录Inode，根据匹配成功的
		 * 目录项inode字段获取相应下一级目录或文件的Inode。
		 */
		k->getInodeTable()->IPut(pInode);
		pInode = k->getInodeTable()->IGet(k->dent.inode);
		/* 回到外层While(true)循环，继续匹配Pathname中下一路径分量 */

		if (pInode == NULL)   /* 获取失败 */
			return NULL;
	}
out:
	k->getInodeTable()->IPut(pInode);
	return NULL;
}

void FileManager::Read()
{
	this->Rdwr(File::FREAD);
}

void FileManager::Write() {
	this->Rdwr(File::FWRITE);
}

void FileManager::Rdwr(enum File::FileFlags mode)  ////////////////////////////////////////////////////
{
	File* pFile;
	Kernel* k = Kernel::getKernel();

	/* 根据Read()/Write()的系统调用参数fd获取打开文件控制块结构 */
	pFile = k->getKernelOpenFiles()->GetF(k->fd);
	if (pFile == NULL)
	{
		Log::log("[ERROR] FROM Rdwr: Target Open File does not found.");
		return;
	}

	k->k_IOParam.m_Base = (char*)k->buf;         /* 目标缓冲区首址 */
	k->k_IOParam.m_Count = k->nbytes;            /* 要求读/写的字节数 */
	k->k_IOParam.m_Offset = pFile->f_offset;     /* 设置文件起始读位置 */
	if (mode == File::FREAD)
		pFile->f_inode->ReadI();
	else
		pFile->f_inode->WriteI();

	/* 根据读写字数，移动文件读写偏移指针 */
	pFile->f_offset += (k->nbytes - k->k_IOParam.m_Count);
	/* 返回实际读写的字节数，修改存放系统调用返回值的核心栈单元 */
	cout << "File pointer:" << pFile->f_offset << endl;
	k->systemcallRet = k->nbytes - k->k_IOParam.m_Count;
}

DirectoryEntry::DirectoryEntry()
{
}

DirectoryEntry::~DirectoryEntry()
{
}
