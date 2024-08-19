;#Mode=DOS
;MASMPlus 单文件代码模板 - 纯 DOS 程序
;--------------------------------------------------------------------
;单个文件需要指定编译模式,否则默认是EXE方式,在系统设置中可以设置默认是DOS还是Windows.
;编译模式自带了DOS/COM/CON/EXE/DLL/LIB这几种,如果有必要,可以更改ide.ini添加新的编译模式
;当然,更好的是创建为一个工程.更方便及易于管理,使用方法:按Ctrl多选->创建工程.必须有多个文件

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