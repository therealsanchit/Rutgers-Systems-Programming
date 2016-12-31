#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>



int getSizeOfFile(FILE* file){
  fseek(file,0,SEEK_END);
  int size = ftell(file);
  rewind(file);

  return size;
}

int getProcessIdIndex(pid_t pid, pid_t* childPids, int numParts){
  int i;
    for(i=0;i<numParts;i++) { if(childPids[i] == pid) return i; }

      return -1;
}
//parent process, this will create all the child processes.
int main(int argc, char** argv){

  if (argc != 3){
    printf("%s\n", "Please Enter Correct Number of Arguments. In the Format : ");
    printf("%s\n", "./executableName <file to compress>, <number of parts>. Exiting");
    return -1;
  }

  int numParts = atoi(argv[2]);

  if(numParts <= 0) {
    printf("%s\n", "Number Of Splits should be >= 1");
    return -1;
  }
  char* fileName = argv[1];

  FILE* file = fopen(fileName, "r");
  int size = 0;
  size = getSizeOfFile(file); //should give total size of file.
  fclose(file);

  if(size < 3) {  //cannot be compressed more
    printf("%s\n", "This file cannot be compressed more. Exiting");
    return -1;
  }


    pid_t* childPids = (pid_t*)(malloc(sizeof(pid_t) * numParts));
    pid_t p;

    int i;
    int n = numParts;


    int x = floor(log10(abs(numParts))) + 1;


/* Start children. */
for (i = 0; i < n; ++i) {
  if ((childPids[i] = fork()) < 0) {
    perror("Fork has failed. Aborting");
    abort();
  } else if (childPids[i] == 0) {
    char* index = malloc(x);
    sprintf(index,"%d",i);
    char *execArgs[] = { fileName, argv[2], index , NULL };
    execvp("./lzwChildProcess", execArgs);
    exit(0);
    free(index);
  }
}


/* Wait for children to exit. */
int status;
pid_t pid;
while (n > 0) {
  pid = wait(&status);
  //free(&childPids[getProcessIdIndex(pid, childPids, numParts)]);
  --n; 
}


for(i=0;i<n;i++){
  free(&childPids[i]);
}
free(childPids);

return 0;
}
