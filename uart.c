/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Stdio demo, UART implementation
 *
 * $Id: uart.c 1008 2005-12-28 21:38:59Z joerg_wunsch $
 */

#include "defines.h"

#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include "uart.h"

/*
 * Initialize the UART to 9600 Bd, tx/rx, 8N1.
 */
void
uart_init(void)
{
	 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	 UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
int uart_putchar(char c)
{

  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = c;

  return 0;
}


void USART_putstring(char* StringPtr)
{
	while (*StringPtr != 0x00)
		uart_putchar(*StringPtr++);
}
