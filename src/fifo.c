/* Author : David Neira Martrus
   Date: 03/02/2020
   Project 2
   Teacher: Ángel López Aguirre
*/

#include"../include/fifo.h"
#ifndef _FIFO_C
#define _FIFO_C
int pointer;
int faults ,hits;
void print_fifo(int frame_size,int frame[])
{
	int i;
	for(i=0;i<frame_size;i++)
	{
		if(frame[i]==-1)
			printf("- ");
		else
			printf("%d ",frame[i]);
	}

	printf("\n");
}

void add_reference_fifo(int frame_size,int frame[], int reference)
{
	int i;
	bool alloted = false;
	for(i=0;i<frame_size;i++)
	{
		if(frame[i]==reference)
		{
			alloted = true;
			printf("  Hit for %d | ", reference);
			hits++;
			break;
		}
		else if(frame[i]==-1)
		{
			alloted = true;
			frame[i] = reference;
			printf("Fault for %d | ", reference);
			faults++;
			break;
		}
	}
	if(alloted == false)
	{
		faults++;
		printf("Fault for %d | ", reference);
		frame[pointer] = reference;
		pointer = (pointer+1)%frame_size;
	}
	print_fifo(frame_size, frame);
}

void execute_fifo(int frame_size,int number_of_reference,int mem_ref[])
{

	int frame[frame_size];
	for(int i=0;i<frame_size;i++)
	{
		frame[i] = -1;
	}

	
	for(int i=0;i<number_of_reference;i++)
	{
		add_reference_fifo(frame_size,frame,mem_ref[i]);
	}
	printf("\nNumber of faults: %d \nNumber of hits: %d\n",faults,hits);
}
#endif