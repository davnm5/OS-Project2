/* Author : David Neira Martrus
   Date: 03/02/2020
   Project 2
   Teacher: Ángel López Aguirre
*/


#include"../include/optimal.h"
#ifndef _OPTIMAL_C
#define _OPTIMAL_C

int pointer;
int faults ,hits;
void print_optimal(int frame_size,int frame[])
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

int predict_optimal(int reference_length, int references[], int page_no ,int frame_size,int frame[], int start)
{
	int pos = -1, farthest = start, i;
	for(i=0;i<frame_size;i++)
	{
		int j;
		for(j=start;j<reference_length;j++)
		{
			if(frame[i]==references[j])
			{
				if(j>farthest)
				{
					farthest=j;
					pos=i;
				}
				break;
			}
		}
		if(j==page_no)
			return i;
	}
	if(pos == -1)
		return 0;
	else
		return pos;
}

void add_reference_optimal(int frame_size,int frame[], int reference, int current_position,int reference_length, int references[])
{
	int i;
	bool allocated=false;
	for(i=0;i<frame_size;i++)
	{
		
		if(frame[i]==reference)
		{
			printf("  Hit for %d | ", reference);
			hits++;
			allocated = true;
			break;
		}
		else if(frame[i]==-1)
		{
			frame[i] = reference;
			printf("Fault for %d | ", reference);
			faults++;
			allocated = true;
			break;
		}
	}
	if(allocated==false)
	{
		int j = predict_optimal(reference_length,references,current_position,frame_size,frame,current_position+1);

		frame[j] = reference;
		printf("Fault for %d | ", reference);
		faults++;	
	}
	print_optimal(frame_size, frame);
}

void execute_optimal(int frame_size,int number_of_references,int mem_ref[])
{
	
	int frame[frame_size];
	for(int i=0;i<frame_size;i++)
	{
		frame[i] = -1;
	}

	for(int i=0;i<number_of_references;i++)
	{
		add_reference_optimal(frame_size,frame,mem_ref[i],i,number_of_references,mem_ref);
	}
	printf("\nNumber of faults: %d \nNumber of hits: %d\n",faults,hits );
}
#endif