#include "BufferManager.h"
#include "Kernel.h"

BufferManager::BufferManager() {
	
}

BufferManager::~BufferManager() {
	Bflush();
}

void BufferManager::Initialize() {
	Log::log("[SYSTM] Initailizing BufferManager ...");
	this->bFreeList.b_forw = this->bFreeList.b_back = &(this->bFreeList);
	this->bFreeList.av_forw = this->bFreeList.av_back = &(this->bFreeList);

	//this->bDevList.b_forw = this->bDevList.b_back = &(this->bDevList);
	//this->bDevList.av_forw = this->bDevList.av_back = &(this->bDevList);

	int i;
	Buf* bp;

	for (i = 0; i < NBUF; i++)
	{
		bp = &(this->m_Buf[i]);
		bp->b_addr = this->Buffer[i];

		/* 初始化NODEV队列 */
		bp->b_back = &(this->bFreeList);
		bp->b_forw = this->bFreeList.b_forw;
		this->bFreeList.b_forw->b_back = bp;
		this->bFreeList.b_forw = bp;
		
		/* 初始化设备队列 */
		//bp->b_back = &(this->bDevList);
		//bp->b_forw = this->bDevList.b_forw;
		//this->bDevList.b_forw->b_back = bp;
		//this->bDevList.b_forw = bp;

		/* 初始化自由队列 */
		bp->b_flags = Buf::B_BUSY;
		Brelse(bp);
	}

	Log::log("[SYSTM] BufferManager Initialized");
	return;
}

Buf* BufferManager::Bread(int blkno) {
	Buf* bp;
	/* 根据设备号，字符块号申请缓存 */
	bp = this->GetBlk(blkno);
	/* 如果在设备队列中找到所需缓存，即B_DONE已设置，就不需进行I/O操作 */
	if (bp->b_flags & Buf::B_DONE) {
		return bp;
	}
	/* 没有找到相应缓存，构成I/O读请求块 */
	bp->b_flags |= Buf::B_READ;
	bp->b_wcount = BufferManager::BUFFER_SIZE;

	/*BlockDevice.cpp ATABlockDevice::Strategy(Buf* bp)*/
	/* 将bp加入I/O请求队列的队尾，此时I/O队列已经退化到单链表形式，将bp->av_forw标志着链表结尾 */
	//bp->av_forw = NULL;


	/*BlockDevice.cpp ATABlockDevice::Start()*/
	/*当前无其他I/O（就一个进程），直接执行读取*/
	fstream f(DISK_IMG, ios::in | ios::binary);
	f.seekg(blkno * BufferManager::BUFFER_SIZE);
	f.read(bp->b_addr, bp->b_wcount);
	f.close();

	///*
	// * 将I/O请求块送入相应设备I/O请求队列，如无其它I/O请求，则将立即执行本次I/O请求；
	// * 否则等待当前I/O请求执行完毕后，由中断处理程序启动执行此请求。
	// * 注：Strategy()函数将I/O请求块送入设备请求队列后，不等I/O操作执行完毕，就直接返回。
	// */
	//this->m_DeviceManager->GetBlockDevice(Utility::GetMajor(dev)).Strategy(bp);
	///* 同步读，等待I/O操作结束 */
	//this->IOWait(bp);
	return bp;
}

void BufferManager::Bwrite(Buf* bp)
{
	bp->b_flags &= ~(Buf::B_READ | Buf::B_DONE | Buf::B_ERROR | Buf::B_DELWRI);
	bp->b_wcount = BufferManager::BUFFER_SIZE;

	// WRITEEEEEEEEEEEEEEEEEEEEEEEEEEEE
	fstream f(DISK_IMG, ios::in | ios::out | ios::binary);
	f.seekp(bp->b_blkno * BufferManager::BUFFER_SIZE);
	f.write(bp->b_addr, bp->b_wcount);
	//cout << "WRITINGGGG..." << " " << bp->b_addr << endl;
	f.close();
	this->Brelse(bp);

	//writeArg* arg = new writeArg(this, bp);
	//writing(arg);
	//delete arg;
	return;
}

void BufferManager::Brelse(Buf* bp) {
	/* 注意以下操作并没有清除B_DELWRI、B_WRITE、B_READ、B_DONE标志
	 * B_DELWRI表示虽然将该控制块释放到自由
	 里面，但是有可能还没有些到磁盘上。
	 * B_DONE则是指该缓存的内容正确地反映了存储在或应存储在磁盘上的信息
	 */
	bp->b_flags &= ~(Buf::B_WANTED | Buf::B_BUSY | Buf::B_ASYNC);
	(this->bFreeList.av_back)->av_forw = bp;
	bp->av_back = this->bFreeList.av_back;
	bp->av_forw = &(this->bFreeList);
	this->bFreeList.av_back = bp;
	return;
}

Buf* BufferManager::GetBlk(int blkno) {
	Buf* bp;
	/* 根据主设备号获得块设备表 */
	//Buf* dp = &this->bDevList;
	Buf* dp = &this->bFreeList;
loop:
	if (dp == NULL)
	{
		Log::log("[ERROR] Null devtab!");
	}
	/* 首先在该设备队列中搜索是否有相应的缓存 */
	for (bp = dp->b_forw; bp != dp; bp = bp->b_forw)
	{
		/* 不是要找的缓存，则继续 */
		if (bp->b_blkno != blkno)
			continue;

		/*
			* 临界区之所以要从这里开始，而不是从上面的for循环开始。
			* 主要是因为，中断服务程序并不会去修改块设备表中的
			* 设备buf队列(b_forw)，所以不会引起冲突。
			*/
		//if (bp->b_flags & Buf::B_BUSY)
		//{
		//	bp->b_flags |= Buf::B_WANTED;
		//	goto loop;
		//}
		/* 从自由队列中抽取出来 */
		this->NotAvail(bp);
		return bp;
	}

	/* 如果自由队列为空 */
	if (this->bFreeList.av_forw == &this->bFreeList)
	{
		this->bFreeList.b_flags |= Buf::B_WANTED;
		goto loop;
	}

	/* 取自由队列第一个空闲块 */
	bp = this->bFreeList.av_forw;
	this->NotAvail(bp);

	/* 如果该字符块是延迟写，将其异步写到磁盘上 */
	if (bp->b_flags & Buf::B_DELWRI)
	{
		bp->b_flags |= Buf::B_ASYNC;
		this->Bwrite(bp);
		goto loop;
	}
	/* 注意: 这里清除了所有其他位，只设了B_BUSY */
	bp->b_flags = Buf::B_BUSY;

	/* 从原设备队列中抽出 */
	bp->b_back->b_forw = bp->b_forw;
	bp->b_forw->b_back = bp->b_back;

	/* 加入新的设备队列 */
	bp->b_forw = dp->b_forw;
	bp->b_back = (Buf*)dp;
	dp->b_forw->b_back = bp;
	dp->b_forw = bp;


	bp->b_blkno = blkno;
	return bp;
}

void BufferManager::NotAvail(Buf* bp)
{
	/* 从自由队列中取出 */
	bp->av_back->av_forw = bp->av_forw;
	bp->av_forw->av_back = bp->av_back;
	/* 设置B_BUSY标志 */
	bp->b_flags |= Buf::B_BUSY;
	return;
}

void BufferManager::ClrBuf(Buf* bp)
{
	int* pInt = (int*)bp->b_addr;

	/* 将缓冲区中数据清零 */
	for (int i = 0; i < BufferManager::BUFFER_SIZE / sizeof(int); i++)
	{
		pInt[i] = 0;
	}
	return;
}

void BufferManager::Bflush()
{
	Buf* bp;
	/* 注意：这里之所以要在搜索到一个块之后重新开始搜索，
	 * 因为在bwite()进入到驱动程序中时有开中断的操作，所以
	 * 等到bwrite执行完成后，CPU已处于开中断状态，所以很
	 * 有可能在这期间产生磁盘中断，使得bfreelist队列出现变化，
	 * 如果这里继续往下搜索，而不是重新开始搜索那么很可能在
	 * 操作bfreelist队列的时候出现错误。
	 */
loop:
	for (bp = this->bFreeList.av_forw; bp != &(this->bFreeList); bp = bp->av_forw)
	{
		/* 找出自由队列中所有延迟写的块 */
		if (bp->b_flags & Buf::B_DELWRI)
		{
			this->NotAvail(bp);
			this->Bwrite(bp);
			goto loop;
		}
	}
	return;
}

void BufferManager::Bdwrite(Buf* bp) {
	/* 置上B_DONE允许其它进程使用该磁盘块内容 */
	bp->b_flags |= (Buf::B_DELWRI | Buf::B_DONE);
	this->Brelse(bp);
	return;
}

void BufferManager::Bawrite(Buf* bp) {
	/* 标记为异步写 */
	bp->b_flags |= Buf::B_ASYNC;
	this->Bwrite(bp);
	return;
}

