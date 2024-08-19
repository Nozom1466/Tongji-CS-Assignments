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
	//��ʽ���������ʱ��
       printf("�꣺%d\n", t.tm_year + 1900);
    printf("�£�%d\n", t.tm_mon + 1);
    printf("�գ�%d\n", t.tm_mday);
    printf("�ܣ�%d\n", t.tm_wday);
    printf("һ���У�%d\n", t.tm_yday);
    printf("ʱ��%d\n", t.tm_hour);
    printf("�֣�%d\n", t.tm_min);
    printf("�룺%d\n", t.tm_sec);
    printf("����ʱ��%d\n", t.tm_isdst);
    system("pause");
    //getchar();
    return 0;
	*/
	Log::log("[SYSTM] SuperBlock loaded.");
}

Buf* FileSystem::Alloc() {
	int blkno;     /* ���䵽�Ŀ��д��̿��� */
	SuperBlock* spb = Kernel::getKernel()->getSuperBlock();
	Buf* pBuf;
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Kernel* k = Kernel::getKernel();

	/* ��������ջ������ȡ���д��̿��� */
	blkno = spb->s_free[--spb->s_nfree];


	/*
	 * ����ȡ���̿���Ϊ�㣬���ʾ�ѷ��価���еĿ��д��̿顣
	 * ���߷��䵽�Ŀ��д��̿��Ų����������̿�������(��BadBlock()���)��
	 * ����ζ�ŷ�����д��̿����ʧ�ܡ�
	 */
	if (blkno == 0)
	{
		spb->s_nfree = 0;
		k->errorCode = Kernel::NOSPACE;
		Log::log("[ERROR] FROM FileSystem: No space left on device!");
		return NULL;
	}

	/*
	 * ջ�ѿգ��·��䵽���д��̿��м�¼����һ����д��̿�ı��,
	 * ����һ����д��̿�ı�Ŷ���SuperBlock�Ŀ��д��̿�������s_free[100]�С�
	 */
	if (spb->s_nfree <= 0)
	{
		/* ����ÿ��д��̿� */
		pBuf = bufMgr->Bread(blkno);

		/* �Ӹô��̿��0�ֽڿ�ʼ��¼����ռ��4(s_nfree)+400(s_free[100])���ֽ� */
		int* p = (int*)pBuf->b_addr;

		/* ���ȶ��������̿���s_nfree */
		spb->s_nfree = *p++;

		/* ��ȡ�����к���λ�õ����ݣ�д�뵽SuperBlock�����̿�������s_free[100]�� */
		Toolkit::copy((int*)p, (int*)(spb->s_free), 100);

		/* ����ʹ����ϣ��ͷ��Ա㱻��������ʹ�� */
		bufMgr->Brelse(pBuf);
	}

	/* ��ͨ����³ɹ����䵽һ���д��̿�(�����Ǽ�¼����һ����д��̿�ı�ŵĿ��д���) */
	pBuf = bufMgr->GetBlk(blkno);	        /* Ϊ�ô��̿����뻺�� */

	bufMgr->ClrBuf(pBuf);	                /* ��ջ����е����� */
	spb->s_fmod = 1;	                    /* ����SuperBlock���޸ı�־ */

	return pBuf;
}

void FileSystem::Free(int blkno) {
	SuperBlock* sb = Kernel::getKernel()->getSuperBlock();
	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Buf* pBuf;


	/*
	 * ��������SuperBlock���޸ı�־���Է�ֹ���ͷ�
	 * ���̿�Free()ִ�й����У���SuperBlock�ڴ渱��
	 * ���޸Ľ�������һ�룬�͸��µ�����SuperBlockȥ
	 */
	sb->s_fmod = 1;

	/*
	 * �����ǰϵͳ���Ѿ�û�п����̿飬
	 * �����ͷŵ���ϵͳ�е�1������̿�
	 */
	if (sb->s_nfree <= 0)
	{
		sb->s_nfree = 1;
		sb->s_free[0] = 0;	/* ʹ��0��ǿ����̿���������־ */
	}

	/* SuperBlock��ֱ�ӹ�����д��̿�ŵ�ջ���� */
	if (sb->s_nfree >= 100)
	{
		/*
		 * ʹ�õ�ǰFree()������Ҫ�ͷŵĴ��̿飬���ǰһ��100������
		 * ���̿��������
		 */
		pBuf = bufMgr->GetBlk(blkno);	/* Ϊ��ǰ��Ҫ�ͷŵĴ��̿���仺�� */

		/* �Ӹô��̿��0�ֽڿ�ʼ��¼����ռ��4(s_nfree)+400(s_free[100])���ֽ� */
		int* p = (int*)pBuf->b_addr;

		/* ����д������̿��������˵�һ��Ϊ99�飬����ÿ�鶼��100�� */
		*p++ = sb->s_nfree;
		/* ��SuperBlock�Ŀ����̿�������s_free[100]д�뻺���к���λ�� */
		Toolkit::copy(sb->s_free, p, 100);

		sb->s_nfree = 0;
		/* ����ſ����̿�������ġ���ǰ�ͷ��̿顱д����̣���ʵ���˿����̿��¼�����̿�ŵ�Ŀ�� */
		bufMgr->Bwrite(pBuf);
	}
	sb->s_free[sb->s_nfree++] = blkno;	/* SuperBlock�м�¼�µ�ǰ�ͷ��̿�� */
	sb->s_fmod = 1;
}