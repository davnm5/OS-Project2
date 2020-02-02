#ifndef _LRU_H
#define _LRU_H
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
void execute_lru(int frame_size,int number_of_references,int mem_ref[]);
int predict(int reference_length, int references[], int page_no ,int frame_size,int frame[], int start);
void add_reference(int frame_size,int frame[], int reference, int current_position,int reference_length, int references[]);
void print(int frame_size,int frame[]);


#endif