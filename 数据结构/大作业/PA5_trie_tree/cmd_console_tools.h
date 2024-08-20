#pragma once

   /* ������ɫ���ú궨��ȡ�����֣�������䣩 */
#define COLOR_BLACK		0	//��
#define COLOR_BLUE		1	//��
#define COLOR_GREEN		2	//��
#define COLOR_CYAN		3	//��
#define COLOR_RED		4	//��
#define COLOR_PINK		5	//��
#define COLOR_YELLOW	6	//��
#define COLOR_WHITE		7	//��
#define COLOR_HBLACK	8	//����
#define COLOR_HBLUE		9	//����
#define COLOR_HGREEN	10	//����
#define COLOR_HCYAN		11	//����
#define COLOR_HRED		12	//����
#define COLOR_HPINK		13	//����
#define COLOR_HYELLOW	14	//����
#define COLOR_HWHITE	15	//����

/* ���������̲������� */
#define CCT_MOUSE_EVENT				0
#define CCT_KEYBOARD_EVENT			1

/* �����ĸ��������Ϊʲô����Щֵ�������ͷ���ƶ�������ҵ�� */
#define KB_ARROW_UP					72
#define KB_ARROW_DOWN				80
#define KB_ARROW_LEFT				75
#define KB_ARROW_RIGHT				77

/* �������Ĳ��� */
#define MOUSE_NO_ACTION						0x0000	//��������
#define MOUSE_ONLY_MOVED					0x0001	//����ƶ�
#define MOUSE_LEFT_BUTTON_CLICK				0x0002	//�������
#define MOUSE_LEFT_BUTTON_DOUBLE_CLICK		0x0004	//˫�����
#define MOUSE_RIGHT_BUTTON_CLICK			0x0008	//�����Ҽ�
#define MOUSE_RIGHT_BUTTON_DOUBLE_CLICK	0x0010	//˫���Ҽ�
#define MOUSE_LEFTRIGHT_BUTTON_CLICK		0x0020	//ͬʱ�������Ҽ�
#define MOUSE_WHEEL_CLICK					0x0040	//���ֱ�����
#define MOUSE_WHEEL_MOVED_UP				0x0080	//���������ƶ�
#define MOUSE_WHEEL_MOVED_DOWN				0x0100	//���������ƶ�

/* ���������̬ */
#define CURSOR_VISIBLE_FULL					0	//�����ʾ��ȫ��ɫ��
#define CURSOR_VISIBLE_HALF					1	//�����ʾ�����ɫ��
#define CURSOR_VISIBLE_NORMAL				2	//�����ʾ�����ߣ�ȱʡΪ�˷�ʽ��
#define CURSOR_INVISIBLE					3	//��겻��ʾ

/* cmd_console_tools.cpp�µĺ������� */

/* ��������ɫ���á�������á��ַ�/�ַ�����ʾ */
void cct_cls(void);
void cct_setcolor(const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE);
void cct_getcolor(int& bg_color, int& fg_color);
void cct_gotoxy(const int X, const int Y);
void cct_getxy(int& x, int& y);
void cct_setcursor(const int options);
void cct_showch(const int X, const int Y, const char ch, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1);
void cct_showstr(const int X, const int Y, const char* str, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, int rpt = 1, int max_len = -1);
void cct_showint(const int X, const int Y, const int num, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1);

/* �봰�ڴ�С�йصĺ��� */
void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols = -1, int set_buffer_lines = -1);
void cct_getconsoleborder(int& cols, int& lines, int& buffer_cols, int& buffer_lines);

/* �б������йصĺ��� */
void cct_getconsoletitle(char* title, int maxbuflen);
void cct_setconsoletitle(const char* title);

/* ���������йصĺ��� */
void cct_enable_mouse(void);
void cct_disable_mouse(void);
int  cct_read_keyboard_and_mouse(int& MX, int& MY, int& MAction, int& keycode1, int& keycode2);

/* �����弰�ֺ������йصĺ��� */
int  cct_getfontinfo(void);
void cct_setfontsize(const char* fontname, const int high, const int width = 0);

