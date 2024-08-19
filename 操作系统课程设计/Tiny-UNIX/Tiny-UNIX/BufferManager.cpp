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

		/* ��ʼ��NODEV���� */
		bp->b_back = &(this->bFreeList);
		bp->b_forw = this->bFreeList.b_forw;
		this->bFreeList.b_forw->b_back = bp;
		this->bFreeList.b_forw = bp;
		
		/* ��ʼ���豸���� */
		//bp->b_back = &(this->bDevList);
		//bp->b_forw = this->bDevList.b_forw;
		//this->bDevList.b_forw->b_back = bp;
		//this->bDevList.b_forw = bp;

		/* ��ʼ�����ɶ��� */
		bp->b_flags = Buf::B_BUSY;
		Brelse(bp);
	}

	Log::log("[SYSTM] BufferManager Initialized");
	return;
}

Buf* BufferManager::Bread(int blkno) {
	Buf* bp;
	/* �����豸�ţ��ַ�������뻺�� */
	bp = this->GetBlk(blkno);
	/* ������豸�������ҵ����軺�棬��B_DONE�����ã��Ͳ������I/O���� */
	if (bp->b_flags & Buf::B_DONE) {
		return bp;
	}
	/* û���ҵ���Ӧ���棬����I/O������� */
	bp->b_flags |= Buf::B_READ;
	bp->b_wcount = BufferManager::BUFFER_SIZE;

	/*BlockDevice.cpp ATABlockDevice::Strategy(Buf* bp)*/
	/* ��bp����I/O������еĶ�β����ʱI/O�����Ѿ��˻�����������ʽ����bp->av_forw��־�������β */
	//bp->av_forw = NULL;


	/*BlockDevice.cpp ATABlockDevice::Start()*/
	/*��ǰ������I/O����һ�����̣���ֱ��ִ�ж�ȡ*/
	fstream f(DISK_IMG, ios::in | ios::binary);
	f.seekg(blkno * BufferManager::BUFFER_SIZE);
	f.read(bp->b_addr, bp->b_wcount);
	f.close();

	///*
	// * ��I/O�����������Ӧ�豸I/O������У���������I/O����������ִ�б���I/O����
	// * ����ȴ���ǰI/O����ִ����Ϻ����жϴ����������ִ�д�����
	// * ע��Strategy()������I/O����������豸������к󣬲���I/O����ִ����ϣ���ֱ�ӷ��ء�
	// */
	//this->m_DeviceManager->GetBlockDevice(Utility::GetMajor(dev)).Strategy(bp);
	///* ͬ�������ȴ�I/O�������� */
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
	/* ע�����²�����û�����B_DELWRI��B_WRITE��B_READ��B_DONE��־
	 * B_DELWRI��ʾ��Ȼ���ÿ��ƿ��ͷŵ�����
	 ���棬�����п��ܻ�û��Щ�������ϡ�
	 * B_DONE����ָ�û����������ȷ�ط�ӳ�˴洢�ڻ�Ӧ�洢�ڴ����ϵ���Ϣ
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
	/* �������豸�Ż�ÿ��豸�� */
	//Buf* dp = &this->bDevList;
	Buf* dp = &this->bFreeList;
loop:
	if (dp == NULL)
	{
		Log::log("[ERROR] Null devtab!");
	}
	/* �����ڸ��豸�����������Ƿ�����Ӧ�Ļ��� */
	for (bp = dp->b_forw; bp != dp; bp = bp->b_forw)
	{
		/* ����Ҫ�ҵĻ��棬����� */
		if (bp->b_blkno != blkno)
			continue;

		/*
			* �ٽ���֮����Ҫ�����￪ʼ�������Ǵ������forѭ����ʼ��
			* ��Ҫ����Ϊ���жϷ�����򲢲���ȥ�޸Ŀ��豸���е�
			* �豸buf����(b_forw)�����Բ��������ͻ��
			*/
		//if (bp->b_flags & Buf::B_BUSY)
		//{
		//	bp->b_flags |= Buf::B_WANTED;
		//	goto loop;
		//}
		/* �����ɶ����г�ȡ���� */
		this->NotAvail(bp);
		return bp;
	}

	/* ������ɶ���Ϊ�� */
	if (this->bFreeList.av_forw == &this->bFreeList)
	{
		this->bFreeList.b_flags |= Buf::B_WANTED;
		goto loop;
	}

	/* ȡ���ɶ��е�һ�����п� */
	bp = this->bFreeList.av_forw;
	this->NotAvail(bp);

	/* ������ַ������ӳ�д�������첽д�������� */
	if (bp->b_flags & Buf::B_DELWRI)
	{
		bp->b_flags |= Buf::B_ASYNC;
		this->Bwrite(bp);
		goto loop;
	}
	/* ע��: �����������������λ��ֻ����B_BUSY */
	bp->b_flags = Buf::B_BUSY;

	/* ��ԭ�豸�����г�� */
	bp->b_back->b_forw = bp->b_forw;
	bp->b_forw->b_back = bp->b_back;

	/* �����µ��豸���� */
	bp->b_forw = dp->b_forw;
	bp->b_back = (Buf*)dp;
	dp->b_forw->b_back = bp;
	dp->b_forw = bp;


	bp->b_blkno = blkno;
	return bp;
}

void BufferManager::NotAvail(Buf* bp)
{
	/* �����ɶ�����ȡ�� */
	bp->av_back->av_forw = bp->av_forw;
	bp->av_forw->av_back = bp->av_back;
	/* ����B_BUSY��־ */
	bp->b_flags |= Buf::B_BUSY;
	return;
}

void BufferManager::ClrBuf(Buf* bp)
{
	int* pInt = (int*)bp->b_addr;

	/* ������������������ */
	for (int i = 0; i < BufferManager::BUFFER_SIZE / sizeof(int); i++)
	{
		pInt[i] = 0;
	}
	return;
}

void BufferManager::Bflush()
{
	Buf* bp;
	/* ע�⣺����֮����Ҫ��������һ����֮�����¿�ʼ������
	 * ��Ϊ��bwite()���뵽����������ʱ�п��жϵĲ���������
	 * �ȵ�bwriteִ����ɺ�CPU�Ѵ��ڿ��ж�״̬�����Ժ�
	 * �п��������ڼ���������жϣ�ʹ��bfreelist���г��ֱ仯��
	 * �����������������������������¿�ʼ������ô�ܿ�����
	 * ����bfreelist���е�ʱ����ִ���
	 */
loop:
	for (bp = this->bFreeList.av_forw; bp != &(this->bFreeList); bp = bp->av_forw)
	{
		/* �ҳ����ɶ����������ӳ�д�Ŀ� */
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
	/* ����B_DONE������������ʹ�øô��̿����� */
	bp->b_flags |= (Buf::B_DELWRI | Buf::B_DONE);
	this->Brelse(bp);
	return;
}

void BufferManager::Bawrite(Buf* bp) {
	/* ���Ϊ�첽д */
	bp->b_flags |= Buf::B_ASYNC;
	this->Bwrite(bp);
	return;
}

