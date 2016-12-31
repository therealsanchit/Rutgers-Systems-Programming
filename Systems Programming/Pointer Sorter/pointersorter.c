#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//tokenArray basically stores all the tokens without sorting it, resultArray is sorted tokens
typedef struct Tokenizer{
	char** tokenArray;
	int noOfTokens;
} Tokenizer;

int x = 0;


int checkWork(char* copy){
	int i;
	for(i=0;i<strlen(copy);i++){
		if(isalpha(copy[i]) != 0)
			return 1;
	}
	return -1;
}

char* checkFirst(char* copy){
	if(isalpha(copy[0]) == 0){
		copy = copy + 1;
		copy = checkFirst(copy); 	
	}
	return copy;
}
char* reWrite(char* copy, int pos){
	copy[pos] = copy[pos+1];
	
	int i;
	for(i=pos+1; i<strlen(copy);i++){
		copy[i] = copy[i+1];
	}
	return copy;
}

	/*delim = 'a' because if a delim is found it will change its value from 'a' to something else, if it dosent change value then no delim has been found in entire string */
char findDelim(char* string){ //find delim
char delim = 'a';
int i;

	for(i=0;i<strlen(string);i++){
					if(isalpha(string[i]) == 0){
							delim = string[i];
							if(isalpha(string[i+1]) == 0){//consecutive delims
								string = reWrite(string,i);
								delim = findDelim(string);
							}
						break;
					}
			}
			return delim;
}

void createTokens(char* string, Tokenizer* tokenizer){


	int check;
	char delim;
	char* copy = strdup(string); //duplicate of original string, strtok modifies strings 
	char* token;
	
	char* use;	//strtok() takes in 2 char* as arguments, use is basically char* version of delim;
	use = malloc(sizeof(char)*2);
	use[1] = '\0';

	if(checkWork(copy) == -1){
		printf("%s\n", "Enter string with atleast 1 alphabet");
		x=-1;
		return;
	}

	copy = checkFirst(copy);

	delim = findDelim(copy);

	if(delim == 'a'){ //if delim not found
		token = copy;
		int size = strlen(token);

		/* dynamically alloc memory as we find more and more tokens*/

		tokenizer->noOfTokens = 1;
		tokenizer->tokenArray = (char**) malloc(sizeof(char*) * (tokenizer->noOfTokens));
		tokenizer->tokenArray[tokenizer->noOfTokens-1] = malloc(size + 1);
		strcpy(tokenizer->tokenArray[tokenizer->noOfTokens-1],token);
		return;
 	}

	use[0] = delim;
	token = strtok(copy,use);
	int size = strlen(token);

	copy = copy + size + 1;//important step, basically removes out the token + delim found from the string. 

	tokenizer->noOfTokens = 1;
	tokenizer->tokenArray = (char**) malloc(sizeof(char*) * (tokenizer->noOfTokens));
	tokenizer->tokenArray[tokenizer->noOfTokens-1] = malloc(size + 1);
	strcpy(tokenizer->tokenArray[tokenizer->noOfTokens-1],token);



	while(token != NULL){

		if(strlen(copy) == 0)
			return;

	delim = findDelim(copy);

	if(delim == 'a'){
		token = copy;
		size = strlen(token);

		if(token == NULL){
			printf("%s\n", "No tokens Left");
			return;
		}

		tokenizer->noOfTokens++;
		tokenizer->tokenArray = (char**) realloc(tokenizer->tokenArray ,sizeof(char*) * (tokenizer->noOfTokens));
		tokenizer->tokenArray[tokenizer->noOfTokens-1] = (char*)malloc(size+1);
		strcpy(tokenizer->tokenArray[tokenizer->noOfTokens-1],token);
		return;
	}

	use[0] = delim;

		token = strtok(copy,use);
		int size = strlen(token);


		tokenizer->noOfTokens++;
		tokenizer->tokenArray = (char**) realloc(tokenizer->tokenArray ,sizeof(char*) * (tokenizer->noOfTokens));
		tokenizer->tokenArray[tokenizer->noOfTokens-1] = (char*)malloc(size+1);
		strcpy(tokenizer->tokenArray[tokenizer->noOfTokens-1],token);

	copy = copy + size + 1;

	}

	free(copy);
	free(use);

	return;
}


void sortTokens(Tokenizer* tokenizer){ // strcmp sort
	char* temp;
	int i,j;
	 for(i=0; i < tokenizer->noOfTokens-1 ; i++){
        for(j=i+1; j< tokenizer->noOfTokens; j++)
        {
            if(strcmp(tokenizer->tokenArray[i],tokenizer->tokenArray[j]) > 0)
            {
            	temp = malloc(strlen(tokenizer->tokenArray[i]) + 1);
                strcpy(temp,tokenizer->tokenArray[i]);
                tokenizer->tokenArray[i] = realloc(tokenizer->tokenArray[i],strlen(tokenizer->tokenArray[i]) + 1);
                strcpy(tokenizer->tokenArray[i],tokenizer->tokenArray[j]);
                tokenizer->tokenArray[j] = realloc(tokenizer->tokenArray[j],strlen(temp) + 1);
                strcpy(tokenizer->tokenArray[j],temp);
                free(temp);
            }
        }
    }
    return;
}

void killErr(Tokenizer* tk){ //free all dynamically alloc mem
	int i;
		for(i=0;i<tk->noOfTokens;i++){
			free(tk->tokenArray[i]);
		}

		free(tk->tokenArray);
		free(tk);

		tk->tokenArray = NULL; tk = NULL;

		return;
}

int main(int argc, char** argv){


	Tokenizer* tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));

	if(argc != 2){
		printf("%s\n", "Enter arguments as : File - String");
		return -1;
	}

	char* string = argv[1];

	if(strlen(string) == 0){
		printf("%s\n", "Please enter a string with atleast 1 letter");
		return -1;
	}

	createTokens(string, tokenizer);

	if(x==-1)
		return -1;
	sortTokens(tokenizer);


	int i;
	for(i=0;i<tokenizer->noOfTokens;i++){
		printf("%s\n", tokenizer->tokenArray[i]);
	}

	killErr(tokenizer);

	return 0;
}