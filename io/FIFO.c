/*
 * FIFO.c
 *
 *  Created on: 12.07.2013
 *      Author: najiji
 */

#include "FIFO.h"

void Fifo_Init(fifo* list)
{
	list->head = 0;
	list->tail = 0;
}

int Fifo_Enqueue(fifo* list, char item)
{
	if(list->head == FIFOSIZE) 				//FIFO recircle
		list->head = 0;						//restart at beginning
	list->data[list->head] = item;			//Assign value to storage position
	list->head++;							//increase start index
	return 1;
}

char Fifo_Dequeue(fifo* list)
{
	if(list->tail == list->head) 			//FIFO empty, cannot dequeue
		return 0x00;						//FAIL value
	char value = list->data[list->tail]; 	//readout value

	list->tail++;
	if(list->tail == FIFOSIZE)				//if we reached end
		list->tail = 0;						//restart at beginning
	return value;
}

char* Fifo_Dump(fifo* list, char* destination)
{
	for(int i=0; list->tail != list->head; i++)
	{
		destination[i] = Fifo_Dequeue(list);
		destination[i+1] = 0x00;
	}
	return destination;
}

void Fifo_Flush(fifo* list)
{
	list->head = 0;							//reset position
	list->tail = 0;
}

int	Fifo_Empty(fifo* list)
{
	return list->tail == list->head;
}

int	Fifo_Length(fifo* list)
{
	return list->head - list->tail;
}
