#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#define malloc(x) mymalloc(x , __FILE__ , __LINE__)
#define free(x) myfree(x , __FILE__ , __LINE__)

//static char ALLMEMORY[5000];
//static short start = 0;

struct NodeN {

	void* data;
	short inUse;
	int capacity;
	struct NodeN* prev;
	struct NodeN* next;
	short signature;

};

typedef struct NodeN Node;

//Node* head = (Node *)ALLMEMORY;

void* mymalloc(size_t size , char* file , int line);
void myfree(void* ptr , char* file , int line);

#endif //MYMALLOC_H_
