#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <ctype.h>


#define unrestricted 0
#define exclusive    1
#define transaction  2
#define INVALID_FILE_MODE 9998


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



void createThreads(void* data);
void setError(char* e);
void* initalizeSocket(void* d);
char** getTokens(char* c);
int simulateNetopen(char** t);
int recieveRequest();
void* decodeRequest(void* data);
int simulateNetread(char** t, char* b);
void sendError(void* d);
int checkCanOpen(char* p, int d, int x);
int getFileIndexFromFileDescp();
int checkTransaction(char* pathName);
int checkExclusive(char* pathName);
void startQueue(void* d);
void insert(void* data);
void delete();
int queue_size();
void check();
void* first_element();
void* checkQueue();


pthread_t queueChecker;


static int portNumber = 9999;

short socketInitalized = 0;
int totalThreads = 0;
int totalFiles = 0;

int serverSocketFildes;
struct sockaddr_in serverAdressInfo;


pthread_t* threadHolder;
filesInfo* fileInfo;

extern int h_errno;
extern int errno;


void sendError(void* data){

arg* margs = data;

int errorLength = snprintf(NULL, 0, "%d", errno);

char* buffer = (char*)calloc(1, sizeof(char) * errorLength + 3);

buffer[0] = 'e';
buffer[1] = ';';

sprintf(&buffer[2], "%d", errno);
buffer[2 + errorLength] = '\0';

if(errno != INVALID_FILE_MODE) 
printf("%s\n", "Sending Error Message To Client");

if(send(margs->clientSocketFildes, buffer, strlen(buffer), 0) < 0){
	setError("Could Not Send Error Message To Client");
}

return;

}


void setError(char* errorMessage){
	perror(errorMessage);
	printf("ErrNo is : %d\n", errno);

	return;
}



void* initalizeSocket(void* data){


	struct sockaddr_in cli_addr;
	int clilen;

	serverSocketFildes = socket(AF_INET, SOCK_STREAM, 0);
	
	if(serverSocketFildes < 0){
		setError("Socket Connection Failed");
		return NULL;
	}

	bzero((char*)&serverAdressInfo, sizeof(serverAdressInfo));

	serverAdressInfo.sin_port = htons(portNumber);
	serverAdressInfo.sin_family = AF_INET;
	serverAdressInfo.sin_addr.s_addr = INADDR_ANY;

	if(bind(serverSocketFildes, (struct sockaddr*) &serverAdressInfo, sizeof(serverAdressInfo)) < 0){
		setError("Error on Binding");
		return NULL;
	}

	listen(serverSocketFildes, 5);
	socketInitalized = 1;

	printf("%s\n", "SOCKET INITALIZED");
	totalThreads++;	

 	int newsockfd;

	 while (1) {

      clilen = sizeof(cli_addr);
      newsockfd = accept(serverSocketFildes, (struct sockaddr *) &cli_addr, &clilen);
		
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }

		threadHolder = (pthread_t*)realloc(threadHolder,sizeof(pthread_t*) * (totalThreads+1));
		arg* margs = calloc(1, sizeof(arg));
		margs->clientStructLength = clilen;
		margs->clientSocketFildes = newsockfd;
		bzero(margs->buffer,512);

		int cc = recv(margs->clientSocketFildes, margs->buffer, sizeof(margs->buffer), 0);

		if(cc < 0){
			setError("Receving from server failed");
			free(margs);
			return NULL;
		}

		pthread_create(&threadHolder[totalThreads], 0, &decodeRequest, margs);
		pthread_join(threadHolder[totalThreads], NULL);  
		totalThreads++;  
}

return NULL;
}

char** getTokens(char* b){

	char** tokens;
	char* buffer = strdup(b);

	if(buffer[0] == '2'){
		char* first = strtok(buffer, ";"); //which function to run
		char* second = strtok(NULL, ";"); //pathname
		char* third = strtok(NULL, ";");  //flags
		tokens = (char**)(malloc(sizeof(char*) * 3));
		tokens[0] = first;
		tokens[1] = second;
		tokens[2] = third;

	}else if(buffer[0] == '1' || buffer[0] == '3'){

		char* first = strtok(buffer, ";"); //which function to run
		char* second = strtok(NULL, ";"); //pathname
		char* third = strtok(NULL, ";");  //flags
		char* fourth = strtok(NULL, ";");

		tokens = (char**)(malloc(sizeof(char*) * 4));
		tokens[0] = first;
		tokens[1] = second;
		tokens[2] = third;
		tokens[3] = fourth;

	}else if( buffer[0] == '4'){
		char* first = strtok(buffer, ";");
		char* second = strtok(NULL, ";");

		tokens = (char**)(malloc(sizeof(char*) * 4));
		tokens[0] = first;
		tokens[1] = second;
	}

	return tokens;

}

//checks if a given file is open in transaction mode
int checkTransaction(char* pathName){
int i;

	for (i = 0; i < totalFiles; ++i)
	{

		if(strcmp(fileInfo[i].fileName, pathName) == 0){
			if(fileInfo[i].fileDescriptor != -1){
				if(fileInfo[i].mode != transaction) continue;
				else return 1;
			}else continue;
	}
}
	return -1;
}

int checkFileOpen(char* pathName){
int i;
	for (i = 0; i < totalFiles; ++i)
	{
		if(strcmp(fileInfo[i].fileName, pathName) == 0){
			if(fileInfo[i].fileDescriptor != -1){
				return 1;
			}else continue;
		}else continue;
	}

	return -1;
}

int checkExclusive(char* pathName){
int i;
	for (i = 0; i < totalFiles; ++i)
{
		
		if(strcmp(fileInfo[i].fileName, pathName) == 0) {

			if(fileInfo[i].fileDescriptor != -1){

				if(fileInfo[i].mode == exclusive){

					if(fileInfo[i].flag == O_RDWR || fileInfo[i].flag == O_WRONLY){

						return 1;

				}else continue;

			}else continue;

		}else continue;

	}else continue;

}

	return -1;

}

int checkUnrestrictedWrite(char* pathName){
int i;
	for (i = 0; i < totalFiles; ++i)
	{
		
		if(strcmp(fileInfo[i].fileName, pathName) == 0) {

			if(fileInfo[i].fileDescriptor != -1){

				if(fileInfo[i].mode == unrestricted){

						if(fileInfo[i].flag == O_WRONLY || fileInfo[i].flag == O_RDWR)

							return 1;

				}else continue;

			}else continue;

		}else continue;

	}

	return -1;
}


int checkCanOpen(char* pathName, int flag, int mode){


	switch(mode){

		case transaction : 
							if(checkFileOpen(pathName) == -1)  return 1;
							else return -2;

		case exclusive : 	
							if(checkTransaction(pathName) == -1){
								if(flag == O_RDONLY) return 1;
									if(checkExclusive(pathName) == -1){
										if(checkUnrestrictedWrite(pathName) == -1){
											return 1;
									} return -1;
								}return -1;

							}return -1;
			
		case unrestricted :  
							if(checkTransaction(pathName) == -1){

								if((checkExclusive(pathName) == 1) && flag == O_RDONLY) { return 1; }
							
								if(checkExclusive(pathName) == -1)   return 1;
							}
							
							return 0;

		default : 			printf("%s\n", "Could not find mode");	return -5;				

	}
}
			




int simulateNetopen(char** tokens){

	char* pathName = tokens[1];
	char* flags = tokens[2];
	char* modes = tokens[3];

	int flag = atoi(flags);
	int mode = atoi(modes);

	int fileDescriptor;


	if(totalFiles == 0){ 
	
	fileInfo = (filesInfo*)(malloc(sizeof(filesInfo) * totalFiles+ 1));
	fileDescriptor = open(pathName, flag, mode);
			if(fileDescriptor < 0) { setError("Error while opening file"); return -1;}
	fileInfo[totalFiles].fileName = (char*)(malloc(strlen(tokens[1] + 1)));
	fileInfo[totalFiles].fileName = strcpy(&fileInfo[totalFiles].fileName[0], tokens[1]);
	fileInfo[totalFiles].fileName[strlen(tokens[1])] = '\0';
	fileInfo[totalFiles].fileDescriptor = fileDescriptor;
	fileInfo[totalFiles].mode = mode;
	fileInfo[totalFiles].flag = flag;
	totalFiles++;
	return fileDescriptor;
	} 

	if(totalFiles > 0){

			int x = checkCanOpen(pathName, flag, mode);
		
			if(x == 1) {

			fileInfo = (filesInfo*)realloc(fileInfo, sizeof(filesInfo) * (totalFiles + 1));

			fileDescriptor = open(pathName, flag, mode);
					if(fileDescriptor < 0) { setError("Error while openning file"); return -1; }
		 	fileInfo[totalFiles].fileName = (char*)(malloc(strlen(tokens[1] + 1)));
		 	fileInfo[totalFiles].fileName = strcpy(&fileInfo[totalFiles].fileName[0], tokens[1]);
		 	fileInfo[totalFiles].fileName[strlen(tokens[1])] = '\0';
		 	fileInfo[totalFiles].fileDescriptor = fileDescriptor;
		 	fileInfo[totalFiles].mode = mode;
		 	fileInfo[totalFiles].flag = flag;
		 	totalFiles++;
			}

			if( x < 1 ){
					errno = INVALID_FILE_MODE;
					return -1;
		}

	}

	return fileDescriptor;
}

int simulateNetread(char** tokens, char* buffer){

	int fileDescriptor = atoi(tokens[1]);
	int bytesToRead = atoi(tokens[2]);

	lseek(fileDescriptor, 0, SEEK_SET);

	int x = read(fileDescriptor, buffer, bytesToRead);

	if(x < 0){
		setError("Read failed from file handle");
			return -1;
	}

	printf("Number of bytes read : %d\n", x);	
	return x;
}

int simulateNetwrite(char** tokens){

	int fileDescriptor = atoi(tokens[1]);
	char* b = tokens[2];
	char* buffer = strdup(b);
	int stringLength = strlen(buffer);
	int bytesToWrite = atoi(tokens[3]);

	char* tempBuffer = (char*)(calloc(1,sizeof(char) * bytesToWrite));
	int i;

	for (i = 0; i < stringLength; ++i)
		tempBuffer[i] = buffer[i];

	for(i; i < bytesToWrite; i++) tempBuffer[i] = ' ';

	int x = write(fileDescriptor, tempBuffer, bytesToWrite);

	if(x < 0){
		setError("Read failed from file handle");
		free(tempBuffer);
		return -1;
	}

	printf("Number of bytes written : %d\n", x);
	free(tempBuffer);
	return x;
}

int simulateNetclose(char** tokens){
	int fileDescriptor = atoi(tokens[1]);

	int x = close(fileDescriptor);


	if(x < 0){
		setError("Close Failed");
		if(errno == EBADF) 
			return -1;
	}


	if(x==0) return x; //good
}

int getFileIndexFromFileDescp(int fileDescriptor){
	int i;
	for (i = 0; i < totalFiles; ++i)
	{
		if(fileDescriptor == fileInfo[i].fileDescriptor) return i;
	}

	return -1;
}
void* decodeRequest(void* data){
	arg* margs = data;


	if(margs->buffer[0] == '1'){ //netopen - read - write - close //this will open the file given pathname
		char** tokens = getTokens(margs->buffer);
		 int x = simulateNetopen(tokens);
	 		free(tokens);

	 		int returnFileDescriptor = htonl(x);
		 	write(margs->clientSocketFildes, &returnFileDescriptor, sizeof(returnFileDescriptor));

	 		if(x > 0) {	
			close(margs->clientSocketFildes);
			free(margs);
			pthread_exit(NULL);
	 		}

		 if(x < 0){

		 	if(errno == INVALID_FILE_MODE){
		 		startQueue(margs);
		 		sendError(margs);
				pthread_exit(NULL);
				}

		 	else if(errno == EACCES || errno == EINTR || errno == EISDIR || errno == ENOENT || errno == EROFS)
				sendError(margs);
				close(margs->clientSocketFildes);
				free(margs);
				pthread_exit(NULL);
		 }
	}else if(margs->buffer[0] == '2'){

			char* buffer;
			char** tokens = getTokens(margs->buffer);

			int fsize = lseek(atoi(tokens[1]), 0, SEEK_END);
			buffer = (char*)(calloc(1,sizeof(char) * fsize));

			int x = simulateNetread(tokens, buffer);

			int l = snprintf(NULL, 0, "%d", x);

			char* tempBuffer = calloc(1, sizeof(char) * (x + 2 + 1 + l + 1));

			tempBuffer[0] = '2';
			tempBuffer[1] = ';';
			int i;

			for (i = 0; i < strlen(buffer); ++i) tempBuffer[2 + i] = buffer[i];
			
			tempBuffer[2 + i] = ';';
			sprintf(&tempBuffer[3+i], "%d", x);
			tempBuffer[3 + i + l] = '\0';

			if(send(margs->clientSocketFildes, tempBuffer, strlen(tempBuffer), 0) < 0){
				setError("Could not send buffer to client");
			}
				if(x < 0){
				if(errno == ETIMEDOUT || errno == EBADF || errno == ECONNRESET) sendError(margs);
			}

			free(buffer);
			free(tempBuffer); 
			free(tokens);
			close(margs->clientSocketFildes);
			free(margs);
			pthread_exit(NULL);
	}else if(margs->buffer[0] == '3'){

			char** tokens = getTokens(margs->buffer);
			int x = simulateNetwrite(tokens);

		
				int returnBytesWritten = htonl(x);
				write(margs->clientSocketFildes, &returnBytesWritten, sizeof(returnBytesWritten));

		if(x < 0){
				if(errno == ETIMEDOUT || errno == EBADF || errno == ECONNRESET) sendError(margs);
			}


	free(tokens);
	close(margs->clientSocketFildes);
	free(margs);
	pthread_exit(NULL);
	}else if(margs->buffer[0] == '4'){
		char** tokens = getTokens(margs->buffer);
		int x = simulateNetclose(tokens);

		
			int returnCloseFile = htonl(x);
			write(margs->clientSocketFildes, &returnCloseFile, sizeof(returnCloseFile));

			if(x<0){
			if(errno == EBADF) sendError(margs);
			}
			else {

			int getFileIndex = getFileIndexFromFileDescp(atoi(tokens[1]));
			if(getFileIndex < 0) 
				setError("Could not find file index");
			else {
				printf("File Index : %d is Closed \n", getFileIndex);
				fileInfo[getFileIndex].fileDescriptor = -1;
			}
		}

	free(tokens);
	close(margs->clientSocketFildes);
	free(margs);
	pthread_create(&queueChecker, 0, &checkQueue, "");	 			
	pthread_join(queueChecker, NULL);
	pthread_exit(NULL);
	}
	else{
		setError("Could not find which function to run");
			return NULL;
	}

}

void startQueue(void* data){

	arg* margs = data;
	insert(margs);
	printf("%s %d\n", "Inserted In Queue at Position :", queue_size());

	return;

}

void* checkQueue(){

	while(queue_size() > 0){
	
	arg* margs = first_element();

	char** tokens = getTokens(margs->buffer);

	char* pathname = tokens[1];
	int flag = atoi(tokens[2]);
	int mode = atoi(tokens[3]);

	if(checkCanOpen(pathname, flag, mode) == 1){
		int x = simulateNetopen(tokens);
		int returnFileDescriptor = htonl(x);
		write(margs->clientSocketFildes, &returnFileDescriptor, sizeof(returnFileDescriptor));
		delete();
		close(margs->clientSocketFildes);
		free(margs);
	}else pthread_exit(NULL);
}

pthread_exit(NULL);
return NULL;

}

int main(int argc, char* argv[]){


	threadHolder = (pthread_t*)(malloc(sizeof(pthread_t) * 1)); //first to initalize socket
	
	pthread_create(&threadHolder[totalThreads], 0, &initalizeSocket, "");
	pthread_join(threadHolder[totalThreads], NULL);

		 		

	return 0;
}

