#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>


typedef struct _Node{
  int frequency;
  char letter;
  struct _Node* next;
} Node;



void insertAtEnd(Node* head, Node* temp){
  Node* new = head;
  for(new; new->next != NULL; new = new->next) continue;    new->next = temp;
}


int getSizeOfFile(FILE* file){
  fseek(file,0,SEEK_END);
	int size = ftell(file);
	rewind(file);

	return size;
}

void writeToFile(FILE* file, Node* head, int size){
Node* temp;
char* compressedString = (char*)(calloc(1,size + 1));

  for (temp = head; temp != NULL; temp = temp->next){
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
        free(temp);
        compressedString = strcat(compressedString, string);
	    free(string);
  }
  fclose(file);
return;
}

void* processString(int index, char* fileName, int numParts, int size){
              short isFirst;
              isFirst = 1;


  char* str = strdup(fileName);
  char* nFileName = strtok(str, ".");


  int x = floor(log10(abs(numParts))) + 1;

  char* wFileName;

  if(numParts == 1)     wFileName = (char*)malloc(sizeof(nFileName) + 9); //writing file name for 1 process
  else                  wFileName = (char*)malloc(sizeof(nFileName) + 9 + x); //wrtite file name for multiple processes


  if(numParts == 1)    sprintf(wFileName, "%s_txt_LOLSP", nFileName+1);
  else                 sprintf(wFileName, "%s_txt_LOLSP%d", nFileName+1, index);

  FILE* nfile = fopen(wFileName, "a");

  free(wFileName);

  FILE* file = fopen(fileName, "r");


  char* string = (char*)calloc(1,size+1); //+1 for the null termintor
  if(!string)
    fclose(file),fputs("memory alloc fails",stderr),exit(1);
  if( 1!=fread(string , size, 1 , file))
  fclose(file),free(string),fputs("entire read fails",stderr),exit(1);

  string[size] = '\0';

  fclose(file);

    int length = size;

     int rem = (length % numParts);

    int lengthEachString = (length - rem) / numParts;
    int lengthFirstString = rem + lengthEachString;

    int offset;

  if(numParts > length){
      printf("%s\n", "Number to file splits is more than length of string, You will have a 'string.length' number of data files, and excess empty files");
      lengthFirstString = 1;
      lengthEachString = 1;
    }

    Node* head;
    Node* follow;


    if(index == 0) offset = 0;
    else if(index == 1) offset = lengthFirstString;
    else { offset = (lengthFirstString + ((index-1) * lengthEachString)); }

    printf("Offset is:%d\n", offset);

              char letter; int freq;
              char lastLetter = NULL;
              Node* temp;
              int dataTaken = offset;
              int max;


              if(offset != 0) max = dataTaken + lengthEachString;
              else max = dataTaken + lengthFirstString;

              while(dataTaken != (max)){
              letter = string[dataTaken]; freq = 0;
              
              //illegal char found
              if(!((letter>='a' && letter<='z') || (letter>='A' && letter<='Z'))){
                    if(!isspace(letter)) printf("%s\n", "Illegal character found, Skipping and Compressing");
                dataTaken++;
                continue;
              }

              temp = (Node*)calloc(1,sizeof(Node));


              while((string[dataTaken + freq] == letter) && ((dataTaken + freq) != (max))) freq++;

             if(letter == lastLetter){
                follow->frequency += freq;
                dataTaken += freq;
                free(temp);
                continue;
              }

              if(isFirst == 1) {   head = temp; isFirst = 0; continue;}


              temp->letter = letter;
              temp->frequency = freq;
              dataTaken += freq;
              lastLetter = letter;

              insertAtEnd(head,temp);
              follow = temp;
        }
      

        //have everything in linked list, need to print to file  
          writeToFile(nfile, head,size);
          free(string);

  return NULL;
}


int main(int argc, char** args){

  char* fileName = args[0];
  int numParts = atoi(args[1]);
  int index = atoi(args[2]);
  printf("Child Number   : %d\n", index);


  FILE* file = fopen(fileName, "r");
  int size = getSizeOfFile(file);
  fclose(file);

  processString(index, fileName, numParts, size);


  return 0;

}
