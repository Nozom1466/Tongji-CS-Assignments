/* -----------------------------------------

	 本文件不需要提交、不允许改动

   ----------------------------------------- */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <string.h>
#include <Windows.h>
#include "cmd_console_tools.h"
using namespace std;

/* --------------------------------------------------------------------------------------- */
/*   此处定义两个静态全局变量，目的是所有函数都不需要再带 HANDLE 参数，使外部调用时更方便  */
/*   注：为防止同名，静态全局一般建议加__做为变量名开始                                    */
/* --------------------------------------------------------------------------------------- */
static const HANDLE __hout = GetStdHandle(STD_OUTPUT_HANDLE);		//取标准输出设备对应的句柄
static const HANDLE __hin = GetStdHandle(STD_INPUT_HANDLE);		//取标准输入设备对应的句柄

/***************************************************************************
  函数名称：
  功    能：完成与system("cls")一样的功能，但效率高
  输入参数：
  返 回 值：
  说    明：清除整个屏幕缓冲区，不仅仅是可见窗口区域(使用当前颜色)
***************************************************************************/
void cct_cls(void)
{
	COORD coord = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO binfo; /* to get buffer info */
	DWORD num;

	/* 取当前缓冲区信息 */
	GetConsoleScreenBufferInfo(__hout, &binfo);
	/* 填充字符 */
	FillConsoleOutputCharacter(__hout, (TCHAR)' ', binfo.dwSize.X * binfo.dwSize.Y, coord, &num);
	/* 填充属性 */
	FillConsoleOutputAttribute(__hout, binfo.wAttributes, binfo.dwSize.X * binfo.dwSize.Y, coord, &num);

	/* 光标回到(0,0) */
	SetConsoleCursorPosition(__hout, coord);
	return;
}

/***************************************************************************
  函数名称：
  功    能：设置指定的颜色
  输入参数：const int bg_color ：背景色（0-15）
			const int fg_color ：前景色（0-15）
  返 回 值：
  说    明：颜色的取值由背景色+前景色组成，各16种
			fg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
					  8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
			bg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
					  8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
			最终的颜色为 背景色*16+前景色
***************************************************************************/
void cct_setcolor(const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(__hout, bg_color * 16 + fg_color);
}

/***************************************************************************
  函数名称：
  功    能：返回最后一次setcolor的前景色和背景色
  输入参数：int &bg_color      ：返回的背景色（0-15）
			int &fg_color      ：返回的前景色（0-15）
  返 回 值：
  说    明：形参中的&表示引用，第六章会讲到，引用形参的值可以返回给实参
***************************************************************************/
void cct_getcolor(int& bg_color, int& fg_color)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	bg_color = binfo.wAttributes / 16;
	fg_color = binfo.wAttributes % 16;
}

/***************************************************************************
  函数名称：
  功    能：将光标移动到指定位置
  输入参数：const int X       ：X轴坐标（列）
			const int Y       ：Y轴坐标（行）
  返 回 值：
  说    明：屏幕左上角坐标为(0,0)，在cmd窗口的大小未被调整的情况下，Win10为：
			横向x轴，对应列(0-119)
			纵向y轴，对应行(0-29)
***************************************************************************/
void cct_gotoxy(const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(__hout, coord);
}

/***************************************************************************
  函数名称：
  功    能：取当前光标所在位置的坐标值
  输入参数：int &x            ：取得的X轴坐标（列）
			int &y            ：取得的Y轴坐标（行）
  返 回 值：
  说    明：形参中的&表示引用，第六章会讲到，引用形参的值可以返回给实参
***************************************************************************/
void cct_getxy(int& x, int& y)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	x = binfo.dwCursorPosition.X;
	y = binfo.dwCursorPosition.Y;

	return;
}

/***************************************************************************
  函数名称：
  功    能：设置光标状态（显示/不显示/全高/半高/横线等）
  输入参数：const int option：要设置的光标状态
  返 回 值：
  说    明：
***************************************************************************/
void cct_setcursor(const int options)
{
	CONSOLE_CURSOR_INFO cursor_info;

	switch (options) {
	case CURSOR_VISIBLE_FULL:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 100;
		break;
	case CURSOR_VISIBLE_HALF:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 50;
		break;
	case CURSOR_INVISIBLE:
		cursor_info.bVisible = 0;
		cursor_info.dwSize = 1;
		break;
	case CURSOR_VISIBLE_NORMAL:
	default: //缺省显示光标，横线
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 25;
		break;
	}
	SetConsoleCursorInfo(__hout, &cursor_info);
}

/***************************************************************************
  函数名称：
  功    能：在指定位置，用指定颜色，显示一个字符若干次
  输入参数：const int X       ：X轴坐标（列）
			const int Y       ：Y轴坐标（行）
			const char ch     ：要输出的字符值
			const int bg_color：背景色（缺省为COLOR_BLACK）
			const int fg_color：背景色（缺省为COLOR_WHITE）
			const int rpt     ：重复次数（缺省为1）
  返 回 值：
  说    明：X、Y的范围参见gotoxy函数的说明
***************************************************************************/
void cct_showch(const int X, const int Y, const char ch, const int bg_color, const int fg_color, const int rpt)
{
	int i;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);

	/* 循环n次，打印字符ch */
	for (i = 0; i < rpt; i++)
		putchar(ch);
}

/***************************************************************************
  函数名称：
  功    能：在指定位置，用指定颜色，显示一个字符串
  输入参数：const int X       ：X轴坐标（列）
			const int Y       ：Y轴坐标（行）
			const char *str   ：要输出的字符串
			const int bg_color：背景色（缺省为COLOR_BLACK）
			const int fg_color：背景色（缺省为COLOR_WHITE）
			const int rpt     ：重复次数（缺省为1）
			const int max_len ：-1（表示不限制长度，按strlen(str)*rpt的实际打印）
  返 回 值：
  说    明：如果读不懂，暂时放着，等指针学习完成后再阅读
***************************************************************************/
void cct_showstr(const int X, const int Y, const char* str, const int bg_color, const int fg_color, int rpt, int maxlen)
{
	const char* p;
	int i, rpt_count = 0;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);

	/* 首先考虑str==NULL / str="" 的情况
	   1、如果maxlen是-1/0，则直接返回，什么都不打印
	   2、如果maxlen>0，则用maxlen个空格填充 */
	if (str == NULL || str[0] == 0) {
		for (i = 0; i < maxlen; i++) //如果maxlen是-1、0，循环不执行，直接返回
			putchar(' ');
		return;
	}

	/* 之行到此，是str非NULL/str!=""的情况(既strlen一定>0) */
	if (rpt <= 0)
		rpt = 1; //防止错误参数

	if (maxlen < 0)
		maxlen = strlen(str) * rpt; //未给出maxlen则为原始长度

	for (i = 0, p = str; i < maxlen; i++, p++) {	//重复rpt次，每次输出字符串，适用于在画边框时输出若干个"═"等情况
		if (*p == 0) {
			p = str; //如果p已经到\0，则回到头（此处已保证strlen(str)>0，即一定有内容）
			rpt_count++;
		}
		putchar(rpt_count < rpt ? *p : ' '); //如果超过了rpt次数则用空格填充
	}
}

/***************************************************************************
  函数名称：
  功    能：在指定位置，用指定颜色，显示一个字符串
  输入参数：const int X       ：X轴坐标（列）
			const int Y       ：Y轴坐标（行）
			const int num     ：要输出的int值
			const int bg_color：背景色（缺省为COLOR_BLACK）
			const int fg_color：背景色（缺省为COLOR_WHITE）
			const int rpt     ：重复次数（缺省为1）
返 回 值：
说    明：
***************************************************************************/
void cct_showint(const int X, const int Y, const int num, const int bg_color, const int fg_color, const int rpt)
{
	int i;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);
	for (i = 0; i < rpt; i++)	//重复rpt次，每次输出字符串，适用于在画边框时输出若干个"═"等情况
		cout << num;
}

/***************************************************************************
  函数名称：
  功    能：改变cmd窗口的大小及缓冲区的大小
  输入参数：const int cols         ：新的列数
			const int lines        ：新的行数
			const int buffer_cols  ：新的缓冲区列数
			const int buffer_lines ：新的缓冲区行数
  返 回 值：
  说    明：必须先设置缓冲区，再设置窗口大小，
			否则若窗口大小大于当前缓冲区（未设置前）则设置失败
***************************************************************************/
void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines)
{
	/* 取当前系统允许的窗口的行列最大值 */
	COORD max_coord;
	max_coord = GetLargestConsoleWindowSize(__hout); /* .X 和 .Y 分别是窗口的列和行的最大值 */

	/* 处理设置窗口的行列的非法值 */
	if (set_cols <= 0 || set_lines <= 0)
		return;
	if (set_cols > max_coord.X)
		set_cols = max_coord.X;
	if (set_lines > max_coord.Y)
		set_lines = max_coord.Y;

	/* 设置窗口的行列大小（从0开始，0 ~ lines-1, 0 ~ cols-1）*/
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = set_lines - 1;
	rect.Left = 0;
	rect.Right = set_cols - 1;

	/* 设置缓冲区的行列大小(缺省或小于窗口值则与窗口值一样) */
	COORD cr;
	cr.X = (set_buffer_cols == -1 || set_buffer_cols < set_cols) ? set_cols : set_buffer_cols;		//未给出或给出的值小于set_cols则用set_cols，未控制上限
	cr.Y = (set_buffer_lines == -1 || set_buffer_lines < set_lines) ? set_lines : set_buffer_lines;	//未给出或给出的值小于set_lines则用set_lines，未控制上限

	/* 取当前窗口及缓冲区的大小(就是getconsoleborder) */
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	cur_cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	//可见窗口的列数
	cur_lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1;	//可见窗口的行数
	cur_buffer_cols = binfo.dwSize.X;							//缓冲区的列数
	cur_buffer_lines = binfo.dwSize.Y;							//缓冲区的行数

	cct_cls();
	/* 设置顺序(保证设置窗口大小时，现缓冲区的列值>窗口值) */
	if (cr.X <= cur_buffer_cols) {
		if (cr.Y <= cur_buffer_lines) {
			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
		else { //cr.Y > cur_buffer_lines，先要让缓冲区的行数变大
			COORD tmpcr;
			tmpcr.X = cur_buffer_cols;
			tmpcr.Y = cr.Y;
			SetConsoleScreenBufferSize(__hout, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
	}
	else {//cr.X > cur_buffer_cols
		if (cr.Y >= cur_buffer_lines) {
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
		}
		else {	//cr.Y < cur_buffer_lines
			COORD tmpcr;
			tmpcr.X = cr.X;
			tmpcr.Y = cur_buffer_lines;
			SetConsoleScreenBufferSize(__hout, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(__hout, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(__hout, cr);//设置缓冲区
		}
	}

	return;
}

/***************************************************************************
  函数名称：
  功    能：取当前cmd窗口的大小设置
  输入参数：int &cols         ：当前窗口的列数-返回值
			int &lines        ：当前窗口的行数-返回值
			int &buffer_cols  ：当前缓冲区的列数-返回值
			int &buffer_lines ：当前缓冲区的行数-返回值
  返 回 值：
  说    明：
***************************************************************************/
void cct_getconsoleborder(int& cols, int& lines, int& buffer_cols, int& buffer_lines)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;		//可见窗口的列数
	lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1;		//可见窗口的行数
	buffer_cols = binfo.dwSize.X;			//缓冲区的列数
	buffer_lines = binfo.dwSize.Y;			//缓冲区的行数
}

/***************************************************************************
  函数名称：
  功    能：取当前cmd窗口的标题
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void cct_getconsoletitle(char* title, int maxbuflen)
{
	GetConsoleTitleA(title, maxbuflen); //不检查是否越界、是否有空间
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void cct_setconsoletitle(const char* title)
{
	SetConsoleTitleA(title);
}

/***************************************************************************
函数名称：
功    能：允许使用鼠标
输入参数：const HANDLE hin ：cmd窗口输入句柄
返 回 值：
说    明：某些cmd窗口控制语句执行后，可能会取消鼠标支持，则调用本函数回再次加入
***************************************************************************/
void cct_enable_mouse(void)
{
	DWORD Mode;

	GetConsoleMode(__hin, &Mode);	/* 取得控制台原来的模式 */
	SetConsoleMode(__hin, Mode | ENABLE_MOUSE_INPUT);	//加入鼠标支持（可能原先已支持鼠标，再加也没错）
}

/***************************************************************************
函数名称：
功    能：允许使用鼠标
输入参数：const HANDLE hin ：cmd窗口输入句柄
返 回 值：
说    明：某些cmd窗口控制语句执行后，可能会取消鼠标支持，则调用本函数回再次加入
***************************************************************************/
void cct_disable_mouse(void)
{
	DWORD Mode;

	GetConsoleMode(__hin, &Mode);	/* 取得控制台原来的模式 */
	SetConsoleMode(__hin, Mode & (~ENABLE_MOUSE_INPUT));	//去除鼠标支持（如果原先已不支持鼠标，再设也没错）
}

/***************************************************************************
  函数名称：
  功    能：读鼠标按键
  输入参数：
  返 回 值：
  说    明：下列说明来自鼠标定义文件
			01.typedef struct _MOUSE_EVENT_RECORD      //鼠标事件结构体
			02.{
			03.    COORD dwMousePosition;      //当前鼠标在控制台窗口缓冲区的位置
			04.    DWORD dwButtonState;        //鼠标按键的状态
			05.    DWORD dwControlKeyState;    //控制键状态
			06.    DWORD dwEventFlags;         //鼠标事件类型
			07.} MOUSE_EVENT_RECORD;
			08.
			09.其中鼠标按键状态dwButtonState可能的值有
			10.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			11.FROM_LEFT_1ST_BUTTON_PRESSED        最左边的鼠标键被按下      一般来说就是鼠标左键
			12.FROM_LEFT_2ND_BUTTON_PRESSED        左起第二个鼠标键被按下    一般来说是鼠标中键，就是滚轮键
			13.FROM_LEFT_3RD_BUTTON_PRESSED        左起第三个鼠标键被按下
			14.FROM_LEFT_4TH_BUTTON_PRESSED        左起第四个鼠标键被按下
			15.RIGHTMOST_BUTTON_PRESSED            最右边的鼠标键被按下      一般来说是鼠标右键
			16.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			17.控制键状态dwControlKeyState与键盘事件的一样
			18.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			19.ENHANCED_KEY        扩展键被按下
			20.LEFT_ALT_PRESSED    左Alt键被按下
			21.LEFT_CTRL_PRESSED   左Ctrl键被按下
			22.RIGHT_ALT_PRESSED   右Alt键被按下
			23.RIGHT_CTRL_PRESSED  右Ctrl键被按下
			24.NUMLOCK_ON          数字锁定被打开
			25.SCROLLLOCK_ON       滚动锁定被打开
			26.CAPSLOCK_ON         大写锁定被打开
			27.SHIFT_PRESSED       Shift键被按下
			28.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			29.鼠标事件类型dwEventFlags有以下几种
			30.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			31.DOUBLE_CLICK            双击，第一击只作为普通按键事件，第二击才作为双击事件
			32.MOUSE_HWHEELED          水平鼠标滚轮移动
			33.MOUSE_MOVED             鼠标移动
			34.MOUSE_WHEELED           垂直鼠标滚轮移动
			35.0                       当鼠标有键被按下或者释放
***************************************************************************/
int cct_read_keyboard_and_mouse(int& MX, int& MY, int& MAction, int& keycode1, int& keycode2)
{
	static int MX_old = -1, MY_old = -1, MAction_old = MOUSE_ONLY_MOVED;
	INPUT_RECORD InputRec;
	DWORD        res;
	COORD        crPos;

	while (1) {
		/* 从hin中读输入状态（包括鼠标、键盘等） */
		ReadConsoleInput(__hin, &InputRec, 1, &res);

		/* 键盘事件（要优于鼠标事件，否则如果鼠标放在目标区，无法读键） */
		if (InputRec.EventType == KEY_EVENT) {
			keycode1 = 0x00;
			keycode2 = 0x00;
			if (InputRec.Event.KeyEvent.bKeyDown) { // 只在按下时判断，弹起时不判断
				/* 所有的虚拟键码可参考下列网址：
					https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E9%94%AE%E7%A0%81/9884611?fr=aladdin
					对应头文件：c:\Program Files (x86)\Windows Kits\10\Include\版本号\um\WinUser.h (以Vdst017为例，缺省路径)
					目前只返回四个箭头键，其余的可以自行添加 */
				switch (InputRec.Event.KeyEvent.wVirtualKeyCode) {
				case VK_UP:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_UP;		//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和72（0x48）
					break;
				case VK_DOWN:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_DOWN;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和80（0x50）
					break;
				case VK_LEFT:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_LEFT;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和75（0x4B）
					break;
				case VK_RIGHT:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_RIGHT;	//模拟 _getch()方式返回的两个键码，分别是224（0xE0）和77（0x4D）
					break;
				default:
					break;
				} //end of switch

				  /* 非箭头键直接返回ASCII形式（Fn、Insert、Delete等均未处理） */
				if (keycode1 == 0)
					keycode1 = InputRec.Event.KeyEvent.uChar.AsciiChar;

				return CCT_KEYBOARD_EVENT;
			} //end of if (KEYDOWN)
		} // end of if (键盘事件)

		/* 鼠标事件 */
		if (InputRec.EventType == MOUSE_EVENT) {
			/* 从返回中读鼠标指针当前的坐标 */
			crPos = InputRec.Event.MouseEvent.dwMousePosition;
			MX = crPos.X;
			MY = crPos.Y;

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {//鼠标移动
				/* 如果始终是MOUSE_MOVED事件且坐标不变，则不认为是MOUSE_MOVED */
				if (MX_old == MX && MY_old == MY && MAction_old == MOUSE_ONLY_MOVED)
					continue;

				/* 位置变化则记录下来 */
				MX_old = MX;
				MY_old = MY;
				MAction = MOUSE_ONLY_MOVED;
				MAction_old = MAction;
				return CCT_MOUSE_EVENT;
			}
			MAction_old = MOUSE_NO_ACTION; //置非MOUSE_ONLY_MOVED值即可

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) { //滚轮移动 
				/* https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
					The vertical mouse wheel was moved.
					If the high word of the dwButtonState member contains a positive value, the wheel was rotated forward, away from the user.
					Otherwise, the wheel was rotated backward, toward the user. */
				if (InputRec.Event.MouseEvent.dwButtonState & 0x80000000) //高位为1，负数
					MAction = MOUSE_WHEEL_MOVED_DOWN;
				else
					MAction = MOUSE_WHEEL_MOVED_UP;
				return CCT_MOUSE_EVENT;
			}

			if (InputRec.Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED)) { //同时按下左右键
				MAction = MOUSE_LEFTRIGHT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //按下左键
				if (InputRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
					MAction = MOUSE_LEFT_BUTTON_DOUBLE_CLICK;
				else
					MAction = MOUSE_LEFT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) { //按下右键
				if (InputRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
					MAction = MOUSE_RIGHT_BUTTON_DOUBLE_CLICK;
				else
					MAction = MOUSE_RIGHT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) { //按下滚轮
				MAction = MOUSE_WHEEL_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else //忽略其它按键操作
				continue;
		} // end of if(鼠标事件)
	} //end of while(1)

	return CCT_MOUSE_EVENT; //此句应该执行不到，为避免某些编译器报不完全分支错误，加
}

typedef BOOL(WINAPI* PROCSETCONSOLEFONT)(HANDLE, DWORD);
typedef BOOL(WINAPI* PROCGETCONSOLEFONTINFO)(HANDLE, BOOL, DWORD, PCONSOLE_FONT_INFO);
typedef COORD(WINAPI* PROCGETCONSOLEFONTSIZE)(HANDLE, DWORD);
typedef DWORD(WINAPI* PROCGETNUMBEROFCONSOLEFONTS)();
typedef BOOL(WINAPI* PROCGETCURRENTCONSOLEFONT)(HANDLE, BOOL, PCONSOLE_FONT_INFO);
typedef BOOL(WINAPI* PROCSetBufferSize)(HANDLE hConsoleOutput, COORD dwSize);
typedef HWND(WINAPI* PROCGETCONSOLEWINDOW)();

#if 0 //条件编译，0表示关闭
PROCSETCONSOLEFONT SetConsoleFont;
PROCGETCONSOLEFONTINFO GetConsoleFontInfo;
PROCGETCONSOLEFONTSIZE GetConsoleFontSize;
PROCGETNUMBEROFCONSOLEFONTS GetNumberOfConsoleFonts;
PROCGETCURRENTCONSOLEFONT GetCurrentConsoleFont;
#endif

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int cct_getfontinfo(void)
{
	HMODULE hKernel32 = GetModuleHandleA("kernel32");
	if (hKernel32 == 0)
		return -1;
	PROCSETCONSOLEFONT SetConsoleFont = (PROCSETCONSOLEFONT)GetProcAddress(hKernel32, "SetConsoleFont");
	PROCGETCONSOLEFONTINFO GetConsoleFontInfo = (PROCGETCONSOLEFONTINFO)GetProcAddress(hKernel32, "GetConsoleFontInfo");
	PROCGETCONSOLEFONTSIZE GetConsoleFontSize = (PROCGETCONSOLEFONTSIZE)GetProcAddress(hKernel32, "GetConsoleFontSize");
	PROCGETNUMBEROFCONSOLEFONTS GetNumberOfConsoleFonts = (PROCGETNUMBEROFCONSOLEFONTS)GetProcAddress(hKernel32, "GetNumberOfConsoleFonts");
	PROCGETCURRENTCONSOLEFONT GetCurrentConsoleFont = (PROCGETCURRENTCONSOLEFONT)GetProcAddress(hKernel32, "GetCurrentConsoleFont");
	//	PROCSetBufferSize SetConsoleBufSize = (PROCSetBufferSize)GetProcAddress(hKernel32,"SetConsoleScreenBufferSize");
	//	PROCGETCONSOLEWINDOW GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");

	CONSOLE_FONT_INFOEX infoex;
	char fontname[64];
	CONSOLE_FONT_INFO cur_f;
	int nFontNum;

	/* 取当前字体的名称，cmd下目前是两种：Terminal(点阵字体)和新宋体*/
	infoex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(__hout, 1, &infoex);
	WideCharToMultiByte(CP_ACP, 0, infoex.FaceName, -1, fontname, sizeof(fontname), NULL, NULL);
	cout << "当前字体：" << fontname << endl;

	/* 打印当前字体的字号信息 */
	nFontNum = GetNumberOfConsoleFonts();
	cout << "    字 号 数 量 ：" << nFontNum << endl;

	/* 取当前的字号设置 */
	GetCurrentConsoleFont(__hout, 0, &cur_f);
	cout << "    当前字号序号：" << cur_f.nFont << endl;
	cout << "            宽度：" << cur_f.dwFontSize.X << " pixels" << endl;
	cout << "            高度：" << cur_f.dwFontSize.Y << " pixels" << endl;

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：改变输出窗口的字号
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void cct_setconsolefont(const int font_no)
{
	HMODULE hKernel32 = GetModuleHandleA("kernel32");
	if (hKernel32 == 0)
		return;

	PROCSETCONSOLEFONT SetConsoleFont = (PROCSETCONSOLEFONT)GetProcAddress(hKernel32, "SetConsoleFont");
	/* font_no width high
	   0       3     5
	   1       4     6
	   2       5     8
	   3       6     8
	   4       8     8
	   5       16    8
	   6       5     12
	   7       6     12
	   8       7     12
	   9       8     12
	   10      16    12
	   11      8     16
	   12      12    16
	   13      8     18
	   14      10    18
	   15      10    20 */
	SetConsoleFont(__hout, font_no);
}

/***************************************************************************
  函数名称：
  功    能：改变输出窗口的字体及大小
  输入参数：
  返 回 值：
  说    明：GBK编码的cmd窗口只支持"点阵字体"和"新宋体"两种，
			给出fontname时，非"新宋体"全部做为缺省字体(Terminal-"点阵字体")
***************************************************************************/
void cct_setfontsize(const char* fontname, const int high, const int width)
{
	CONSOLE_FONT_INFOEX infoex = { 0 };

	infoex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	infoex.dwFontSize.X = width;		// 字体宽度，对于Truetype字体，不需要宽度，为0即可，对于点阵字体，如果宽度为0，则选指定高度存在的宽度
	infoex.dwFontSize.Y = high;			// 字体高度
	infoex.FontWeight = FW_NORMAL;		//具体见 wingdi.h

	MultiByteToWideChar(CP_ACP, 0, fontname, -1, infoex.FaceName, LF_FACESIZE);
	SetCurrentConsoleFontEx(__hout, NULL, &infoex);

	return;
}
