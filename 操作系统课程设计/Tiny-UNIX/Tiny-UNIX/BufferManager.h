#pragma once

#include "Buf.h"
#include "Log.h"


class BufferManager
{
public:
	/* static const member */
	static const int NBUF = 15;			/* ������ƿ顢������������ */
	static const int BUFFER_SIZE = 512;	/* ��������С�� ���ֽ�Ϊ��λ */

public:
	BufferManager();
	~BufferManager();

	void Initialize();					/* ������ƿ���еĳ�ʼ������������ƿ���b_addrָ����Ӧ�������׵�ַ��*/

	Buf* GetBlk(int blkno);	/* ����һ�黺�棬���ڶ�д�豸dev�ϵ��ַ���blkno��*/
	void Brelse(Buf* bp);				/* �ͷŻ�����ƿ�buf */
	void IOWait(Buf* bp);				/* ͬ����ʽI/O���ȴ�I/O�������� */
	void IODone(Buf* bp);				/* I/O���������ƺ��� */

	Buf* Bread(int blkno);	/* ��һ�����̿顣devΪ�������豸�ţ�blknoΪĿ����̿��߼���š� */
	Buf* Breada(short adev, int blkno, int rablkno);	/* ��һ�����̿飬����Ԥ����ʽ��
														 * adevΪ�������豸�š�blknoΪĿ����̿��߼���ţ�ͬ����ʽ��blkno��
														 * rablknoΪԤ�����̿��߼���ţ��첽��ʽ��rablkno�� */
	void Bwrite(Buf* bp);				/* дһ�����̿� */
	void Bdwrite(Buf* bp);				/* �ӳ�д���̿� */
	void Bawrite(Buf* bp);				/* �첽д���̿� */

	void ClrBuf(Buf* bp);				/* ��ջ��������� */
	void Bflush();				/* ��devָ���豸�������ӳ�д�Ļ���ȫ����������� */
	bool Swap(int blkno, unsigned long addr, int count, enum Buf::BufFlag flag);
	/* Swap I/O ���ڽ���ͼ�����ڴ���̽�����֮�䴫��
	 * blkno: ���������̿�ţ�addr:  ����ͼ��(���Ͳ���)�ڴ���ʼ��ַ��
	 * count: ���д����ֽ�����byteΪ��λ�����䷽��flag: �ڴ�->������ or ������->�ڴ档 */
	Buf& GetSwapBuf();					/* ��ȡ����ͼ���������Buf�������� */
	Buf& GetBFreeList();				/* ��ȡ���ɻ�����п��ƿ�Buf�������� */

private:
	void GetError(Buf* bp);				/* ��ȡI/O�����з����Ĵ�����Ϣ */
	void NotAvail(Buf* bp);				/* �����ɶ�����ժ��ָ���Ļ�����ƿ�buf */
	Buf* InCore(short adev, int blkno);	/* ���ָ���ַ����Ƿ����ڻ����� */

private:
	Buf bFreeList;						/* ���ɻ�����п��ƿ� */
	//Buf bDevList;                        /* �豸���ƿ飬�������豸���ж�ͷ */   //������Ա����
	Buf m_Buf[NBUF];					/* ������ƿ����� */
	char Buffer[NBUF][BUFFER_SIZE];	/* ���������� */
};

