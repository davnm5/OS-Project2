#ifndef _OPTIMAL_H
#define _OPTIMAL_H
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

void execute_optimal(int frame_size,int number_of_references,int mem_ref[]);
void add_reference_optimal(int frame_size,int frame[], int reference, int current_position,int reference_length, int references[]);
int predict_optimal(int reference_length, int references[], int page_no ,int frame_size,int frame[], int start);
void print_optimal(int frame_size,int frame[]);

#endif