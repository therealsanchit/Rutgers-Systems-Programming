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


typedef struct _dict{
  char** key;
  char* code;
  int keyIndex;
  int length;
  int start;
  int end;
} dict;

//Arguments passed through a thread. Each thread allocates memory for these arguments, and are used throughout the compression process.
typedef struct threadArg{
  int index;
  char* fileName;
  int numParts;
  int size;
  char isFirst;
  int dataTaken;
  Node* head;
  char* compressedString;
  dict* dictionary;
  char* outputCode;
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

int checkIfContained(dict* dict, char* string){
  int i;
    for(i=0;i<dict->keyIndex + 3;i++){
      if(strcmp(string,dict->key[i]) == 0) {
        printf("%s", "Contained");
          return 1;
    }
}
    printf("%s\n", "Not Contained");
    return 0;
}

char getCodeOfKey(char* key, dict* dict){

  for(int i=0;i<dict->keyIndex + 3;i++){
        if(strcmp(dict->key[i], key) == 0) return dict->code[i];
  }

  return 0;

}


char* makeOutputString(char* madeString);
void computeLPSArray(char *pat, int M, int *lps);
 // Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(char *pat, int M, int *lps)
{

    printf("%s\n", "Insde lps");
    // length of the previous longest prefix suffix
    int len = 0;
 
    lps[0] = 0; 
 
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
 
            if (len != 0)
            {
                len = lps[len-1];

            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Prints occurrences of txt[] in pat[]
char* KMPSearch(char* compressedString, void* args)
{ 
    arg* margs = args;
    dict* dict = margs->dictionary;

    int taken=0;


    for(int q=dict->keyIndex +2;q>=0; q--){

    char* pat = dict->key[q];
    printf("%s\n", pat);
    int M = strlen(pat);
    int N = strlen(margs->outputCode);
 
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
 
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
 
    int i = 0;  // index for txt[]
    int j  = 0;  // index for pat[]
    while (i < N)
    {

        if (pat[j] == margs->outputCode[i])
        {
            j++;
            i++;
        }
        if (j == M)
        {
            printf("Found pattern at index %d \n", i-j);
            

            int x;
            x=i-j;
            margs->outputCode[x] = getCodeOfKey(pat, dict);


            int offset = strlen(pat);
            int z =0;
            for(z=x+1;z< x + offset;z++){
                margs->outputCode[z] = ' ';
            }

            margs->outputCode = makeOutputString(margs->outputCode);
            printf("%s\n", margs->outputCode);

            N = strlen(margs->outputCode);
            j = lps[j-1];
        }
 
        else if (i < N && pat[j] != margs->outputCode[i])
        {
          
            if (j != 0)
                j = lps[j-1];
            else
                i = i+1;
        }
    }
  }

  return margs->outputCode;
}
  
// Driver program to test above function


char* makeOutputString(char* madeString){


char* i = madeString;
  char* j = madeString;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;

return madeString;

}




char* getNextToken(char* compressedString, dict* dict){
    int start = dict->start;
    int end = dict->end;
    int x = dict->length;


    printf("Starting Index %d, with letter %c\n", dict->start, compressedString[start]);
    printf("Ending Index %d, with letter %c\n", dict->end, compressedString[end]);
    printf("String length %d\n", x);
    char* string = calloc(1,dict->length+1);
    int i;

    if(strlen(compressedString) < 4) {
      printf("%s\n", "String Size not enough");
      return NULL;
}
    for (i = start; i <= end && compressedString[i] != '\0'; ++i)
    {
          string[i-start] = compressedString[i];
          printf("Letter found %c\n", string[i]);
    }

    if(compressedString[i] != '\0') //string[i+1] = '\0';

    printf("%s\n", string);

    int y = checkIfContained(dict, string);

    if(y == 1){
      if(compressedString[i] == '\0'){
          printf("%s", "String over"); return NULL;
        }
      dict->end++;
      dict->length++;
      free(string);
      string = getNextToken(compressedString, dict);
    }else{

      dict->start = dict->end;
      dict->end += (dict->length-1);
}
      return string;
}





void makeDictionary(char* compressedString, void* args){

  arg* margs = args;
  (margs->dictionary)->key = (char**)(malloc(sizeof(char*) * 3));
  dict* dict = margs->dictionary;

//initalize dictionary
  dict->key[0] = malloc(2);
  dict->key[1] = malloc(2);
  dict->key[2] = malloc(2);

  dict->key[0][0] = compressedString[0];
  dict->key[0][1] = '\0';
  dict->key[1][0] = compressedString[1];
  dict->key[1][1] = '\0';
  dict->key[2][0] = compressedString[2];
  dict->key[2][1] = '\0';

  dict->keyIndex = 0;
  dict->start = 0;
  dict->end = 1;
  dict->length = 2;
  char* string;
  margs->outputCode = (char*)(calloc(1,strlen(compressedString)));
  margs->outputCode = strcpy(margs->outputCode, compressedString);
  printf("%s\n", margs->outputCode);

printf("%s - %s - %s\n", dict->key[0],dict->key[1],dict->key[2]);
  int i = 3;

  while(i > 0 && i != 32){

    if(dict->start > strlen(compressedString) || (dict->end+dict->length -1) > strlen(compressedString)) break;
    string = getNextToken(compressedString, dict);
    if(string == NULL) break;
    i++;
          printf("%s\n", string);
      dict->key = realloc(dict->key, sizeof(char*) * i);
      dict->key[dict->keyIndex + 3] = malloc(sizeof(char) * dict->length);
      dict->key[dict->keyIndex + 3] = strcpy(dict->key[dict->keyIndex + 3], string);
      free(string);
      dict->keyIndex++;
  }

  dict->code = (char*)malloc(sizeof(char) * (dict->keyIndex + 3));
  int y = 0;
  int z = 0;
  for (int i = 0; i < dict->keyIndex+3; ++i)
  {
    if(33 + i > 47 && 33 + i < 53){
      dict->code[i] = 91 + y;
      y++;
    }
    else if(33 + i == 53) dict->code[i] = 96;
    else if(33 + i > 53 && 33 + i < 58){
      dict->code[i] = 123 + z;
      z++;
    }else dict->code[i] = 33 + i;
    printf("This is the key  : %s   And this is the code : %c\n", dict->key[i], dict->code[i]);
  }

  printf("%s\n", "Outputting string");

  //makeOutputString(compressedString, margs);
  margs->outputCode = KMPSearch(margs->outputCode, margs);
  printf("Final String %s\n", margs->outputCode);

  char* fileName = margs->fileName;
  char* str = strdup(fileName);
  char* nFileName = strtok(str, ".");

  int x = floor(log10(abs(margs->numParts))) + 1;

char* wFileName;
char* dFileName;

  if(margs->numParts == 1)   {  wFileName = (char*)malloc(sizeof(nFileName) + 9); dFileName = (char*)malloc(sizeof(nFileName) + 11); }//writing file name for 1 process
  else                {  wFileName = (char*)malloc(sizeof(nFileName) + 9 + x);  dFileName = (char*)malloc(sizeof(nFileName) + 11 + x); }//wrtite file name for multiple processes


  if(margs->numParts == 1)  {  sprintf(wFileName, "%s_LZW_LOLST", nFileName+1);  sprintf(dFileName, "%s_LZWRLEDict", nFileName+1);}
  else               {  sprintf(wFileName, "%s_LZW_LOLST%d", nFileName+1, margs->index); sprintf(dFileName, "%s_LZWRLEDict%d", nFileName+1, margs->index); }

  FILE* nfile = fopen(wFileName, "w");

  free(wFileName);

  fprintf(nfile, "%s\n", margs->outputCode);

  fclose(nfile);

  FILE* dfile = fopen(dFileName, "w");

  fprintf(dfile, "%d\n", dict->keyIndex);
  for (int i = 0; i < dict->keyIndex+3; ++i)
  {
    fprintf(dfile, "%s - %c\n", dict->key[i], dict->code[i]);
  }

  fclose(dfile);


return;

}


//Takes in a file pointer, to write to, and the arguments of the thread which calls this function.
void writeToFile(FILE* file,void* args){

arg* margs = args;


  if(margs->head == NULL){
    fputs("", file);
    return;
  }
  Node* temp;
  char* compressedString = (char*)(calloc(1,margs->size + 1));
  int offset = 0;

//Each thread builds a linkedList for their partition of the original string. This traverses through each node, and appends / or writes it to a file
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
   //     printf("Encoded into : %s\n", string);
        fputs(string, file);
        free(temp); 
        compressedString = strcat(compressedString, string);
        free(string);
        //As this is the end of the program, we dont have use for the linkedlist, thus free each node
  }
fclose(file);


printf("this is it: %s\n", compressedString);
margs->compressedString = compressedString;
makeDictionary(compressedString, margs);
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

  FILE* nfile = fopen(wFileName, "ar");

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

   // printf("Offset is:%d\n", offset);

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
  for (i= 0; i < numParts; i++) {
  arg *args = calloc(1,sizeof *args);
  args->index = i;
  args->isFirst = 1;
  args->fileName = fileName;
  args->numParts = numParts;
  args->size = size;
  args->head = (Node*)(calloc(1,sizeof(Node)));
  args->dataTaken = 0;
  args->dictionary = (dict*)(calloc(1,sizeof(dict)));
  pthread_create(&threadHolder[i], 0, processString, args);
  //so we gotta join simoltanously

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