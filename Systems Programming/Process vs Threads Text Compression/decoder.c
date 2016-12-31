#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>

typedef struct _dict{
  char** key;
  char* code;
  int keyIndex;
} dictc;
dictc* dict;


int getSizeOfFile(FILE* file){
  fseek(file,0,SEEK_END);
	int size = ftell(file);
	rewind(file);
	return size;
}

char* getKeyFromCode(char c){

	for(int i=0; i<dict->keyIndex + 3;i++){
        if(dict->code[i] == c) return dict->key[i];
  }

  return 0;
}

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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char* decodeString(FILE* rfile, char* originalString){

	int offset = 0;



	char* compressedString = calloc(1,getSizeOfFile(rfile)+1);
if(!compressedString)
    fclose(rfile),fputs("memory alloc fails",stderr),exit(1);
  if( 1!=fread(compressedString , getSizeOfFile(rfile), 1 , rfile))
  fclose(rfile),free(compressedString),fputs("entire read fails",stderr),exit(1);


printf("%s\n", compressedString);

for(int i=0; compressedString[i] != '\0'; i++){
	char c = compressedString[i];

	char* key = getKeyFromCode(c);

	if(key != 0){
		printf("%s - %c\n", key, compressedString[i]);
		for(int j=0;j<strlen(key);j++){
			originalString[offset] = key[j];
			offset++;
		}
	}else{
		originalString[offset] = c;
		offset++;
	}
	printf("%s\n", originalString);
}
originalString = makeOutputString(originalString);
printf("Decompressed: %s\n", originalString);

}

void makeDictionary(FILE* file, FILE* rfile, char* originalString){

int i=0;
int index = 0;

       if (file == NULL)	
        exit(EXIT_FAILURE);

   const size_t line_size = 20;
	char* line = malloc(line_size);
	while (fgets(line, line_size, file) != NULL)  {

    if(i==0) {
    	dict->keyIndex = atoi(line); printf("%d\n", dict->keyIndex); dict->key = (char**)(calloc(1,sizeof(char*) * dict->keyIndex+3)); dict->code = (char*)(calloc(1,sizeof(char) * (dict->keyIndex+3))); i++;
    	continue;
    }
    
    	char** str = str_split(line, '-');
    	dict->key[index] = malloc(sizeof(char) * strlen(str[0]) + 1);
    	dict->key[index] = strcpy(dict->key[index], str[0]);
    	printf("%s\n", dict->key[index]);
    	char c = str[1][1];
    	dict->code[index] = c;
    	printf("%c\n", dict->code[index]);  
    	index++;  	
    	free(str[0]);
    	free(str[1]);
    	free(str);
    
}


free(line);
//fclose(file);    // dont forget to free heap memory
    return;

}


int main(int argc, char** argv){

	if(argc != 3){
		printf("%s\n", "Exec. Compressed. Dict");
	}


char* originalString = calloc(1, 10000);
dict = (dictc*)(calloc(1,sizeof(dictc)));
FILE* file = fopen(argv[2], "r");
FILE* rfile = fopen(argv[1], "r");
makeDictionary(file,rfile,originalString);
fclose(file);


decodeString(rfile, originalString);



}