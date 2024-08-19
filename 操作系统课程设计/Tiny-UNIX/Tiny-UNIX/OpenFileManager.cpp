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
		/* ������Inode�Ƿ����ڴ濽�� */
		int index = this->IsLoaded(inumber);
		if (index >= 0)	/* �ҵ��ڴ濽�� */
		{
			pInode = &(this->m_Inode[index]);

			/*
			 * ����ִ�е������ʾ���ڴ�Inode���ٻ������ҵ���Ӧ�ڴ�Inode��
			 * ���������ü���������ILOCK��־������֮
			 */
			pInode->i_count++;

			return pInode;
		}
		else	/* û��Inode���ڴ濽���������һ�������ڴ�Inode */
		{
			pInode = this->GetFreeInode();
			/* ���ڴ�Inode���������������Inodeʧ�� */
			if (pInode == NULL) {
				Log::log("[ERROR] Inode Table Overflow!");
				return NULL;
			}
			else	/* �������Inode�ɹ��������Inode�����·�����ڴ�Inode */
			{
				/* �����µ��豸�š����Inode��ţ��������ü������������ڵ����� */
				pInode->i_number = inumber;
				pInode->i_count++;
				//pInode->i_lastr = -1;

				/* �������Inode���뻺���� */
				Buf* pBuf = bufMgr->Bread(FileSystem::INODE_ZONE_START_SECTOR + inumber / FileSystem::INODE_NUMBER_PER_SECTOR);

				/* �������I/O���� */
				if (pBuf->b_flags & Buf::B_ERROR)
				{
					/* �ͷŻ��� */
					bufMgr->Brelse(pBuf);
					/* �ͷ�ռ�ݵ��ڴ�Inode */
					this->IPut(pInode);
					Log::log("[ERROR] FROM IGet: I/O Error.");
					return NULL;
				}

				/* ���������е����Inode��Ϣ�������·�����ڴ�Inode�� */
				pInode->ICopy(pBuf, inumber);
				/* �ͷŻ��� */
				bufMgr->Brelse(pBuf);
				return pInode;
			}
		}
	}

	return nullptr;
}

void InodeTable::IPut(Inode* pNode) {
	/* ��ǰ����Ϊ���ø��ڴ�Inode��Ψһ���̣���׼���ͷŸ��ڴ�Inode */
	if (pNode->i_count == 1)
	{
		/*
		 * ��������Ϊ�������ͷŹ����п�����Ϊ���̲�����ʹ�øý���˯�ߣ�
		 * ��ʱ�п�����һ�����̻�Ը��ڴ�Inode���в������⽫�п��ܵ��´���
		 */
		pNode->i_flag |= Inode::ILOCK;

		/* ���ļ��Ѿ�û��Ŀ¼·��ָ���� */
		if (pNode->i_nlink <= 0)
		{
			/* �ͷŸ��ļ�ռ�ݵ������̿� */
			pNode->ITrunc();
			pNode->i_mode = 0;
			/* �ͷŶ�Ӧ�����Inode */
			Kernel::getKernel()->getFileSystem()->IFree(pNode->i_number);
		}

		/* �������Inode��Ϣ */
		time_t now;
		time(&now);
		pNode->IUpdate(now);

		/* ����ڴ�Inode�����б�־λ */
		pNode->i_flag = 0;
		/* �����ڴ�inode���еı�־֮һ����һ����i_count == 0 */
		pNode->i_number = -1;
	}

	/* �����ڴ�Inode�����ü��������ѵȴ����� */
	pNode->i_count--;
}

void InodeTable::UpdateInodeTable()
{
	for (int i = 0; i < InodeTable::NINODE; i++)
	{
		/* count������0��count == 0��ζ�Ÿ��ڴ�Inodeδ���κδ��ļ����ã�����ͬ�� */
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

	/* �����SuperBlock�ڴ渱��û�б��޸ģ�ֱ�ӹ���inode�Ϳ����̿鱻��������ļ�ϵͳ��ֻ���ļ�ϵͳ */
	if (spb->s_fmod == 0) {
		return;;
	}

	/* ��SuperBlock�޸ı�־ */
	spb->s_fmod = 0;
	/* д��SuperBlock�����ʱ�� */
	time_t now;
	time(&now);
	spb->s_time = now;

	/*
	 * Ϊ��Ҫд�ص�������ȥ��SuperBlock����һ�黺�棬���ڻ�����СΪ512�ֽڣ�
	 * SuperBlock��СΪ1024�ֽڣ�ռ��2��������������������Ҫ2��д�������
	 */
	for (int j = 0; j < 2; j++)
	{
		/* ��һ��pָ��SuperBlock�ĵ�0�ֽڣ��ڶ���pָ���512�ֽ� */
		int* p = (int*)spb + j * 128;

		/* ��Ҫд�뵽�豸dev�ϵ�SUPER_BLOCK_SECTOR_NUMBER + j������ȥ */
		pBuf = bufMgr->GetBlk(FileSystem::SUPER_BLOCK_SECTOR_NUMBER + j);

		/* ��SuperBlock�е�0 - 511�ֽ�д�뻺���� */
		Toolkit::copy(p, (int*)pBuf->b_addr, 128);

		/* ���������е�����д�������� */
		bufMgr->Bwrite(pBuf);
	}

	/* ͬ���޸Ĺ����ڴ�Inode����Ӧ���Inode */
	InodeTable* k_InodeTable = Kernel::getKernel()->getInodeTable();
	k_InodeTable->UpdateInodeTable();

	/* ���ӳ�д�Ļ����д�������� */
	bufMgr->Bflush();
}

Inode* FileSystem::IAlloc()
{
	
	Buf* pBuf;
	Inode* pNode;
	int ino;	/* ���䵽�Ŀ������Inode��� */

	BufferManager* bufMgr = Kernel::getKernel()->getBufferManager();
	Kernel* k = Kernel::getKernel();
	InodeTable* k_InodeTable = Kernel::getKernel()->getInodeTable();

	SuperBlock* sb = k->getSuperBlock();

	/*
	 * SuperBlockֱ�ӹ���Ŀ���Inode�������ѿգ�
	 * ���뵽��������������Inode���ȶ�inode�б�������
	 * ��Ϊ�����³����л���ж��̲������ܻᵼ�½����л���
	 * ���������п��ܷ��ʸ����������ᵼ�²�һ���ԡ�
	 */
	if (sb->s_ninode <= 0)
	{

		/* ���Inode��Ŵ�0��ʼ���ⲻͬ��Unix V6�����Inode��1��ʼ��� */
		ino = -1;

		/* ���ζ������Inode���еĴ��̿飬�������п������Inode���������Inode������ */
		for (int i = 0; i < sb->s_isize; i++)
		{
			pBuf = bufMgr->Bread(FileSystem::INODE_ZONE_START_SECTOR + i);

			/* ��ȡ��������ַ */
			int* p = (int*)pBuf->b_addr;

			/* ���û�������ÿ�����Inode��i_mode != 0����ʾ�Ѿ���ռ�� */
			for (int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; j++)
			{
				ino++;

				int mode = *(p + j * sizeof(DiskInode) / sizeof(int));

				/* �����Inode�ѱ�ռ�ã����ܼ������Inode������ */
				if (mode != 0)
				{
					continue;
				}

				/*
				 * ������inode��i_mode==0����ʱ������ȷ��
				 * ��inode�ǿ��еģ���Ϊ�п������ڴ�inodeû��д��
				 * ������,����Ҫ���������ڴ�inode���Ƿ�����Ӧ����
				 */
				if (k_InodeTable->IsLoaded(ino) == -1)
				{
					/* �����Inodeû�ж�Ӧ���ڴ濽��������������Inode������ */
					sb->s_inode[sb->s_ninode++] = ino;

					/* ��������������Ѿ�װ�����򲻼������� */
					if (sb->s_ninode >= 100)
					{
						break;
					}
				}
			}

			/* �����Ѷ��굱ǰ���̿飬�ͷ���Ӧ�Ļ��� */
			bufMgr->Brelse(pBuf);

			/* ��������������Ѿ�װ�����򲻼������� */
			if (sb->s_ninode >= 100)
			{
				break;
			}
		}

		/* ����ڴ�����û���������κο������Inode������NULL */
		if (sb->s_ninode <= 0)
		{
			Log::log("[ERROR] FROM Creat: No Space left on device!");
			return NULL;
		}
	}

	/*
	 * ���沿���Ѿ���֤������ϵͳ��û�п������Inode��
	 * �������Inode�������бض����¼�������Inode�ı�š�
	 */
	while (true)
	{
		/* ��������ջ������ȡ�������Inode��� */
		ino = sb->s_inode[--sb->s_ninode];

		/* ������Inode�����ڴ� */
		pNode = k_InodeTable->IGet(ino);
		/* δ�ܷ��䵽�ڴ�inode */
		if (NULL == pNode)
		{
			return NULL;
		}

		/* �����Inode����,���Inode�е����� */
		if (0 == pNode->i_mode)
		{
			pNode->Clean();
			/* ����SuperBlock���޸ı�־ */
			sb->s_fmod = 1;
			return pNode;
		}
		else	/* �����Inode�ѱ�ռ�� */
		{
			k_InodeTable->IPut(pNode);
			continue;	/* whileѭ�� */
		}
	}
	return NULL;	/* GCC likes it! */
}

void FileSystem::IFree(int number) {
	SuperBlock* sb = Kernel::getKernel()->getSuperBlock();


	/*
	 * ���������ֱ�ӹ���Ŀ������Inode����100����
	 * ͬ�����ͷŵ����Inodeɢ���ڴ���Inode���С�
	 */
	if (sb->s_ninode >= 100)
	{
		return;
	}

	sb->s_inode[sb->s_ninode++] = number;

	/* ����SuperBlock���޸ı�־ */
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
		/* ������ڴ�Inode���ü���Ϊ�㣬���Inode��ʾ���� */
		if (this->m_Inode[i].i_count == 0) {
			return &(this->m_Inode[i]);
		}
	}
	return NULL;	/* Ѱ��ʧ�� */
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

	/* ���ں˴��ļ����������л�ȡһ�������� */
	fd = k->getKernelOpenFiles()->AllocFreeSlot();

	if (fd < 0)  /* ���Ѱ�ҿ�����ʧ�� */
	{
		return NULL;
	}

	for (int i = 0; i < OpenFileTable::NFILE; i++)
	{
		/* f_count==0��ʾ������� */
		if (this->m_File[i].f_count == 0)
		{
			/* ������������File�ṹ�Ĺ�����ϵ */
			k->getKernelOpenFiles()->SetF(fd, &this->m_File[i]);
			/* ���Ӷ�file�ṹ�����ü��� */
			this->m_File[i].f_count++;
			/* ����ļ�����дλ�� */
			this->m_File[i].f_offset = 0;
			return (&this->m_File[i]);
		}
	}
	cout << "NOTFOUND" << endl;
	return NULL;
}

void OpenFileTable::CloseF(File* pFile)
{
	/* ���ü���f_count����Ϊ0�����ͷ�File�ṹ */
	if (pFile->f_count <= 1)
	{
		Kernel::getKernel()->getInodeTable()->IPut(pFile->f_inode);
	}


	/* ���õ�ǰFile�Ľ�������1 */
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
		/* ���̴��ļ������������ҵ�������򷵻�֮ */
		if (this->k_OpenFileTable[i] == NULL)
		{
			/* ϵͳ���÷���ֵ */
			Kernel::getKernel()->systemcallRet = i;
			return i;
		}
	}
	Kernel::getKernel()->systemcallRet = -1;   /* Open1����Ҫһ����־�������ļ��ṹ����ʧ��ʱ�����Ի���ϵͳ��Դ*/
	return -1;
}

File* OpenFiles::GetF(int fd)
{
	File* pFile;
	Kernel* k = Kernel::getKernel();

	/* ������ļ���������ֵ�����˷�Χ */
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
	return pFile;   /* ��ʹpFile==NULLҲ���������ɵ���GetF�ĺ������жϷ���ֵ */
}

void OpenFiles::SetF(int fd, File* pFile) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
		return;
	/* ���̴��ļ�������ָ��ϵͳ���ļ�������Ӧ��File�ṹ */
	this->k_OpenFileTable[fd] = pFile;
}
