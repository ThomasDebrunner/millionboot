/*
 * FIFO.h
 *
 *  Created on: 12.07.2013
 *      Author: najiji
 */

#ifndef FIFO_H_
#define FIFO_H_
#define FIFOSIZE 100



typedef volatile struct
{
	char data[FIFOSIZE];
	int head;
	int tail;
} fifo;


void Fifo_Init(fifo* list);

int Fifo_Enqueue(fifo* list, char item);

char Fifo_Dequeue(fifo* list);

char* Fifo_Dump(fifo* list, char* destination);

void Fifo_Flush(fifo* list);

int	Fifo_Empty(fifo* list);

int Fifo_Length(fifo* list);

#endif /* FIFO_H_ */
