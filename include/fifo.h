#ifndef _FIFO_H
#define _FIFO_H
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
void execute_fifo(int frame_size,int number_of_reference,int mem_ref[]);
void add_reference_fifo(int frame_size,int frame[], int reference);
void print_fifo(int frame_size,int frame[]);
#endif