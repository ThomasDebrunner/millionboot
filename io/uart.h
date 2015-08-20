/*
 * uart.h
 *
 *  Created on: 06.07.2013
 *      Author: najiji
 */

#ifndef UART_H_
#define UART_H_
#include "fifo.h"

/*------------------------------------------------------------------------
 * 	Precomputed UBBR values (Platform dependant!)
 *------------------------------------------------------------------------*/
#define UBBR_9600 103
#define UBBR_14400 68
#define UBBR_19200 51
#define UBBR_28800 34
#define UBBR_38400 25
#define UBBR_57600 16
#define UBBR_115200 8


volatile fifo txbuf;
volatile fifo rxbuf;



/*-----------------------------------------------------------------
 *	Initializes Uart interface with specified baud
 *-----------------------------------------------------------------*/
void uart_init(unsigned int baud);

/*-----------------------------------------------------------------
 *	Transmits a byte (character) over UART
 *-----------------------------------------------------------------*/
int uart_send_char(char c);

/*-----------------------------------------------------------------
 *	Transmits a the string specified by str over UART, until 0x00
 *-----------------------------------------------------------------*/
void uart_send(char* str);


#endif /* UART_H_ */
