#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include"mymalloc.h"

static char ALLMEMORY[5000];	//this is the entire memory space
static short start = 0;		//this is used to note when malloc was first called so the head can be made

char* end = ALLMEMORY + 4999; 	//this points to the end of memory so we know when malloc asks for too much

Node * head = (Node *)ALLMEMORY;	//this points to the front of the memory

void* mymalloc(size_t size , char* file , int line) {	

	if(start == 0) {

		if(((char*)(head + 1) + size) > end) {		//checks if too much memory was asked for initially

			fprintf(stderr , "ERROR : Memory size too large to allocate. On line: %d , In line: %s\n" , line , file);
			errno = -1;
			return NULL;

		}

		(*head).capacity = size;			//head is initiallized and the data it points to is returned
		(*head).inUse = 1;
		(*head).data = (char*)(head + 1);
		(*head).next = NULL;
		(*head).prev = NULL;
		(*head).signature = 3323;
		start = 1;
		errno = 0;

		return (*head).data;	

	}

	Node * temp = head;					

	if((*temp).inUse == 0 && (*temp).capacity == size) {	//checks if there is a node that can be reused

		(*temp).inUse = 1;
		errno = 0;
		return (*temp).data;

	} else if((*temp).inUse == 0 && (*temp).capacity >= size && (*temp).next == NULL) {	//if a node is at the end and can accomadate
												//a pointer of a smaller size it is given to that node
		(*temp).inUse = 1;
		(*temp).capacity = size;
		errno = 0;
		return (*temp).data;

	}

	while((*temp).next != NULL) {								//this loop searches for a node that doesn't point to
												//another node
		temp = (*temp).next;
		
		if((*temp).inUse == 0 && (*temp).capacity == size) {				//again checks for a reusable node

                	(*temp).inUse = 1;
			errno = 0;
                	return (*temp).data;

        	} else if((*temp).inUse == 0 && (*temp).capacity >= size && (*temp).next == NULL) {  //again checks for a node at the end that can 
													//accomadate the new space
			(*temp).inUse = 1;
			(*temp).capacity = size;
			errno = 0;
			return (*temp).data;

		}

	} 

	if(((char*)(temp + 2) + (*temp).capacity + size) > end) {		//checks if the new node and memory can fit in the memory space

		fprintf(stderr , "ERROR : Memory size too large to allocate. On line: %d , In file: %s\n" , line , file);
		errno = -1;
		return NULL;

	}

	Node * newMem;								//a new node is made
	newMem = (Node*)((char*)(temp + 1) + (*temp).capacity);
	(*temp).next = newMem;
	(*newMem).prev = temp;
	(*newMem).capacity = size;
	(*newMem).inUse = 1;
	(*newMem).data = (char *)(newMem + 1);
	(*newMem).next = NULL;
	(*newMem).signature = 3323;

	errno = 0;				//if malloc succeeds then errno is set to 0 otherwise it is set to -1

	return (*newMem).data;
	

}

void myfree(void* ptr , char* file , int line) {

	if(ptr < (void*)head || ptr > (void*)end) {	//checks if the pointer is outside the memory space

		fprintf(stderr , "ERROR : Pointer not allocated via malloc. On line: %d , In file: %s\n" , line , file);
		errno = -1;
		return;

	}

	Node* temp = (Node*)ptr - 1;			//gets the node that points to the malloced pointer

	if(temp < head) {				//if the node is oustside the memory space the pointer was not malloced

		fprintf(stderr , "ERROR : Pointer not allocated via malloc. On line: %d , In file: %s\n" , line , file);
		errno = -1;
		return;

	}

	if((*temp).signature != 3323 ) {		//if the node does not have the signature then the pointer was not malloced

		fprintf(stderr , "ERROR : Pointer not allocated via malloc. On line: %d , In file: %s\n" , line , file);
		errno = -1;
		return;

	}

	if((*temp).inUse == 0) {			//if inUse is 0 then the pointer was already freed

		fprintf(stderr , "ERROR : Pointer was already freed. On line: %d , In file: %s\n" , line , file);
		errno = -1;	
		return;  

	}

	(*temp).inUse = 0;				//inUse is set to 0 because the pointer was freed

	Node * fuseNode = (*temp).next;			//if the next node is not in use the nodes are fused
	if(fuseNode != NULL) {				//the node left is the one currently pointed to ie temp

		if((*fuseNode).inUse == 0) {

			(*temp).capacity = (*temp).capacity + (*fuseNode).capacity + sizeof(Node);
			(*temp).next = (*fuseNode).next;
			if((*fuseNode).next != NULL) {

				(*(*fuseNode).next).prev = temp;

			}

		}

	}

	fuseNode = (*temp).prev;			//if the previous node is not in use the nodes are fused
	if(fuseNode != NULL) {				//the node left is the node previous to the one pointed to ie temp.prev

		if((*fuseNode).inUse == 0) {

			(*fuseNode).capacity = (*fuseNode).capacity + (*temp).capacity + sizeof(Node);
			(*fuseNode).next = (*temp).next;
			if((*temp).next != NULL) {

				(*(*temp).next).prev = fuseNode;

			}

		}

	}

	errno = 0;		//if free succeeds then errno is set to zero otherwise it is set to -1
	

}

void freeAll() {	//this sets start to 0 so that the memory space can be rused for another set of pointers and nodes

	start = 0;
	errno = 0;	//sets errno to 0 because it succeeds every time and any use of new memory can start off successful

}











