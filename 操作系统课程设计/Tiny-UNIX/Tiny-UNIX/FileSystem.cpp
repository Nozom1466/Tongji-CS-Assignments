#include "FileSystem.h"
#include "Kernel.h"

SuperBlock::SuperBlock() {

}

SuperBlock::~SuperBlock() {

}

FileSystem::FileSystem() {

}

FileSystem::~FileSystem() {

}

void FileSystem::Initialize() {
	Log::log("[SYSTM] Initializing FileSystem...");
	//this->m_BufferManager = Kernel::getKernel()->getBufferManager();
	Log::log("[SYSTM] FileSystem Initialized.");

}

void FileSystem::LoadSuperBlock() {
	Log::log("[SYSTM] Loading SuperBlock ...");
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	SuperBlock* spb = Kernel::getKernel()->getSuperBlock();
	Buf* pBuf;

	for (int i = 0; i < 2; i++)
	{
		int* p = (int*)spb + i * 128;

		pBuf = bufMgr->Bread(FileSystem::SUPER_BLOCK_SECTOR_NUMBER + i);

		Toolkit::copy((int*)pBuf->b_addr, p, 128);

		bufMgr->Brelse(pBuf);
	}

	time_t now;
	time(&now);
	spb->s_time = now;
	/*
	//格式化输出本地时间
       printf("年：%d\n", t.tm_year + 1900);
    printf("月：%d\n", t.tm_mon + 1);
    printf("日：%d\n", t.tm_mday);
    printf("周：%d\n", t.tm_wday);
    printf("一年中：%d\n", t.tm_yday);
    printf("时：%d\n", t.tm_hour);
    printf("分：%d\n", t.tm_min);
    printf("秒：%d\n", t.tm_sec);
    printf("夏令时：%d\n", t.tm_isdst);
    system("pause");
    //getchar();
    return 0;
	*/
	Log::log("[SYSTM] SuperBlock loaded.");
}

Buf* FileSystem::Alloc() {
	int blkno;     /* 分配到的空闲磁盘块编号 */
	SuperBlock* spb = Kernel::getKernel()->getSuperBlock();
	Buf* pBuf;
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Kernel* k = Kernel::getKernel();

	/* 从索引表“栈顶”获取空闲磁盘块编号 */
	blkno = spb->s_free[--spb->s_nfree];


	/*
	 * 若获取磁盘块编号为零，则表示已分配尽所有的空闲磁盘块。
	 * 或者分配到的空闲磁盘块编号不属于数据盘块区域中(由BadBlock()检查)，
	 * 都意味着分配空闲磁盘块操作失败。
	 */
	if (blkno == 0)
	{
		spb->s_nfree = 0;
		k->errorCode = Kernel::NOSPACE;
		Log::log("[ERROR] FROM FileSystem: No space left on device!");
		return NULL;
	}

	/*
	 * 栈已空，新分配到空闲磁盘块中记录了下一组空闲磁盘块的编号,
	 * 将下一组空闲磁盘块的编号读入SuperBlock的空闲磁盘块索引表s_free[100]中。
	 */
	if (spb->s_nfree <= 0)
	{
		/* 读入该空闲磁盘块 */
		pBuf = bufMgr->Bread(blkno);

		/* 从该磁盘块的0字节开始记录，共占据4(s_nfree)+400(s_free[100])个字节 */
		int* p = (int*)pBuf->b_addr;

		/* 首先读出空闲盘块数s_nfree */
		spb->s_nfree = *p++;

		/* 读取缓存中后续位置的数据，写入到SuperBlock空闲盘块索引表s_free[100]中 */
		Toolkit::copy((int*)p, (int*)(spb->s_free), 100);

		/* 缓存使用完毕，释放以便被其它进程使用 */
		bufMgr->Brelse(pBuf);
	}

	/* 普通情况下成功分配到一空闲磁盘块(可能是记录了下一组空闲磁盘块的编号的空闲磁盘) */
	pBuf = bufMgr->GetBlk(blkno);	        /* 为该磁盘块申请缓存 */

	bufMgr->ClrBuf(pBuf);	                /* 清空缓存中的数据 */
	spb->s_fmod = 1;	                    /* 设置SuperBlock被修改标志 */

	return pBuf;
}

void FileSystem::Free(int blkno) {
	SuperBlock* sb = Kernel::getKernel()->getSuperBlock();
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Buf* pBuf;


	/*
	 * 尽早设置SuperBlock被修改标志，以防止在释放
	 * 磁盘块Free()执行过程中，对SuperBlock内存副本
	 * 的修改仅进行了一半，就更新到磁盘SuperBlock去
	 */
	sb->s_fmod = 1;

	/*
	 * 如果先前系统中已经没有空闲盘块，
	 * 现在释放的是系统中第1块空闲盘块
	 */
	if (sb->s_nfree <= 0)
	{
		sb->s_nfree = 1;
		sb->s_free[0] = 0;	/* 使用0标记空闲盘块链结束标志 */
	}

	/* SuperBlock中直接管理空闲磁盘块号的栈已满 */
	if (sb->s_nfree >= 100)
	{
		/*
		 * 使用当前Free()函数正要释放的磁盘块，存放前一组100个空闲
		 * 磁盘块的索引表
		 */
		pBuf = bufMgr->GetBlk(blkno);	/* 为当前正要释放的磁盘块分配缓存 */

		/* 从该磁盘块的0字节开始记录，共占据4(s_nfree)+400(s_free[100])个字节 */
		int* p = (int*)pBuf->b_addr;

		/* 首先写入空闲盘块数，除了第一组为99块，后续每组都是100块 */
		*p++ = sb->s_nfree;
		/* 将SuperBlock的空闲盘块索引表s_free[100]写入缓存中后续位置 */
		Toolkit::copy(sb->s_free, p, 100);

		sb->s_nfree = 0;
		/* 将存放空闲盘块索引表的“当前释放盘块”写入磁盘，即实现了空闲盘块记录空闲盘块号的目标 */
		bufMgr->Bwrite(pBuf);
	}
	sb->s_free[sb->s_nfree++] = blkno;	/* SuperBlock中记录下当前释放盘块号 */
	sb->s_fmod = 1;
}