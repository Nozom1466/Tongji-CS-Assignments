#include "Systemcall.h"

Systemcall::Systemcall()
{
}

Systemcall::~Systemcall()
{
}

Systemcall* Systemcall::getSystemcallInst()
{
	return &syscall;
}

/*
cd home
cd qq
cd uu
cd ../../wechat

*/

int Systemcall::Sys_ChDir(char* pathname) {
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();

	//cout << "PATHNAME " << pathname << "\t" << k->curdir << endl;


	int p = 0;
	char* tempCurdir = new char[128];
	tempCurdir[127] = '\0';
	Toolkit::IOMove(k->curdir, tempCurdir, 127);

	/*
	For example, path '../../etc'
	*/

	std::vector<std::string> tokens;
	std::stringstream ss(pathname);
	std::string token;

	while (getline(ss, token, '/')) {
		tokens.push_back(token);
	}

	if (tokens[0] == ".." && tokens.size() == 1 || tokens[0] != "..") {
		if (strcmp(pathname, "..") == 0) {
			if (k->curdir[1] == '\0')
			{
				cout << "Already in the root!" << endl;
				return 0;
			}
			else {
				for (int i = 127; i >= 0; i--) /* 从结尾开始找最后一个/ */
				{
					if (k->curdir[i] != '/') {
						k->curdir[i] = '\0';
					}
					else if (i != 0) {
						k->curdir[i] = '\0';
						break;
					}
				}

				pathname = k->curdir;
				//p = 1;

			}
		}
		else if (strcmp(pathname, ".") == 0) {
			return 0;
		}

		//Toolkit::IOMove(tempCurdir, k->curdir, 127);
		k->pathname = k->dirPointer = pathname;
		k->getFileManager()->ChDir(p);

		if (k->errorCode == Kernel::NOACCESS) {
			Toolkit::IOMove(tempCurdir, k->curdir, 127);
		}
		else if (k->errorCode == Kernel::NOTDIR) {
			if (k->isVerbose) {
				cout << pathname << ": Not a directory" << endl;
			}
		}
		else if (k->errorCode == Kernel::NOENT && k->isVerbose) {
			cout << pathname << ": No such file or directory" << endl;
		}
	}
	else {
		if (k->curdir[1] == '\0')
		{
			cout << "Already in the root!" << endl;
			return 0;
		}
		else {
			for (int i = 127; i >= 0; i--) /* 从结尾开始找最后一个/ */
			{
				if (k->curdir[i] != '/') {
					k->curdir[i] = '\0';
				}
				else if (i != 0) {
					k->curdir[i] = '\0';
					break;
				}
			}

			pathname = k->curdir;
			//p = 1;

		}


		k->pathname = k->dirPointer = pathname;
		k->getFileManager()->ChDir(p);
		if (!tokens.empty() && tokens[0] == "..") {
			// 移除第一个 `../`
			tokens.erase(tokens.begin());

			// 重新构建剩余路径
			string newPath;
			for (auto& t : tokens) {
				if (!newPath.empty()) {
					newPath += "/";
				}
				newPath += t;
			}

			// 递归调用 Sys_ChDir，传入新路径
			if (!newPath.empty()) {
				Sys_ChDir(const_cast<char*>(newPath.c_str()));
			}
		}
	}

	return 0;
}

int Systemcall::Sys_ChDir1(char* pathname)
{

	Kernel* k = Kernel::getKernel();
	k->refreshSystem();

	int p = 0;
	char* tempCurdir = new char[128];
	tempCurdir[127] = '\0';
	Toolkit::IOMove(k->curdir, tempCurdir, 127);

	/*
	For example, path '../../etc'

	*/



	if (strcmp(pathname, "..") == 0) {
		if (k->curdir[1] == '\0')
		{
			cout << "Already in the root!" << endl;
			return 0;
		}
		else {
			for (int i = 127; i >= 0; i--) /* 从结尾开始找最后一个/ */
			{
				if (k->curdir[i] != '/') {
					k->curdir[i] = '\0';
				}
				else if (i != 0) {
					k->curdir[i] = '\0';
					break;
				}
			}

			pathname = k->curdir;
			//p = 1;

		}
	}
	else if (strcmp(pathname, ".") == 0) {
		return 0;
	}

	//Toolkit::IOMove(tempCurdir, k->curdir, 127);
	k->pathname = k->dirPointer = pathname;
	k->getFileManager()->ChDir(p);

	if (k->errorCode == Kernel::NOACCESS) {
		Toolkit::IOMove(tempCurdir, k->curdir, 127);
	}
	else if (k->errorCode == Kernel::NOTDIR) {
		if (k->isVerbose) {
			cout << pathname << ": Not a directory" << endl;
		}
	}
	else if (k->errorCode == Kernel::NOENT) {
		cout << pathname << ": No such file or directory" << endl;
	}

	return 0;
}

int Systemcall::Sys_Creat(char* pathname, int mode, int verbose) {
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->isDir = false;
	k->mode = mode;
	k->pathname = k->dirPointer = pathname;
	k->getFileManager()->Creat();
	k->fileSys->Update();
	if (k->errorCode == Kernel::NOENT) {
		cout << "No such a file or directory" << endl;
	}
	else if (k->errorCode == Kernel::ISDIR && verbose) {
		cout << pathname << ": This is a directory" << endl;
	}
	else if (k->errorCode == Kernel::NO_ERROR && verbose) {
		cout << "File Handle fd = " << k->systemcallRet << endl;
	}
	return k->systemcallRet;
}

int Systemcall::Sys_Pwd() {
	Kernel* k = Kernel::getKernel();
	cout << k->curdir << endl;
	return 0;
}

int Systemcall::Sys_Mkdir(char* pathname, int verbose)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->isDir = true;
	k->pathname = k->dirPointer = pathname;
	k->fileMgr->Creat();
	k->fileSys->Update();
	if (k->systemcallRet != -1) {
		Systemcall::getSystemcallInst()->Sys_Close(k->systemcallRet);
	}
	if (k->errorCode == Kernel::NOENT) {
		cout << "No such a file or directory" << endl;
	}
	else if (k->errorCode == Kernel::ISDIR && verbose) {
		cout << pathname << ": This is a directory" << endl;
	}
	return 0;
}

int Systemcall::Sys_Rename(char* from, char* to)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	char curDir[128];
	memcpy(curDir, k->curdir, 128);
	string ori = from;
	string cur = to;
	if (ori.find('/') != string::npos) {
		string nextDir = ori.substr(0, ori.find_last_of('/'));
		if (nextDir == "") {
			nextDir = "/";
		}
		char nd[128];
		strcpy_s(nd, nextDir.c_str());
		Systemcall::getSystemcallInst()->Sys_ChDir(nd);
		ori = ori.substr(ori.find_last_of('/') + 1);
		if (cur.find('/') != string::npos)
			cur = cur.substr(cur.find_last_of('/') + 1);
	}
	k->getFileManager()->Rename(ori, cur);
	if (k->errorCode == Kernel::NOENT) {
		cout << from << ": No such a file or directory" << endl;
	}
	Systemcall::getSystemcallInst()->Sys_ChDir(curDir);
	return 0;
}

int Systemcall::Sys_Close(int fd)
{	
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->fd = fd;
	k->fileMgr->Close();
	return k->systemcallRet;
}

int Systemcall::Sys_Ls()
{
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

		Buf* pBuf = k->getBufferManager()->Bread(FileSystem::INODE_ZONE_START_SECTOR + k->dent.inode / FileSystem::INODE_NUMBER_PER_SECTOR);
		char* p = pBuf->b_addr + (k->dent.inode % FileSystem::INODE_NUMBER_PER_SECTOR) * sizeof(DiskInode);
		DiskInode* tempNode = new DiskInode;
		Toolkit::copy((int*)p, (int*)tempNode, sizeof(DiskInode) / sizeof(int));

		//struct tm* temptt = Toolkit::convertTime(tempNode->d_mtime);
		struct tm* temptt = Toolkit::convertTime(tempNode->d_atime);
		char now[64];
		strftime(now, 64, "%Y-%m-%d %H:%M:%S", temptt);

		cout << k->dent.name << "\t" << now << '\t' << tempNode->d_size << "B" <<  endl;
		//cout << k->dent.name  << endl;


	}
	cout << endl;
	return 0;
}

int Systemcall::Sys_Delete(char* pathname)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->dirPointer = pathname;
	k->fileMgr->Delete();
	if (k->errorCode == Kernel::NOENT) {
		cout << pathname << ": No such a file or directory" << endl;
	}
	return 0;
}

int Systemcall::Sys_Format()
{
	cout << "Please wait 30s - 1min until format process completed." << endl;
	/* 格式化磁盘 */
	Kernel* k = Kernel::getKernel();
	fstream f(DISK_IMG, ios::out | ios::binary);
	for (int i = 0; i <= k->getFileSystem()->DATA_ZONE_END_SECTOR; i++)
	{
		for (int j = 0; j < k->getBufferManager()->BUFFER_SIZE; j++)
		{
			f.write((char*)" ", 1);
		}

	}
	f.close();

	/* 格式化SuperBlock */
	Buf* pBuf;
	SuperBlock& spb = (*k->spb);
	spb.s_isize = FileSystem::INODE_ZONE_SIZE;
	spb.s_fsize = FileSystem::DATA_ZONE_SIZE;
	spb.s_ninode = 100;
	spb.s_nfree = 0;
	for (int i = 0; i < 100; i++)  //栈式存储
	{
		spb.s_inode[99 - i] = i + 1;
	}

	for (int i = FileSystem::DATA_ZONE_END_SECTOR; i >= FileSystem::DATA_ZONE_START_SECTOR; i--) //清磁盘数据区
	{
		k->getFileSystem()->Free(i);
	}

	for (int i = 0; i < 2; i++)
	{
		int* p = (int*)&spb + i * 128;
		pBuf = k->getBufferManager()->GetBlk(FileSystem::SUPER_BLOCK_SECTOR_NUMBER + i);
		Toolkit::copy(p, (int*)pBuf->b_addr, 128);
		k->getBufferManager()->Bwrite(pBuf);
	}

	/* 格式化Inode区 */
	for (int i = 0; i < FileSystem::INODE_ZONE_SIZE; i++)
	{
		pBuf = k->getBufferManager()->GetBlk(FileSystem::ROOTINO + FileSystem::INODE_ZONE_START_SECTOR + i);
		DiskInode DiskInode[FileSystem::INODE_NUMBER_PER_SECTOR];
		for (int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; j++)
		{
			//单个DiskINode格式化
			DiskInode[j].d_mode = DiskInode[j].d_nlink = DiskInode[j].d_size = 0;
			for (int k = 0; k < 10; k++)
			{
				DiskInode[j].d_addr[k] = 0;
			}
		}
		/* 为根目录增加目录标志 */
		if (i == 0)
		{
			DiskInode[0].d_mode |= Inode::IFDIR;
		}
		Toolkit::copy((int*)&DiskInode, (int*)pBuf->b_addr, 128);
		//写回磁盘
		k->getBufferManager()->Bwrite(pBuf);
	}

	k->BufMgr = new BufferManager;
	k->fileSys = new FileSystem;
	k->fileMgr = new FileManager;
	k->k_InodeTable = new InodeTable;
	k->s_openFiles = new OpenFileTable;
	k->k_openFiles = new OpenFiles;
	k->spb = new SuperBlock;

	k->isformat = true;
	Systemcall* syscall = this->getSystemcallInst();
	k->init(syscall);

	// Create dirs
	syscall->Sys_Mkdir((char*)"bin", 0);
	syscall->Sys_Mkdir((char*)"etc", 0);
	syscall->Sys_Mkdir((char*)"home", 0);
	syscall->Sys_Mkdir((char*)"dev", 0);

	// Init
	syscall->Sys_ChDir((char*)"etc");
	int fid = syscall->Sys_Creat((char*)"passwd", 0x1FF, 0);
	//int fid = syscall->Sys_OpenFile((char*)"passwd", 0x1FF, 1);

	string tempbuf = "root\n1466\n0\n0\nuser\n111\n1\n1";
	syscall->Sys_WriteFile(fid, (char*)tempbuf.c_str(), tempbuf.size(), 0);
	syscall->Sys_CloseFile(fid);
	syscall->Sys_ChDir((char*)"..");

	//cout << fid << endl;

	syscall->Sys_ChDir((char*)"etc");
	fid = syscall->Sys_OpenFile((char*)"passwd", 0x1FF, 0);
	syscall->Sys_ReadFile(fid, INT_MAX, 0);
	syscall->Sys_CloseFile(fid);
	syscall->Sys_ChDir((char*)"..");

	//cout << fid << endl;

	return 0;
}

int Systemcall::Sys_OpenFile(char* pathname, int mode, int verbose)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->mode = mode;
	k->pathname = k->dirPointer = pathname;
	k->fileMgr->Open();

	if (k->errorCode == Kernel::NO_ERROR && verbose)
		cout << "File Handle fd = " << k->systemcallRet << endl;
	else if (k->errorCode == Kernel::ISDIR && verbose)
		cout << pathname << ": This is a directory" << endl;
	else if (k->errorCode == Kernel::NOENT)
		cout << pathname << ": No such a file or directory" << endl;

	return k->systemcallRet;
}

int Systemcall::Sys_CloseFile(int fd)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->fd = fd;
	k->fileMgr->Close();
	return 0;
}

int Systemcall::Sys_WriteFile(int writeFd, char* buf, int nbytes, int verbose)
{
	if (nbytes > Toolkit::strLength(buf)) {
		cout << "[WARNING] nbytes can not be greater than the length of the string" << endl;
		//return 1;
	}

	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->fd = writeFd;
	k->buf = buf;
	k->nbytes = nbytes;
	k->getFileManager()->Write();

	if (k->systemcallRet == -1) {
		if (k->errorCode == Kernel::BADF) {
			cout << writeFd << ": This fd is wrong" << endl;
		}
	}
	else if (verbose) {
		cout << "Successfully write " << k->systemcallRet << " bytes" << endl;
	}
	return k->systemcallRet;
}

int Systemcall::Sys_ReadFile(int readFd, int nbytes, int verbose)
{
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	
	k->fd = readFd;
	char* buf = new char[nbytes];
	buf[0] = '\0';
	k->buf = buf;
	
	k->nbytes = nbytes;
	k->fileMgr->Read();

	if (k->systemcallRet == -1)
	{
		if (k->errorCode == Kernel::BADF)
			cout << readFd << ":This fd is wrong" << endl;
	}
	else {
		if (k->systemcallRet > 0 && verbose) {
			for (int i = 0; i < k->systemcallRet; i++) {
				cout << buf[i];
			}
			cout << endl;
		}
		if (verbose)
			cout << "Successfully read " << k->systemcallRet << " bytes" << endl;
	}

	return k->systemcallRet;
}

int Systemcall::Sys_SeekFilePointer(int seekFd, int offset, int SeekMode)
{
	if (SeekMode < 0 || SeekMode > 5) {
		cout << "SeekMode Error (0-5)." << endl;
		return 0;
	}
	Kernel* k = Kernel::getKernel();
	k->refreshSystem();
	k->fd = seekFd;
	k->offset = offset;
	k->mode = SeekMode;
	k->fileMgr->Seek();

	if (k->errorCode == Kernel::BADF) {
		cout << seekFd << ": This fd is wrong" << endl;
	}

	return 0;
}

int Systemcall::Sys_Cp(char* from, char* to)
{
	Kernel* k = Kernel::getKernel();

	int fromFileFd = Systemcall::getSystemcallInst()->Sys_OpenFile(from, 0x1FF, 1);

	if (k->offset > 0) {
		Systemcall::getSystemcallInst()->Sys_SeekFilePointer(fromFileFd, k->offset, 0);
	}

	int fileByte = 0;
	char* tempBuf = NULL;
	if (k->errorCode == Kernel::NO_ERROR) {
		fileByte = Systemcall::getSystemcallInst()->Sys_ReadFile(fromFileFd, INT_MAX, 0);
		tempBuf = new char[fileByte + 1];
		tempBuf[fileByte] = '\0';
		Toolkit::IOMove(k->buf, tempBuf, fileByte);
	}
	Systemcall::getSystemcallInst()->Sys_CloseFile(fromFileFd);


	int toFileFd = Systemcall::getSystemcallInst()->Sys_OpenFile(to, 0x1FF, 1);
	if (k->errorCode == Kernel::NOENT) {
		toFileFd = Systemcall::getSystemcallInst()->Sys_Creat(to, 0x1FF, 1);
		k->errorCode = Kernel::NO_ERROR;
	}

	int writeByte = 0;
	if (k->errorCode == Kernel::NO_ERROR) {
		writeByte = Systemcall::getSystemcallInst()->Sys_WriteFile(toFileFd, tempBuf, fileByte, 1);
	}
	Systemcall::getSystemcallInst()->Sys_CloseFile(toFileFd);

	if (k->errorCode == Kernel::NOENT)
		cout << to << ": No such a file or directory" << endl;
	else if (k->errorCode == Kernel::NOOUTENT)
		cout << from << ": No such a file or directory" << endl;
	else if (k->errorCode == Kernel::ISDIR)
		cout << to << ": This is a directory" << endl;
	else {
		cout << "Successfully copied " << writeByte << " Byte" << endl;
	}
	return 0;
}

int Systemcall::Sys_Tree(char* path)
{
	Kernel* k = Kernel::getKernel();
	FileManager* fileMgr = k->getFileManager();
	string curDirPath = k->curdir;

	char nd[128];
	strcpy_s(nd, path);
	Systemcall::getSystemcallInst()->Sys_ChDir(nd);

	string temp = path;
	cout << "|---" << (path == "/" ? "/" : temp.substr(temp.find_last_of('/') + 1)) << endl;
	fileMgr->dirDfs(path, 0);
	strcpy_s(nd, curDirPath.c_str());
	Systemcall::getSystemcallInst()->Sys_ChDir(nd);
	return 0;
}

int Systemcall::Sys_Scp(char* mode, char* from, char* to)
{
	Kernel* k = Kernel::getKernel();
	string mod = mode;
	if (mod == "-u") {
		fstream f(from, ios::in | ios::binary);
		if (!f) {
			k->errorCode = Kernel::NOOUTENT;
		}

		if (!f.is_open()) {
			cout << "文件打开失败！请检查文件路径" << endl;
			return 1;
		}

		f.seekg(0, f.end);  /* 第一个参数是偏移量，第二个参数是基地址 */
		int length = f.tellg();  /* 返回当前定位指针的位置，也代表着输入流的大小 */
		f.seekg(0, f.beg);
		cout << "FileBytes:" << length << endl;
		char* tmpBuffer = new char[length];
		f.read(tmpBuffer, length);  //将内容读到中间变量中
		int tmpFd = this->getSystemcallInst()->Sys_OpenFile(to, 0x1FF, 0);
		if (k->errorCode == Kernel::NOENT) {
			tmpFd = Systemcall::getSystemcallInst()->Sys_Creat(to, 0x1FF, 0);
			k->errorCode = Kernel::NO_ERROR;
		}
		if (Kernel::getKernel()->errorCode == Kernel::NO_ERROR) {
			cout << Toolkit::strLength(tmpBuffer) << " " << length << endl;
			this->Sys_WriteFile(tmpFd, tmpBuffer, length, 0);
		}
		this->Sys_Close(tmpFd);
		f.close();
	}
	else if (mod == "-d") {
		fstream f(to, ios::out | ios::binary);
		int fromFileFd = Systemcall::getSystemcallInst()->Sys_OpenFile(from, 0x1FF, 0);
		int fileByte = 0;
		char* tempBuf = NULL;
		if (k->errorCode == Kernel::NO_ERROR) {
			fileByte = Systemcall::getSystemcallInst()->Sys_ReadFile(fromFileFd, INT_MAX, 0);
			tempBuf = new char[fileByte + 1];
			tempBuf[fileByte] = '\0';
			Toolkit::IOMove(k->buf, tempBuf, fileByte);
		}
		Systemcall::getSystemcallInst()->Sys_CloseFile(fromFileFd);

		if (k->errorCode == Kernel::NO_ERROR) {
			f.write(tempBuf, fileByte);
		}
		f.close();
	}

	if (k->errorCode == Kernel::NOOUTENT)
		cout << from << ": No such a file or directory" << endl;

	return 0;
}

int Systemcall::Sys_Login(char* username)
{
	Kernel* k = Kernel::getKernel();
	Systemcall* syscall = this->getSystemcallInst();
	k->refreshSystem();

	k->u_uid = k->u_gid = 0;
	syscall->Sys_ChDir((char*)"/etc");
	int fromFileFd = Systemcall::getSystemcallInst()->Sys_OpenFile((char*)"passwd", 0x1FF, 1);
	int fileByte = 0;
	char* tempBuf = NULL;
	if (k->errorCode == Kernel::NO_ERROR) {
		fileByte = Systemcall::getSystemcallInst()->Sys_ReadFile(fromFileFd, INT_MAX, 0);
		tempBuf = new char[fileByte + 1];
		tempBuf[fileByte] = '\0';
		Toolkit::IOMove(k->buf, tempBuf, fileByte);
	}
	Systemcall::getSystemcallInst()->Sys_CloseFile(fromFileFd);

	std::vector<std::string> results;
	std::stringstream ss(tempBuf); // 使用 stringstream 包装字符数组
	std::string line;

	// 使用 getline 从 stringstream 中读取直到遇到 '\n'
	while (std::getline(ss, line, '\n')) {
		if (!line.empty()) {
			results.push_back(line);
		}
	}
	string inp;
	cout << "PASSWORD:" << endl;
	cin >> inp;

	short uid = -1, gid = -1;
	for (int i = 0; i < results.size(); i += 4) {
		if (results[i] == username) {
			if (results[i + 1] != inp) {
				cout << "Pass Err" << endl;
				return 0;
			}
			else {
				uid = (short)stoi(results[i + 2]);
				gid = (short)stoi(results[i + 3]);
				break;
			}
		}
	}

	if (uid == -1) {
		cout << "NO USER NAMED" << username << endl;
	}

	cout << "User [" << username << "] login." << endl;

	syscall->Sys_ChDir((char*)"/home");

	k->u_uid = uid;
	k->u_gid = gid;

	//if (uid) {
	//	syscall->Sys_Mkdir(username, 0);
	//	syscall->Sys_ChDir(username);
	//}
	
	cout << "uid:" << k->u_gid << "gid:" << k->u_gid << endl;
	cin.ignore(std::numeric_limits< streamsize >::max(), '\n');

	return 0;
}

int Systemcall::Sys_Logout(char* username)
{
	return 0;
}


