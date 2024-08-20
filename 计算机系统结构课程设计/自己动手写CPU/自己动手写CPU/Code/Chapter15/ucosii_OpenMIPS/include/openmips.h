#define REG8(add) *((volatile INT8U *)(add))
#define REG16(add) *((volatile INT16U *)(add))
#define REG32(add) *((volatile INT32U *)(add))

#define IN_CLK 27000000             /* ����ʱ����27MHz */

/**********************************************************************

                            ������ز���������
                                 
**********************************************************************/                               

#define UART_BAUD_RATE  9600          /* ����������9600bps */
#define UART_BASE      0x10000000
#define UART_LC_REG    0x00000003    /* Line Control Register */
#define UART_IE_REG     0x00000001    /* Interrupt Enable Register */
#define UART_TH_REG    0x00000000    /* Transmitter Holding Register */
#define UART_LS_REG     0x00000005    /* Line Status Register */
#define UART_DLB1_REG   0x00000000    /* Divisor Latch Byte 1(LSB) */
#define UART_DLB2_REG   0x00000001    /* Divisor Latch Byte 2(MSB) */

/* Line Status Register�ı�־λ */
#define UART_LS_TEMT	0x40	/* Transmitter empty */
#define UART_LS_THRE	0x20	/* Transmit-hold-register empty */

/* Line Control Register�ı�־λ */
#define UART_LC_NO_PARITY	0x00	/* Parity Disable */
#define UART_LC_ONE_STOP	0x00	/* Stop bits: 0x00��ʾone stop bit */
#define UART_LC_WLEN8  0x03	/* Wordlength: 8 bits */

extern void uart_init(void);
extern void uart_putc(char);
extern void uart_print_str(char*);
extern void uart_print_int(unsigned int);

/**********************************************************************

                              GPIO��ز���������
                                 
**********************************************************************/     
#define GPIO_BASE 0x20000000
#define GPIO_IN_REG  0x00000000
#define GPIO_OUT_REG  0x00000004
#define GPIO_OE_REG  0x00000008
#define GPIO_INTE_REG 0x0000000c

extern void gpio_init(void);
extern void gpio_out(INT32U);
extern INT32U gpio_in(void);




extern void main(void);
