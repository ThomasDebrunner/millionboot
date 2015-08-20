/*
 * UART Library designed for Atmel ATMEGA 328 and 168.
 *
 * (c) najiji 2013
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "uart.h"
#include "FIFO.h"

void uart_init(unsigned int baud)
{
	int ubrr_value=0;
	switch(baud)
	{
	case 9600:
		ubrr_value = UBBR_9600;
		break;
	case 14400:
		ubrr_value = UBBR_14400;
		break;
	case 19200:
		ubrr_value = UBBR_19200;
		break;
	case 28800:
		ubrr_value = UBBR_28800;
		break;
	case 38400:
		ubrr_value = UBBR_38400;
		break;
	case 57600:
		ubrr_value = UBBR_57600;
		break;
	case (unsigned int)115200:
		ubrr_value = UBBR_115200;
		break;
	}
	//set baudrate
	UBRR0H = (unsigned char)(ubrr_value>>8);
	UBRR0L = (unsigned char)ubrr_value;
	//enable tx/rx
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	//set 8-bit mode
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	//setup fifos
	Fifo_Init(&txbuf);
	Fifo_Init(&rxbuf);
}

//------------------------------------------------------------------------------------------------------

int uart_send_char(char c)
{
	Fifo_Enqueue(&txbuf, c);	//queue element
	UCSR0B |= (1<<UDRIE0); //Force TX interrupt;
	return 1;
}



void uart_send(char* str)
{
	while(*str)
	{
		uart_send_char(*str);
		str++;
	}
}


//------------------------------------------------------------------------------------------------------


ISR(USART_UDRE_vect) //data register empty
{
	int back = SREG;
	cli();
	if(!Fifo_Empty(&txbuf))
		UDR0 = Fifo_Dequeue(&txbuf);
	else
		UCSR0B &= ~(1<<UDRIE0); //disable interrupt
	SREG = back;
	sei();
}

