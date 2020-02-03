#ifndef _LIST_H
#define _LIST_H

typedef struct node Node;

typedef struct list List;

List * makelist();
void insert(int data, List * list);
void delete(int data, List * list);
void search(int data, List * list);
void update(int data_old,int data_new,List * list);
void display(List * list);
void destroy(List * list);

#endif