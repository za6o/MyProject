/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h 2186 2010-09-22 10:25:15Z aboyapati $
 */

/* CPU frequency */
#define F_CPU 16000000UL

/* UART baud rate */
#define UART_BAUD  9600

#define BAUD_PRESCALLER (((F_CPU / (UART_BAUD * 16UL))) - 1)    //The formula that does all the required maths

/* HD44780 LCD port connections */
#define HD44780_RS D, 7
#define HD44780_RW D, 5
#define HD44780_E  D, 6
/* The data bits have to be not only in ascending order but also consecutive. */
#define HD44780_D4 B, 0

/* Whether to read the busy flag, or fall back to
   worst-time delays. */
#define USE_BUSY_BIT 0
