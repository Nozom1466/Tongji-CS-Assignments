#include "includes.h"

#define BOTH_EMPTY (UART_LS_TEMT | UART_LS_THRE)

#define WAIT_FOR_XMITR \
    do { \
        lsr = REG8(UART_BASE + UART_LS_REG); \
    } while ((lsr & BOTH_EMPTY) != BOTH_EMPTY)

#define WAIT_FOR_THRE \
    do { \
        lsr = REG8(UART_BASE + UART_LS_REG); \
    } while ((lsr & UART_LS_THRE) != UART_LS_THRE)

#define TASK_STK_SIZE 512

OS_STK TaskStartStk[TASK_STK_SIZE];

char Info[100] = "\nPlay Cross Game! \n";

void uart_init(void) {
    INT32U divisor = (INT32U)IN_CLK / (16 * UART_BAUD_RATE);
    REG8(UART_BASE + UART_LC_REG) = 0x80;
    REG8(UART_BASE + UART_DLB1_REG) = divisor & 0xFF;
    REG8(UART_BASE + UART_DLB2_REG) = (divisor >> 8) & 0xFF;
    REG8(UART_BASE + UART_LC_REG) = 0x00;
    REG8(UART_BASE + UART_IE_REG) = 0x00;
    REG8(UART_BASE + UART_LC_REG) = UART_LC_WLEN8 | UART_LC_ONE_STOP | UART_LC_NO_PARITY;
}

void uart_putc(char c) {
    unsigned char lsr;
    WAIT_FOR_THRE;
    REG8(UART_BASE + UART_TH_REG) = c;
    if (c == '\n') {
        WAIT_FOR_THRE;
        REG8(UART_BASE + UART_TH_REG) = '\r';
    }
    WAIT_FOR_XMITR;  
}

INT32U uart_getc(void) {
    unsigned char lsr;
    do {
        lsr = REG8(UART_BASE + UART_LS_REG);
    } while ((lsr & 0x01) == 0);
    return (INT32U)REG8(UART_BASE + UART_TH_REG);
}

void uart_print_str(char* str) {
    INT32U i = 0;
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL();
    while (str[i] != 0) {
        uart_putc(str[i]);
        i++;
    }
    OS_EXIT_CRITICAL();
}

void gpio_init() {
    REG32(GPIO_BASE + GPIO_OE_REG) = 0xFFFFFFFF;
    REG32(GPIO_BASE + GPIO_INTE_REG) = 0x00000000;
    gpio_out(0x0f0f0f0f);
}

void gpio_out(INT32U number) {
    REG32(GPIO_BASE + GPIO_OUT_REG) = number;
}

INT32U gpio_in() {
    return REG32(GPIO_BASE + GPIO_IN_REG);
}

void OSInitTick(void) {
    INT32U compare = (INT32U)(IN_CLK / OS_TICKS_PER_SEC);
    asm volatile("mtc0 %0, $9" : :"r"(0x0));
    asm volatile("mtc0 %0, $11" : :"r"(compare));
    asm volatile("mtc0 %0, $12" : :"r"(0x10000401));
}

void printUnit(INT32U unit) {
    uart_print_str(unit == 0 ? "*" : unit == 1 ? "O" : "X");
}

void printBoard(INT32U* board) {
    int i;
    for (i = 0; i < 9; i++) {
        printUnit(board[i]);
        uart_print_str((i + 1) % 3 == 0 ? "\n" : " ");
    }
    uart_print_str("\n");
}

INT32U checkWinner(INT32U* board) {
    static const INT32U win_patterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };
    int i;
    for (i = 0; i < 8; i++) {
        if (board[win_patterns[i][0]] != 0 &&
            board[win_patterns[i][0]] == board[win_patterns[i][1]] &&
            board[win_patterns[i][1]] == board[win_patterns[i][2]]) {
            return board[win_patterns[i][0]];
        }
    }
    return 0;
}

INT32U isBoardFull(INT32U* board) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == 0) {
            return 0;
        }
    }
    return 1;
}

INT32U isFirst = 1;
INT32U isValid = 1;

void TaskStart(void *pdata) {
    INT32U board[9] = {0};
    INT32U computerPre[9] = {4, 1, 3, 5, 7, 0, 2, 6, 8};
    INT32U gamer = 0;
    INT32U wer = gpio_in() >> 1;
    
    if ((wer >>14) & 1) {
    	if (isFirst == 1) {
    		uart_print_str("Please make sure V10 port is 0...\n");
    		isFirst = 0;
		}
		else {
			isFirst = 0;
		}
		return;
	}
	else {
		isFirst = 1;
	}
	
	if (isValid == 0) {
		return;
	}
	
	
	
	uart_print_str("This is cross game. Enjoy yourself!:\n");

    for (;;) {
        INT32U data = gpio_in();
        INT32U choice = data >> 1;
        INT32U gamer_now = (choice >> 14) & 1;

        if (gamer != gamer_now) {
        	isValid = 1;
            gamer = gamer_now;
            uart_print_str("\nYour turn: \n");

            int index = -1;
            int i;
            for (i = 0; i < 9; i++) {
                if ((choice >> i) & 1) {
                    index = i;
                    break;
                }
            }

            if (index != -1 && board[index] == 0) {
                board[index] = 2;
                printBoard(board);
                if (checkWinner(board) == 2) {
                    uart_print_str("You win!\n");
                    break;
                }
                if (isBoardFull(board)) {
                    uart_print_str("A tie!\n");
                    break;
                }
                uart_print_str("Computer turn: \n");
                for (i = 0; i < 9; i++) {
                    if (board[computerPre[i]] == 0) {
                        board[computerPre[i]] = 1;
                        break;
                    }
                }
                printBoard(board);
                if (checkWinner(board) == 1) {
                    uart_print_str("Computer wins!\n");
                    break;
                }
                if (isBoardFull(board)) {
                    uart_print_str("A tie!\n");
                    break;
                }
            } 
			else {
                uart_print_str("Invalid move, try again.\n");
                printBoard(board);
                isValid = 0;
            }
        }
    }
}

void main() {
    OSInit();
    uart_init();
    gpio_init();
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();
}

