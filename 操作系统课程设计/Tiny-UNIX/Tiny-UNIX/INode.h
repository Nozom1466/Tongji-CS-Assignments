#pragma once
#ifndef INODE_H
#define INODE_H

#include "Buf.h"
#include <ctime>

/*
 * �ڴ������ڵ�(INode)�Ķ���
 * ϵͳ��ÿһ���򿪵��ļ�����ǰ����Ŀ¼��
 * ���ص����ļ�ϵͳ����ӦΨһ���ڴ�inode��
 * ÿ���ڴ�inodeͨ�����inode���ڴ洢�豸���豸��(i_dev)
 * �Լ����豸���inode���еı��(i_number)��ȷ��
 * ���Ӧ�����inode��
 */
class Inode
{
public:
	/* i_flag�б�־λ */
	enum INodeFlag
	{
		ILOCK = 0x1,		/* �����ڵ����� */
		IUPD = 0x2,		/* �ڴ�inode���޸Ĺ�����Ҫ������Ӧ���inode */
		IACC = 0x4,		/* �ڴ�inode�����ʹ�����Ҫ�޸����һ�η���ʱ�� */
		IMOUNT = 0x8,		/* �ڴ�inode���ڹ������ļ�ϵͳ */
		IWANT = 0x10,		/* �н������ڵȴ����ڴ�inode����������ILOCK��־ʱ��Ҫ�������ֽ��� */
		ITEXT = 0x20		/* �ڴ�inode��Ӧ����ͼ������Ķ� */
	};

	/* static const member */
	static const unsigned int IALLOC = 0x8000;		/* �ļ���ʹ�� */
	static const unsigned int IFMT = 0x6000;		/* �ļ��������� */
	static const unsigned int IFDIR = 0x4000;		/* �ļ����ͣ�Ŀ¼�ļ� */
	static const unsigned int IFCHR = 0x2000;		/* �ַ��豸���������ļ� */
	static const unsigned int IFBLK = 0x6000;		/* ���豸���������ļ���Ϊ0��ʾ���������ļ� */
	static const unsigned int ILARG = 0x1000;		/* �ļ��������ͣ����ͻ�����ļ� */
	static const unsigned int ISUID = 0x800;		/* ִ��ʱ�ļ�ʱ���û�����Ч�û�ID�޸�Ϊ�ļ������ߵ�User ID */
	static const unsigned int ISGID = 0x400;		/* ִ��ʱ�ļ�ʱ���û�����Ч��ID�޸�Ϊ�ļ������ߵ�Group ID */
	static const unsigned int ISVTX = 0x200;		/* ʹ�ú���Ȼλ�ڽ������ϵ����Ķ� */
	static const unsigned int IREAD = 0x100;		/* ���ļ��Ķ�Ȩ�� */
	static const unsigned int IWRITE = 0x80;		/* ���ļ���дȨ�� */
	static const unsigned int IEXEC = 0x40;			/* ���ļ���ִ��Ȩ�� */
	static const unsigned int IRWXU = (IREAD | IWRITE | IEXEC);		/* �ļ������ļ��Ķ���д��ִ��Ȩ�� */
	static const unsigned int IRWXG = ((IRWXU) >> 3);			/* �ļ���ͬ���û����ļ��Ķ���д��ִ��Ȩ�� */
	static const unsigned int IRWXO = ((IRWXU) >> 6);			/* �����û����ļ��Ķ���д��ִ��Ȩ�� */

	static const int BLOCK_SIZE = 512;		/* �ļ��߼����С: 512�ֽ� */
	static const int ADDRESS_PER_INDEX_BLOCK = BLOCK_SIZE / sizeof(int);	/* ÿ�����������(��������)�����������̿�� */

	static const int SMALL_FILE_BLOCK = 6;	/* С���ļ���ֱ������������Ѱַ���߼���� */
	static const int LARGE_FILE_BLOCK = 128 * 2 + 6;	/* �����ļ�����һ�μ������������Ѱַ���߼���� */
	static const int HUGE_FILE_BLOCK = 128 * 128 * 2 + 128 * 2 + 6;	/* �����ļ��������μ����������Ѱַ�ļ��߼���� */

	static const int PIPSIZ = SMALL_FILE_BLOCK * BLOCK_SIZE;

	/* static member */
	static int rablock;		/* ˳���ʱ��ʹ��Ԥ�����������ļ�����һ�ַ��飬rablock��¼����һ�߼����
							����bmapת���õ��������̿�š���rablock��Ϊ��̬������ԭ�򣺵���һ��bmap�Ŀ���
							�Ե�ǰ���Ԥ������߼���Ž���ת����bmap���ص�ǰ��������̿�ţ����ҽ�Ԥ����
							�������̿�ű�����rablock�С� */

							/* Functions */
public:
	/* Constructors */
	Inode();
	/* Destructors */
	~Inode();

	/*
	 * @comment ����Inode�����е�������̿���������ȡ��Ӧ
	 * ���ļ�����
	 */
	void ReadI();
	/*
	 * @comment ����Inode�����е�������̿�������������д���ļ�
	 */
	void WriteI();
	/*
	 * @comment ���ļ����߼����ת���ɶ�Ӧ�������̿��
	 */
	int Bmap(int lbn);

	/*
	 * @comment �������ַ��豸�����豸�ļ������ø��豸ע���ڿ��豸���ر�
	 * �е��豸��ʼ������
	 */
	void OpenI(int mode);
	/*
	 * @comment �������ַ��豸�����豸�ļ�������Ը��豸�����ü���Ϊ0��
	 * ����ø��豸�Ĺرճ���
	 */
	void CloseI(int mode);

	/*
	 * @comment �������Inode�����ķ���ʱ�䡢�޸�ʱ��
	 */
	void IUpdate(time_t time);
	/*
	 * @comment �ͷ�Inode��Ӧ�ļ�ռ�õĴ��̿�
	 */
	void ITrunc();

	/*
	 * @comment ��Pipe���������Pipe�Ѿ���������������IWANT��־��˯�ߵȴ�ֱ������
	 */
	void Plock();

	/*
	 * @comment ��Pipe����Inode���������һ�����ȴ�����˯�ߵĽ���
	 */
	void NFrele();

	/*
	 * @comment ��Pipe���������Pipe�Ѿ���������������IWANT��־��˯�ߵȴ�ֱ������
	 */
	void NFlock();

	/*
	 * @comment ���Inode�����е�����
	 */
	void Clean();
	/*
	 * @comment ���������Inode�ַ�������Ϣ�������ڴ�Inode��
	 */
	void ICopy(Buf* bp, int inumber);

	/* Members */
public:
	unsigned int i_flag;	/* ״̬�ı�־λ�������enum INodeFlag */
	unsigned int i_mode;	/* �ļ�������ʽ��Ϣ */

	int		i_count;		/* ���ü��� */
	int		i_nlink;		/* �ļ���������������ļ���Ŀ¼���в�ͬ·���������� */

	short	i_dev;			/* ���inode���ڴ洢�豸���豸�� */
	int		i_number;		/* ���inode���еı�� */

	short	i_uid;			/* �ļ������ߵ��û���ʶ�� */
	short	i_gid;			/* �ļ������ߵ����ʶ�� */

	int		i_size;			/* �ļ���С���ֽ�Ϊ��λ */
	int		i_addr[10];		/* �����ļ��߼���ú�������ת���Ļ��������� */

	int		i_lastr;		/* ������һ�ζ�ȡ�ļ����߼���ţ������ж��Ƿ���ҪԤ�� */
};


/*
 * ��������ڵ�(DiskINode)�Ķ���
 * ���Inodeλ���ļ��洢�豸�ϵ�
 * ���Inode���С�ÿ���ļ���Ψһ��Ӧ
 * �����Inode���������Ǽ�¼�˸��ļ�
 * ��Ӧ�Ŀ�����Ϣ��
 * ���Inode������ֶκ��ڴ�Inode���ֶ�
 * ���Ӧ�����INode���󳤶�Ϊ64�ֽڣ�
 * ÿ�����̿���Դ��512/64 = 8�����Inode
 */
class DiskInode
{
	/* Functions */
public:
	/* Constructors */
	DiskInode();
	/* Destructors */
	~DiskInode();

	/* Members */
public:
	unsigned int d_mode;	/* ״̬�ı�־λ�������enum INodeFlag */
	int		d_nlink;		/* �ļ���������������ļ���Ŀ¼���в�ͬ·���������� */

	int		d_size;			/* �ļ���С���ֽ�Ϊ��λ */
	int		d_addr[10];		/* �����ļ��߼���ú�������ת���Ļ��������� */

	int		d_atime;		/* ������ʱ�� */
	int		d_mtime;		/* ����޸�ʱ�� */	
	short	d_uid;
	short	d_gid;
	//int		padding[1];		/* Padding */
};

#endif
