#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>



//Way of storing data. Every distinct char, which is not consecutively same as one char before, gets a Node.
typedef struct _Node{
  int frequency;
  char letter;
  struct _Node* next;
} Node;

//Arguments passed through a thread. Each thread allocates memory for these arguments, and are used throughout the compression process.

typedef struct threadArg{
  int index;
  char* fileName;
  int numParts;
  int size;
  char isFirst;
  int dataTaken;
  Node* head;
} arg;

int getSizeOfFile(FILE* file){
  fseek(file,0,SEEK_END);
	int size = ftell(file);
	rewind(file);
	return size;
}

void insertAtEnd(Node* head, Node* temp){
  Node* new = head;
  for(new; new->next != NULL; new = new->next) continue;    new->next = temp;
}


//Takes in a file pointer, to write to, and the arguments of the thread which calls this function.
void writeToFile(FILE* file,void* args){

arg* margs = args;

  if(margs->head == NULL){
    fputs("", file);
    return;
  }
//Each thread builds a linkedList for their partition of the original string. This traverses through each node, and appends / or writes it to a file
  Node* temp;
  for (temp = margs->head ; temp != NULL; temp = temp->next){
        char* string = malloc(sizeof(char) * 3);
        int i;

        if(temp->frequency > 2){
        	char* freq = malloc(sizeof(char) * floor(log10(abs(temp->frequency))) + 1);
        	sprintf(freq, "%d", temp->frequency);
        	string = realloc(string,sizeof(char) * floor(log10(abs(temp->frequency))) + 2);
          	for(i=0;i<floor(log10(abs(temp->frequency))) + 1;i++){
          		string[i] = freq[i];
          	}
          	free(freq);
          string[i] = temp->letter;
          string[i+1] = '\0';
        }else if(temp->frequency == 2){
          string[0] = temp->letter;
          string[1] = temp->letter;
          string[2] = '\0';
        }else{
          string[0] = temp->letter;
          string[1] = '\0';
          string[2] = '\0';
        }
        printf("Encoded into : %s\n", string);
        fputs(string, file);
        free(temp); //As this is the end of the program, we dont have use for the linkedlist, thus free each node
        free(string);
  }
fclose(file);
free(margs); //free thread arguments
return;
}

//Takes in a thread argument structure, and uses that to retrieve all necessary information.
void* processString(void* args){

  arg* margs = args;
  int size = margs->size;
  int numParts = margs->numParts;
  int index = margs->index;//which number thread it is, finds offset value/where to start scanning string from this
  char* fileName = margs->fileName;
  char* str = strdup(fileName);
  char* nFileName = strtok(str, ".");

  int x = floor(log10(abs(numParts))) + 1;

char* wFileName;

  if(numParts == 1)     wFileName = (char*)malloc(sizeof(nFileName) + 9); //writing file name for 1 process
  else                  wFileName = (char*)malloc(sizeof(nFileName) + 9 + x); //wrtite file name for multiple processes


  if(numParts == 1)    sprintf(wFileName, "%s_txt_LOLST", nFileName+1);
  else                 sprintf(wFileName, "%s_txt_LOLST%d", nFileName+1, index);

  FILE* nfile = fopen(wFileName, "a");

  free(wFileName);


  FILE* file = fopen(fileName, "r");


  char* string = (char*)calloc(1,size+1); //+1 for the null termintor
  if(!string)
    fclose(file),fputs("memory alloc fails",stderr),exit(1);
  if( 1!=fread(string , size, 1 , file))
  fclose(file),free(string),fputs("entire read fails",stderr),exit(1);

  string[size] = '\0';

  //printf("%s\n", string);       
   fclose(file);

//when a string cannot be split evenly, the extra workload is added to the firstworker.
    int length = size;
    int rem = (length % numParts);

    int lengthEachString = (length - rem) / numParts;//makes it divisible, now all workers have same length 
    int lengthFirstString = rem + lengthEachString;//special worker working so hard

    int offset;


    if(numParts > length){
      printf("%s\n", "Number to file splits is more than length of string, You will have a 'string.length' number of data files, and excess");
      lengthFirstString = 1;
      lengthEachString = 1;
    }

    Node* head;
    Node* follow; //last node basically


    if(index == 0) offset = 0;
    else if(index == 1) offset = lengthFirstString;
    else { offset = (lengthFirstString + ((index-1) * lengthEachString)); }


              char letter; int freq;
              char lastLetter;//case- String: jjj  jjj , encoded = 6j. Used to handle those
              Node* temp;
              margs->dataTaken = offset;
              int max;


              if(offset != 0) max = margs->dataTaken + lengthEachString;
              else max = margs->dataTaken + lengthFirstString;

              while(margs->dataTaken != (max)){
              letter = string[margs->dataTaken]; freq = 0;

              
              //illegal char found
              if(!((letter>='a' && letter<='z') || (letter>='A' && letter<='Z'))){
                    if(!isspace(letter)) printf("%s\n", "Illegal character found, Skipping and Compressing");
                margs->dataTaken++;
                continue;
              }

              temp = (Node*)calloc(1,sizeof(Node));

              if(margs->isFirst == 1) margs->head = temp;
//keep scanning until different character found or reaching max limit for thread.
              while((string[margs->dataTaken + freq] == letter) && ((margs->dataTaken + freq) != (max))) freq++;

             if(letter == lastLetter){
                follow->frequency += freq;
                margs->dataTaken += freq;
                free(temp);
                continue;
              }
              if(margs->isFirst == 1){ follow = head; margs->isFirst = 0; continue; }//for head node pointer.

              temp->letter = letter;
              temp->frequency = freq;
              margs->dataTaken += freq;
              lastLetter = letter;

              insertAtEnd(margs->head,temp);
              follow = temp;
        }
      

        //have everything in linked list, need to print to file  
          writeToFile(nfile, margs);
          free(string);

  pthread_exit(NULL);

  return NULL;
}


void createThreads(pthread_t* threadHolder, int numParts, char* fileName, int size){

size_t i;
  for (i = 0; i < numParts; i++) {
  arg *args = calloc(1,sizeof *args);
  args->index = i;
  args->isFirst = 1;
  args->fileName = fileName;
  args->numParts = numParts;
  args->size = size;
  args->head = (Node*)(calloc(1,sizeof(Node)));
  args->dataTaken = 0;
  pthread_create(&threadHolder[i], 0, processString, args);
  }

  for (i = 0; i < numParts; i++) {
    pthread_join(threadHolder[i], NULL);
  }

  

  return;
}



int main(int argc, char** argv){

  //arguments should be 3, filename, file to read, number of parts
  if (argc != 3){
    printf("%s\n", "Please Enter Correct Number of Arguments. In the Format : ");
    printf("%s\n", "./executableName <file to compress>, <number of parts>");
    return -1;
  }

  int numParts = atoi(argv[2]);
  char* fileName = argv[1];


  if(numParts <= 0) {
    printf("%s\n", "Num Of Splits should be >= 1");
    return -1;
  }
  pthread_t* threadHolder = (pthread_t*)(malloc(sizeof(pthread_t) * numParts));

  FILE* file = fopen(fileName, "r");
  int size = 0;
  size = getSizeOfFile(file); //should give total size of file.
  fclose(file);
  

  if(size < 3) {  //cannot be compressed more
    printf("%s\n", "This file cannot be compressed more");
    return -1;
  }

  createThreads(threadHolder, numParts, fileName, size);
  free(threadHolder);

  return 0;
  }

