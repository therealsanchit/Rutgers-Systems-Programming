#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mymalloc.h"

char* tokens[3000];
char* workload4[3000];

void workLoad1(){
int i;
int success = 0;
	for (i = 0; i < 3000; ++i)
	{
		tokens[i] = malloc(1);
		if(mallocError == 1)break;
		success++;

	}
	 for (i = 0; i < success; ++i)
	{
		free(tokens[i]);
		if(freeError==1) break;
	}

	printf("%s\n", "Perofrming Merge Action");
	performMerge();
	if(dataTaken < 0) dataTaken=0;
	printf("Data Used : %d\n", dataTaken); //0
	printf("%s\n", "WORKLOAD 1 finished");
	return;
}

void workLoad2(){

	int* x = malloc(1);
int i;
for (i = 0; i < 3000; ++i)
{
	free(x);
	if(freeError==1) break;
}
	if(dataTaken < 0) dataTaken=0;

printf("Data Used : %d\n", dataTaken); //0
printf("%s\n", "WORKLOAD 2 finished");
return;
}


void workLoad3(){

char* workload3[3000];

int totalMallocs = 0;
int operations = 0;
int workMallocs = -1;
int workFrees =0;


while(operations < 6000){


	if(mallocError == 1 && freeError == 1)
		performMerge();

	int x = rand();
		if(x%2 && totalMallocs<3000){
		char* test = malloc(1);

		if(mallocError != 1){
		workload3[workMallocs+1] = test;
		workMallocs++;
		totalMallocs++;
		operations++;
		continue;
	}
	}else {

		if(workMallocs == -1) continue;
			free(workload3[workMallocs]);

			if(freeError==1) continue;
			workMallocs--;
			operations++;
			mallocError = 0;
	}
}



printf("%s\n", "Perofrming Merge Action");
performMerge();
printf("Operations : %d\n", operations);
if(dataTaken < 0) dataTaken = 0;

printf("Data Used : %d\n", dataTaken); //0
printf("%s\n", "WorkLoad 4 finished");

return;
}

unsigned int randr(unsigned int min, unsigned int max)
{
       double scaled = (double)rand()/RAND_MAX;

       return (max - min +1)*scaled + min;
}






void workLoad4(){

char* worload4[3000];

int totalMallocs = 0;
int operations = 0;
int workMallocs = -1;
int workFrees =0;
char* workload4[3000];


while(operations < 6000){


	if(mallocError == 1 && freeError == 1)
		performMerge();

	int x = rand();
		if(x%2 && totalMallocs<3000){
		char* test = malloc(randr(1,100));

		if(mallocError != 1){
		workload4[workMallocs+1] = test;
		workMallocs++;
		totalMallocs++;
		operations++;
		continue;
	}
	}else {

		if(workMallocs == -1) continue;
			free(workload4[workMallocs]);

			if(freeError==1) continue;
			workMallocs--;
			operations++;
			mallocError = 0;
	}
}



printf("%s\n", "Perofrming Merge Action");
performMerge();
printf("Operations : %d\n", operations);
if(dataTaken < 0) dataTaken = 0;

printf("Data Used : %d\n", dataTaken); //0
printf("%s\n", "WorkLoad 4 finished");

return;

}

void workLoad5(){

int workMallocs = 0;
int workFrees=0;
int operations=0;
int totalMallocs=0;

while(operations < 6000){


	int x = rand();
	int mallocSize = 1;
	if(x%2 && totalMallocs<3000){
		tokens[workMallocs] = malloc(mallocSize);
		workMallocs++;
		totalMallocs++;
		operations++;
		mallocSize += 10;
		continue;
	}else{
		for(workFrees=0;workFrees<workMallocs;workFrees++){
			free(tokens[workFrees]);
			if(freeError==1) break;
			operations++;
		}
		performMerge();
		workMallocs = 0;
		workFrees = 0;
	}
}

printf("%s\n", "Perofrming Merge Action");
performMerge();
printf("Operations : %d\n", operations);
if(dataTaken < 0) dataTaken = 0;

printf("Data Used : %d\n", dataTaken); //0
printf("%s\n", "WorkLoad 5 finished");

return;
}


void workLoad6(){
    int workMallocs =0;
    int workFrees=0;
    int operations =0;
    int totalMallocs = 0;

// malloc every other item in tokens array until 3000 bytes malloced
    while(dataTaken <3000){

        tokens[workMallocs] = malloc(1);
        workMallocs +=2;
        totalMallocs ++;
        operations++;
        continue;
    }
    for(int i =0; i < totalMallocs; i+=2){
        free(tokens[i]);
        if (freeError==1) break;
        }
performMerge();
workMallocs =0;
workFrees=0;

    printf("%s\n", "Performing Merge Action");
    performMerge();
    printf("Operations : %d\n", operations);
    if(dataTaken < 0) dataTaken = 0;

printf("Data Used : %d\n", dataTaken); //0
printf("%s\n", "WorkLoad 6 finished");

return;
}





int main(){

	int i;

	long workload1Time;	long workload2Time;
	long workload3Time;
	long workload4Time;
	long workload5Time;

	struct timeval t0;
	struct timeval t1;

		gettimeofday(&t0, 0);

	for (i = 0; i < 100; ++i)
	{
		workLoad1();
	}
		gettimeofday(&t1, 0);


	long elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
	elapsed = elapsed/1000;
	elapsed = elapsed/1000;
	
	workload1Time = elapsed;	



	struct timeval t2;
	struct timeval t3;

		gettimeofday(&t2, 0);

	for (i = 0; i < 100; ++i) workLoad2();

		gettimeofday(&t3, 0);


	elapsed = (t3.tv_sec-t2.tv_sec)*1000000 + t3.tv_usec-t2.tv_usec;
	elapsed = elapsed/1000;
	elapsed = elapsed/1000;
	
	workload2Time = elapsed;


	struct timeval t4;
	struct timeval t5;

		gettimeofday(&t4, 0);

	for (i = 0; i < 100; ++i) workLoad3();
	
		gettimeofday(&t5, 0);

	elapsed = (t5.tv_sec-t4.tv_sec)*1000000 + t5.tv_usec-t4.tv_usec;
	elapsed = elapsed/1000;
	elapsed = elapsed/1000;
	
	workload3Time = elapsed;







	struct timeval t6;
	struct timeval t7;

		gettimeofday(&t6, 0);


	for (i = 0; i < 100; ++i) workLoad4();

		gettimeofday(&t7, 0);


elapsed = (t7.tv_sec-t6.tv_sec)*1000000 + t7.tv_usec-t6.tv_usec;
elapsed = elapsed/1000;
elapsed = elapsed/1000;

workload4Time = elapsed;


	struct timeval t8;
	struct timeval t9;

		gettimeofday(&t8, 0);

	for (i = 0; i < 100; ++i) workLoad5();
	
			gettimeofday(&t9, 0);

elapsed = (t9.tv_sec-t8.tv_sec)*1000000 + t9.tv_usec-t8.tv_usec;
elapsed = elapsed/1000;
elapsed = elapsed/1000;

workload5Time = elapsed;




printf("Workload 1 Time : %li\n", workload1Time);
printf("Workload 2 Time : %li\n", workload2Time);
printf("Workload 3 Time : %li\n", workload3Time);
printf("Workload 4 Time : %li\n", workload4Time);
printf("Workload 5 Time : %li\n", workload5Time);



for (int i = 0; i < 100; ++i)
{
	workLoad6();
}









}






