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

typedef struct _dict{
  char** key;
  char* code;
  int keyIndex;
  int length;
  int start;
  int end;
} dict;



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
char* KMPSearch(char* compressedString, dict* dict, char* outputCode)
{ 
    int taken=0;


    for(int q=dict->keyIndex +2;q>=0; q--){

    char* pat = dict->key[q];
    printf("%s\n", pat);
    int M = strlen(pat);
    int N = strlen(outputCode);
 
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
 
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
 
    int i = 0;  // index for txt[]
    int j  = 0;  // index for pat[]
    while (i < N)
    {

        if (pat[j] == outputCode[i])
        {
            j++;
            i++;
        }
        if (j == M)
        {
            printf("Found pattern at index %d \n", i-j);
            

            int x;
            x=i-j;
            outputCode[x] = getCodeOfKey(pat, dict);


            int offset = strlen(pat);
            int z =0;
            for(z=x+1;z< x + offset;z++){
                outputCode[z] = ' ';
            }

            outputCode = makeOutputString(outputCode);
            printf("%s\n", outputCode);

            N = strlen(outputCode);
            j = lps[j-1];
        }
 
        else if (i < N && pat[j] != outputCode[i])
        {
          
            if (j != 0)
                j = lps[j-1];
            else
                i = i+1;
        }
    }
  }

  return outputCode;
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





void makeDictionary(char* compressedString, dict* dictionary, char* fileName, int index, int numParts){

  dict* dict = dictionary;
  dict->key = (char**)(malloc(sizeof(char*) * 3));

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
  char* outputCode = (char*)(calloc(1,strlen(compressedString)));
  outputCode = strcpy(outputCode, compressedString);
  printf("%s\n", outputCode);

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
  outputCode = KMPSearch(outputCode, dict, outputCode);
  printf("Final String %s\n", outputCode);

  char* str = strdup(fileName);
  char* nFileName = strtok(str, ".");

  int x = floor(log10(abs(numParts))) + 1;

char* wFileName;
char* dFileName;

  if(numParts == 1)   {  wFileName = (char*)malloc(sizeof(nFileName) + 9); dFileName = (char*)malloc(sizeof(nFileName) + 11); }//writing file name for 1 process
  else                {  wFileName = (char*)malloc(sizeof(nFileName) + 9 + x);  dFileName = (char*)malloc(sizeof(nFileName) + 11 + x); }//wrtite file name for multiple processes


  if(numParts == 1)  {  sprintf(wFileName, "%s_LZW_LOLSP", nFileName+1);  sprintf(dFileName, "%s_LZWRLEDict", nFileName+1);}
  else               {  sprintf(wFileName, "%s_LZW_LOLSP%d", nFileName+1, index); sprintf(dFileName, "%s_LZWRLEDict%d", nFileName+1, index); }

  FILE* nfile = fopen(wFileName, "w");

  free(wFileName);

  fprintf(nfile, "%s\n", outputCode);

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


void writeToFile(FILE* file, Node* head, int size, int index, char* fileName, int numParts){
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

    dict* dictionary = (dict*)(calloc(1,sizeof(dict)));
    makeDictionary(compressedString, dictionary, fileName, index, numParts);
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
          writeToFile(nfile, head, size, index, fileName, numParts);
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
