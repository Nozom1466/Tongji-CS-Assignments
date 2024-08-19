#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "FileSystem.h"
#include "OpenFileManager.h"
#include "Log.h"

//#include "File.h"

/*
 * �ļ�������(FileManager)
 * ��װ���ļ�ϵͳ�ĸ���ϵͳ�����ں���̬�´�����̣�
 * ����ļ���Open()��Close()��Read()��Write()�ȵ�
 * ��װ�˶��ļ�ϵͳ���ʵľ���ϸ�ڡ�
 */
class FileManager
{
public:
	/* Ŀ¼����ģʽ������NameI()���� */
	enum DirectorySearchMode
	{
		OPEN = 0,		/* �Դ��ļ���ʽ����Ŀ¼ */
		CREATE = 1,		/* ���½��ļ���ʽ����Ŀ¼ */
		DELETE = 2		/* ��ɾ���ļ���ʽ����Ŀ¼ */
	};

	/* Functions */
public:
	/* Constructors */
	FileManager();
	/* Destructors */
	~FileManager();


	/*
	 * @comment ��ʼ����ȫ�ֶ��������
	 */
	void Initialize();

	/*
	 * @comment Open()ϵͳ���ô������
	 */
	void Open();

	/*
	 * @comment Creat()ϵͳ���ô������
	 */
	void Creat();

	/*
	 * @comment Open()��Creat()ϵͳ���õĹ�������
	 */
	void Open1(Inode* pInode, int mode, int trf);

	/*
	 * @comment Close()ϵͳ���ô������
	 */
	void Close();

	/*
	 * @comment Seek()ϵͳ���ô������
	 */
	void Seek();

	/*
	 * @comment Dup()���ƽ��̴��ļ�������
	 */
	void Dup();

	/*
	 * @comment FStat()��ȡ�ļ���Ϣ
	 */
	void FStat();

	/*
	 * @comment FStat()��ȡ�ļ���Ϣ
	 */
	void Stat();

	/* FStat()��Stat()ϵͳ���õĹ������� */
	void Stat1(Inode* pInode, unsigned long statBuf);

	/*
	 * @comment Read()ϵͳ���ô������
	 */
	void Read();

	/*
	 * @comment Write()ϵͳ���ô������
	 */
	void Write();

	/*
	 * @comment ��дϵͳ���ù������ִ���
	 */
	void Rdwr(enum File::FileFlags mode);

	/*
	 * @comment Pipe()�ܵ�����ϵͳ���ô������
	 */
	void Pipe();

	/*
	 * @comment �ܵ�������
	 */
	void ReadP(File* pFile);

	/*
	 * @comment �ܵ�д����
	 */
	void WriteP(File* pFile);

	/*
	 * @comment Ŀ¼��������·��ת��Ϊ��Ӧ��Inode��
	 * �����������Inode
	 */
	Inode* NameI(char (*func)(), enum DirectorySearchMode mode);

	/*
	 * @comment ��ȡ·���е���һ���ַ�
	 */
	static char NextChar();

	/*
	 * @comment ��Creat()ϵͳ����ʹ�ã�����Ϊ�������ļ������ں���Դ
	 */
	Inode* MakNode(unsigned int mode);

	/*
	 * @comment ��Ŀ¼��Ŀ¼�ļ�д��һ��Ŀ¼��
	 */
	void WriteDir(Inode* pInode);

	/*
	 * @comment ���õ�ǰ����·��
	 */
	void SetCurDir(char* pathname);

	/*
	 * @comment �����ļ���Ŀ¼������������Ȩ�ޣ���Ϊϵͳ���õĸ�������
	 */
	int Access(Inode* pInode, unsigned int mode);

	/*
	 * @comment ����ļ��Ƿ����ڵ�ǰ�û�
	 */
	Inode* Owner();

	/* �ı��ļ�����ģʽ */
	void ChMod();

	/* �ı��ļ��ļ�������user ID����group ID */
	void ChOwn();

	void Delete();

	void Rename(string from, string to);

	/* �ı䵱ǰ����Ŀ¼ */
	void ChDir(int p=0);

	/* �����ļ����������� */
	void Link();

	/* ȡ���ļ� */
	void UnLink();

	/* ���ڽ��������豸�ļ���ϵͳ���� */
	void MkNod();

	void dirDfs(string path, int depth);

public:
	/* ��Ŀ¼�ڴ�Inode */
	Inode* rootDirInode;
};


class DirectoryEntry
{
	/* static members */
public:
	static const int DIRSIZ = 28;	/* Ŀ¼����·�����ֵ�����ַ������� */

	/* Functions */
public:
	/* Constructors */
	DirectoryEntry();
	/* Destructors */
	~DirectoryEntry();

	/* Members */
public:
	int inode;		/* Ŀ¼����Inode��Ų��� */
	char name[DIRSIZ];	/* Ŀ¼����·�������� */
};

#endif
