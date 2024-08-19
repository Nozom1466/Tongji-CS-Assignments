#include "Kernel.h"

Kernel::Kernel() {
	this->BufMgr = new BufferManager;
	this->fileSys = new FileSystem;
	this->fileMgr = new FileManager;
	this->k_InodeTable = new InodeTable;
	this->s_openFiles = new OpenFileTable;
	this->k_openFiles = new OpenFiles;
	this->spb = new SuperBlock;
}

Kernel::~Kernel() {
	delete this->BufMgr;
	delete this->fileSys;
	delete this->fileMgr;
	delete this->k_InodeTable;
	delete this->s_openFiles;
	delete this->k_openFiles;
	delete this->spb;
}

Kernel* Kernel::getKernel() {
	return &inst;
}

BufferManager* Kernel::getBufferManager() {
	return BufMgr;
}

FileSystem* Kernel::getFileSystem() {
	return fileSys;
}

FileManager* Kernel::getFileManager() {
	return fileMgr;
}

InodeTable* Kernel::getInodeTable() {
	return k_InodeTable;
}

OpenFileTable* Kernel::getOpenFileTable() {
	return s_openFiles;
}

OpenFiles* Kernel::getKernelOpenFiles() {
	return k_openFiles;
}

SuperBlock* Kernel::getSuperBlock() {
	return spb;
}

void Kernel::init(Systemcall* syscall) {
	if (!this->isformat) {
		fstream outfile;
		outfile.open(LOG_PATH, ios::out);
		outfile.close();
	}
	
	Log::log("[SYSTM] Initializing kernel ...");

	this->errorCode = Kernel::NO_ERROR;
	this->initBuffer();
	this->initFileSystem();
	Kernel::getKernel()->getFileSystem()->LoadSuperBlock();

	this->fileMgr->rootDirInode = this->k_InodeTable->IGet(FileSystem::ROOTINO); //获得根目录内存INode
	this->cdir = this->fileMgr->rootDirInode;      //给指向当前目录(根目录)的指针赋值
	Toolkit::stringCopy("/", this->curdir);
	
	Log::log("[SYSTM] Kernel Initialized.");
}

/*
cd etc
fopen passwd
fread 0 1000


*/


void Kernel::initBuffer() {
	this->BufMgr->Initialize();
}

void Kernel::initFileSystem() {
	this->fileSys->Initialize();
	this->fileMgr->Initialize();
}

void Kernel::refreshSystem() {
	this->fileMgr->rootDirInode = this->k_InodeTable->IGet(FileSystem::ROOTINO);
	this->systemcallRet = -1;
	this->errorCode = NO_ERROR;           //清标志位
}
