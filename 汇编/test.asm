;#Mode=DOS
;MASMPlus ���ļ�����ģ�� - �� DOS ����
;--------------------------------------------------------------------
;�����ļ���Ҫָ������ģʽ,����Ĭ����EXE��ʽ,��ϵͳ�����п�������Ĭ����DOS����Windows.
;����ģʽ�Դ���DOS/COM/CON/EXE/DLL/LIB�⼸��,����б�Ҫ,���Ը���ide.ini����µı���ģʽ
;��Ȼ,���õ��Ǵ���Ϊһ������.�����㼰���ڹ���,ʹ�÷���:��Ctrl��ѡ->��������.�����ж���ļ�

.model small
.stack 200h
.data
	szMsg db 'Hello World!',13,10,'$'
	ARRAY DB 10 DUP(10)
	
.CODE
START:
	mov ax,@data
	mov ds,ax
	
	mov ax, -15
	mov bl, [di]
	mov ah,4ch
	int 21h
END START