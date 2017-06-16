#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

#define HALF_SEC (500000u)

// define thread parameters structure type
typedef struct{
    // start of range
    int st;
    // end of range
    int num;
    // step
    int stp;
}sum_range_t;

void * arithmeticProgression(void* params);

// define parameters of thread functions
sum_range_t t1_params, t2_params;

// allocate custom stacks for threads
char t1_stack[PTHREAD_STACK_MIN];
char t1_stack[PTHREAD_STACK_MIN];

int main()
{
    pthread_t thread1, thread2;
    pthread_attr_t t1_attr, t2_attr;

    // retval of threads
    void * result;
    int threads_count=0;

    printf("Initializing threads parameters\n");

    // first thread parameters
    t1_params.st=1;
    t1_params.num=10;
    t1_params.stp=1;
    // second thread parameters
    t2_params.st=5;
    t2_params.num=30;
    t2_params.stp=2;

    // init thread attr structure
    pthread_attr_init(&t1_attr);
    pthread_attr_init(&t2_attr);

    // set stack addr and size
    pthread_attr_setstack(&t1_attr, &t1_stack, PTHREAD_STACK_MIN);
    pthread_attr_setstack(&t1_attr, &t1_stack, PTHREAD_STACK_MIN);

    // create two threads, increment thread counter
    printf("Creating threads\n");
    if (!pthread_create(&thread1, NULL, &arithmeticProgression, &t1_params)){
        threads_count++;
    }
    if (!pthread_create(&thread2, NULL, &arithmeticProgression, &t2_params)){
        threads_count++;
    }

    // destroy threads atributes
    pthread_attr_destroy(&t1_attr);
    pthread_attr_destroy(&t2_attr);

    // check for finished threads w/o blocking the main
    while(threads_count){

        if(! pthread_tryjoin_np(thread1, &result)){
            printf("First thread result=%d\n", ((int) (intptr_t) result));
            threads_count--;
        }

        if(! pthread_tryjoin_np(thread2, &result)){
            printf("Second thread result=%d\n", ((int) (intptr_t) result));
            threads_count--;
        }

        printf("Still waiting for %d threads\n", threads_count);
        usleep(HALF_SEC);
    }

    printf("All threads are finished!\n");

    return 0;
}


void * arithmeticProgression(void* params)
{
    int i;
    long int retVal=0;
    sum_range_t* range=(sum_range_t*)params;

    for(i=range->st; i <= range->num; i+=range->stp){
        retVal+=i;
        usleep(HALF_SEC);
    }

    return (void*)retVal;
}
