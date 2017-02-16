#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<sys/time.h>
#include"mymalloc.h"

int main() {

        long int avgA = 0;
        int a;
        for(a = 1 ; a <= 100 ; a++) {				//each workload is done 100 times
								
                struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;				//the time when the workload starts is saved

                char* ptrs[3000];				//this is used as a list of pointers returned by mymalloc

                int i;
                int j = 0;
                for(i = 0 ; i < 3000 ; i++) {

                        ptrs[i] = malloc(1);			//the pointer is obtained
                        if(errno == -1) {			//if the pointer cannot be obtained because not enough memory is available
								//then all the pointers so far are freed
                                i--;				
                                for(j = j ; j <= i ; j++) {

                                        free(ptrs[j]);

                                }

                        }

                }

                for(j = j ; j < 3000 ; j++) {			//the pointers are freed

                        free(ptrs[j]);

                }

		freeAll();					//the memory is set up to be reused

                suseconds_t end;
                gettimeofday(&tv , NULL);
                end = tv.tv_usec;				//the end time is saved

                avgA = (end - start) + avgA;			//this finds the total run time and saved to a total

        }

        avgA = avgA / 100;					//this finds the average of each run of the workload

        printf("Average time of workload A : %ld microseconds\n" , avgA);

        long int avgB = 0;					//like all other workloads this one finds the average the same way as workload A
        int b;
        for(b = 1 ; b <= 100 ; b++) {

                struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;

                char * front = (char*)malloc(1);		//this mallocs a 1 byte pointer

                int i;						//it is then freed 3000 times
                for(i = 0 ; i < 3000 ; i++) {

                        free(front);

                }

                freeAll();					//the memory is reset

                suseconds_t end;
                gettimeofday(&tv , NULL);
                end = tv.tv_usec;

                avgB = (end - start) + avgB;

        }

	avgB = avgB / 100;

        printf("Average time of workload B : %ld microseconds\n" , avgB);

        long int avgC = 0;
        int c;
        for(c = 1 ; c <= 100 ; c++) {

                struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;

                char* ptrs[3000];
                int indexM = 0;
                char* front = (char*)malloc(1);
                ptrs[indexM] = front;
                indexM++;
                int indexF = 0;

                while(indexF < 3000) {			//the loop is run until free is run 3000 times

                        int random = rand() % 2;	//if random is odd then malloc happens if even then free happens	

                        if(random == 0) {	

                                if(indexM == 3000 && indexF < 3000) {	//if malloc already happened 3000 times then a pointer is freed

                                        free(ptrs[indexF]);
                                        indexF++;
					continue;

                                }

                                ptrs[indexM] = malloc(1);
				if(errno == -1) {		//if malloc failed then a pointer is freed to make space

					free(ptrs[indexF]);
					indexF++;
					continue;

				}
                                indexM++;

                        } else {

                                if(indexM <= indexF) {			//if there are no pointers to free the a pointer is malloced

					ptrs[indexM] = malloc(1);
                                        indexM++;
					continue;

                                }

                                free(ptrs[indexF]);
                                indexF++;

                        }

                }

                freeAll();

                suseconds_t end;
                gettimeofday(&tv , NULL);

                end = tv.tv_usec;

                avgC = (end - start) + avgC;

        }

        avgC = avgC / 100;

        printf("Average time of workload C : %ld microseconds\n" , avgC);

        long int avgD = 0;
        int d;
        for(d = 1 ; d <= 100 ; d++) { //this works almost just like workload C with a few exceptions listed below and the size malloced is random

		struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;

                char* ptrs[3000];
                int indexM = 0;
		int firstRand = rand() % 5000;			//the random number is from 0 to 4999
                char* front = (char*)malloc(firstRand);
		if(errno == -1) {

			errno = -1;
			while(errno == -1) {

				firstRand = rand() % 5000;
				front = (char*)malloc(firstRand);

			}

		}
                ptrs[indexM] = front;
                indexM++;
                int indexF = 0;

                while(indexF < 3000) {

                        int random = rand() % 2;
                        int ranSize = rand() % 5000;

			 if(random == 0) {

                                if(indexM == 3000 && indexF < 30) {

                                        free(ptrs[indexF]);
                                        indexF++;
					continue;

                                }

                                ptrs[indexM] = malloc(ranSize);
				if(errno == -1) {		//if malloc fails then all the pointers are freed to make space
					
					int k;
					for(k = indexF ; k < indexM ; k++) {

						free(ptrs[k]);
						indexF++;

					}
					continue;

				}
                                indexM++;

                        } else {

                                if(indexM <= indexF) {	//if there are no pointers are not freed then the loop just continues

					continue;

                                }

                                free(ptrs[indexF]);
                                indexF++;

                        }

                }

                freeAll();

		suseconds_t end;
                gettimeofday(&tv , NULL);
                end = tv.tv_usec;

                avgD = (end - start) + avgD;

        }

	avgD = avgD / 100;

        printf("Average time of workload D : %ld microseconds\n" , avgD);

        long int avgE = 0;				//workload E mallocs 100 two byte pointers and frees all the odd pointers
        int e;						//then it frees all the even pointers
        for(e = 1 ; e <= 100 ; e++) {			//after it mallocs 100 more two byte pointers and frees all the even pointers
							//then it frees all the odd pointers
		struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;

		char* ptrs[3000];
		
		int i;
		for(i = 0 ; i < 100 ; i++) {

			ptrs[i] = malloc(2);

		}
		
		int odds;
		for(odds = 1 ; odds < 100 ; odds = odds + 2) {

			free(ptrs[odds]);

		}

		int evens;
		for(evens = 0 ; evens < 100 ; evens = evens + 2) {

			free(ptrs[evens]);

		}

		for(i = 0 ; i < 100 ; i++) {

			ptrs[i] = malloc(2);

		}

		for(evens = 0 ; evens < 100 ; evens = evens + 2) {

			free(ptrs[evens]);

		}

		for(odds = 1 ; odds < 100 ; odds = odds + 2) {

			free(ptrs[odds]);

		}

		freeAll();

		suseconds_t end;
                gettimeofday(&tv , NULL);
                end = tv.tv_usec;

                avgE = (end - start) + avgE;


        }

	avgE = avgE / 100;

	printf("Average time of workload E : %ld microseconds\n" , avgE);

        long int avgF = 0;
        int f;
        for(f = 1 ; f <= 100 ; f++) {		//workload F mallocs a one byte pointer and randonly chooses between freeing the malloced pointer
						//and a pointer that was not malloced
		struct timeval tv;
                suseconds_t start;
                gettimeofday(&tv , NULL);
                start = tv.tv_usec;

		char * front = malloc(1);
		char * decoy;

		int i;
		for(i = 0 ; i < 3000 ; i++) {

			int random = rand() % 2;

			if(random == 0) {

				free(front);
			
			} else {


				free(decoy);
			}

		}

		freeAll();

		suseconds_t end;
                gettimeofday(&tv , NULL);
                end = tv.tv_usec;

                avgF = (end - start) + avgF;

        }

	avgF = avgF / 100;

        printf("Average time of workload F : %ld microseconds\n" , avgF);

        return 1;

}





























