#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <ctype.h>


typedef struct threadArg{

  int clientStructLength;
  int clientSocketFildes;
  struct sockaddr_in clientAdressInfo;
  char buffer[512];
  int fileIndex;

} arg;

typedef struct fileInfo{
   int flag;
   int mode;
   int fileDescriptor;
   char* fileName;
} filesInfo;
 
struct node
{
    arg* thread;
    struct node *link;
}*front, *rear;
 
// function protypes
void insert(void* data);
void delete();
int queue_size();
void check();
void* first_element();
 
// to insert elements in queue
void insert(void* data)
{
    struct node *temp;
    arg* margs = data;
    temp = (struct node*)malloc(sizeof(struct node));
    temp->thread = margs;

    temp->link = NULL;
    if (rear  ==  NULL || front == NULL)
    {
        front = rear = temp;
    }
    else
    {
        rear->link = temp;
        rear = temp;
    }    
}
 
// delete first element from queue
void delete()
{
    struct node *temp;
    temp = front;
    if (front == NULL)
    {
        printf("Queue is Empty\n");
        front = rear = NULL;
    }
    else
    {    
        front = front->link;
    }

    free(temp);
    return;
}
 
// returns first element of queue
void* first_element()
{
    if (front == NULL)
    {
        return NULL;
    }
    else {
        return front->thread;
    }
    return NULL;
}
 
// returns number of entries and displays the elements in queue
int queue_size()
{
    struct node *temp;
 
    temp = front;
    int cnt = 0;
    if (front  ==  NULL)
    {
        printf("Requests Queue is now Empty \n");
    }
    while (temp)
    {
        temp = temp->link;
        cnt++;
    }
   return cnt;
}