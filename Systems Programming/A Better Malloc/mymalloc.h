

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _MYMALLOC_H
#define _MYMALLOC_H

#define malloc(x) my_malloc(x,__FILE__,__LINE__)
#define free(x) my_free(x,__FILE__,__LINE__)


int dataTaken;
short mallocError;
short freeError;
short lastNode;
short indexChange;

typedef struct Node_{
char* data;
struct Node_* next;
int size;
short inUse;
}Node;

int checkAll();
void hitMalloc();
void hitFree();
void linkNode(Node* head, Node* tail, int dataWanted);
void performMerge();
void merge(Node* head, Node* tail);
void* my_malloc(int x,char* file, int line);
void my_free(void* ptr, char* file, int line);
int dataTakenUntil(Node* end);
int dataTakenUptil(Node* end);
Node* returnLastNode();
int numofNodes();
Node *reverse(Node *head);



#endif