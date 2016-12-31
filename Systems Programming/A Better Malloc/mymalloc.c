#include "mymalloc.h"


static char myBlock[5000];
Node* head;
int numMallocs=0;
int dataTaken=0;
int dataTakenW=0;
short firstMalloc = 0;
short mallocError = 0;
short freeError = 0;

void* my_malloc(int size,char* file, int line){

			mallocError = 0;
			printf("%s\n", "::::::Entering Malloc::::::");
			printf("\n");
			printf("Number of Mallocs : %d\n", numMallocs);
	    	int dataWanted = size; 		//size of NODE is 24;
			int freeSize = 0;

				if(dataWanted <= 0){
					fprintf(stderr,"Negative allocation. Please Enter a valid size to Malloc.\n");
					mallocError = 1;
					return NULL;
				}
				if(dataWanted + sizeof(Node) >= 5000)//cant be doing this shit
				{
					fprintf(stderr, "Storage Error. Size too high. \n");
					mallocError = 1;
					return NULL;
				}
				if((dataWanted + dataTaken) >= 5000)  //memory can never be over
				{
					fprintf(stderr, "Storage Error. This will make the System run out of Memory.\n");
					mallocError = 1;
					return NULL;
				}
				
				if(firstMalloc== 0){//first time running malloc, instantiate
					head = (Node*)myBlock;
					head->inUse = 1;
					head->size = dataWanted;
					head->data = &myBlock[(sizeof(Node))];	
					dataTaken = sizeof(Node)+dataWanted;
					numMallocs++;
					firstMalloc = 1;
					printf("%s, --File %s Line %d\n", "Succesfully malloced",file,line);
					printf("Data Taken: %d\n", dataTaken);					printf("\n");

					mallocError = 0;
					return (void*)head->data;
				}
			

				Node* temp = head;
				Node* prev;


				while(temp != 0){				//situation : mallocing(10) -> mallocing (20) -> mallocing(30) -> freeing (20) -> mallocing(20); should be malloced at second node.
					if(temp->inUse == 0 && temp->size == dataWanted){
						temp->inUse = 1;
						numMallocs++;
						mallocError = 0;
						return (void*)(temp->data);
					}					
					if(temp->inUse == 0 && temp->size > (sizeof(Node) + 1 + dataWanted)){
							printf("%s\n", "Create Node");	
							int tempData = dataTakenUntil(temp) + sizeof(Node) + dataWanted;
							Node* new = (Node*)(&myBlock[tempData]);
							new->next = temp->next;
							temp->data = &myBlock[dataTakenUntil(temp) + sizeof(Node)];
							temp->next = new;
							temp->inUse = 1;
							freeSize = (temp->size - dataWanted - sizeof(Node)); 
							temp->size = dataWanted;
							new->size = freeSize;
							new->data = &myBlock[tempData + sizeof(Node)];
							new->inUse = 0;
							printf("%s, --File %s Line %d\n", "Succesfully malloced",file,line);
							printf("Data Taken: %d\n", dataTaken);
							numMallocs++;
							printf("\n");	
							mallocError = 0;
							return (void*)(temp->data);		
					}			

					if(temp->next == NULL)break;					
					temp = temp->next;
				}

				dataTaken = dataTakenUptil(temp);
				Node* newyetagain = (Node*)(&myBlock[dataTaken]);//always go to end of list for a new->malloc.
				linkNode(temp,newyetagain,dataWanted);
				printf("%s, --File %s Line %d\n", "Succesfully malloced",file,line);
				printf("Data Taken: %d\n", dataTaken);
				printf("\n");
				mallocError = 0;
				return (void*)newyetagain->data;
}

void my_free(void* variableName, char* file, int line){

			

					printf("%s\n", "-----------------Entering Free-----------------");
					printf("\n");
					Node* temp = head;
					Node* prev;
					freeError = 0;

					if(head == NULL){
						fprintf(stderr, "%s\n", "Error, No Malloced Elements. Free Fail");
												printf("%s FILE -- %d LINE \n", file, line);

						freeError = 1;
						return;
					}

					while((temp->data) != &variableName[0]){
						if(temp == NULL) break;
							prev = temp;
							temp = temp->next;
						}

					if(temp == NULL){
						fprintf(stderr, "%s\n", "Error, Could Not Find Allocation. Free Fail");
						printf("%s FILE -- %d LINE \n", file, line);
						freeError = 1;
						return;
					}


					if((temp->next == NULL) && (temp == head)){
						dataTaken = 0;
						firstMalloc = 0;
						head = NULL;
						printf("%s\n", "All of 5000 bytes are now available");
												printf("%s FILE -- %d LINE \n", file, line);

						printf("\n");
						freeError = 0;
						return;
					}


					if(temp->next == NULL){
						dataTaken -= (temp->size + sizeof(Node));
						prev->next = NULL;
						printf("This was the last Node\n");
						printf("Data Taken after Free : %d\n",dataTaken);
						lastNode = 1;
						printf("%s FILE -- %d LINE \n", file, line);
						printf("\n");
						freeError = 0;
						return;
					}

					dataTaken -= temp->size;
					temp->inUse = 0;


					printf("Data Taken after Free : %d\n",dataTaken);
											printf("%s FILE -- %d LINE \n", file, line);

					freeError = 0;
					return;
}

	void linkNode(Node* head, Node* tail, int dataWanted){//fuck it
				head->next = tail; 
				head->inUse = 1;
				tail->size = dataWanted;
				tail->inUse = 1;
				tail->data = &myBlock[dataTaken + sizeof(Node)];
				dataTaken += (sizeof(Node) + dataWanted);
				numMallocs++;
				tail->next = NULL;
				return;
	}

	void merge(Node* head, Node* tail){//if two consecutive nodes are not in use
		head->size += (tail->size + sizeof(Node));
		head->next = tail->next;
		tail = NULL;
		return;
	}


	void performMerge(){
		Node* temp = head;

		if(head == NULL){
			firstMalloc = 0;
			return;
		}

		if(head->inUse == 0 && head->next == NULL){
			dataTaken = 0;
			head->size = 0;
			head->inUse = 0;
			head->data = NULL;
			head = NULL;
			firstMalloc = 0;
			return;
		}

		while(temp->next != NULL){
			if(temp->inUse == 0 && temp->next->inUse == 0){
				merge(temp,temp->next);
				performMerge();
				continue;
			}
			temp = temp->next;
		}
		return;
	}

int dataTakenUntil(Node* end){ //counts all the data spent until some node. Used to index next node. 
	
	Node* temp = head;
	int data;	
	while(temp != end){
		data += sizeof(Node) + temp->size;
		temp = temp->next;
	}	
	return data;	
}


int dataTakenUptil(Node* end){
	Node* temp = head;

	int data=0;	
	while(temp != end){
		data += (sizeof(Node) + temp->size);
		temp = temp->next;
	}	

	data += (sizeof(Node) + end->size);
	return data;	

}


Node* returnLastNode(){
	Node* temp = head;

	if(head->next == NULL) return head;
	
	while(temp->next != NULL){
		temp = temp->next;
	}
	
	return temp;


}
      

int numofNodes(){
	Node* temp = head;
	int x=1;
	while(temp->next != NULL){
		temp = temp->next;
		x++;
	}
x++;
return x;
}















