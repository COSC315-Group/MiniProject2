#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
use the pthread flag with gcc to compile this code
~$ gcc -pthread producer_consumer.c -o producer_consumer
*/

struct Job{
    int id;
    int length;
}

void *Producer(void, *threadid){
    int nextId = 0;
    while(1){
        sem_wait(&empty);
        Job nextJob;
        requestQ(); /*request ID increases sequentially and have a random reuqest length*/
        sem_post(&full);
        sem_post(&requestQMutex);
        sleep(1); /*Sleep for a random short duration*/
    }
    


}


void *Consumer(void, *threadid){
    while(1){
        sem_wait(full);
        sem_wait(requestQMutex);
        long tid = (long)threadid;
        printf("Hello World! It's me, thread #%ld!\n", tid)
        rc.busy;
        requestQ(); /*Decrements requestQ*/
        sleep(requestQ[i]); /*Sleep for length of requestLength*/
        sem_post(empty);
        sem_post(requestMutex);
    }
    pthread_exit(NULL);
}


int main (int argc, char *argv[]){
    pthread_t producer;
    pthread_t threads[N]; /*Number of Consumers/Slaves threads*/
    
    int requestLengthMax = M; /*Max request length*/
    int requestQ[N]; /*Request queue of buffer size N*/

    sem_t requestQMutex;
    sem_t full;
    sem_t empty;

    sem_init(requestQMutex, 1, 0);
    sem_init(full, 0, 0);
    sem_init(empty, 0, N);


    int rc;
    long t;
    for(t=0, t<N; t++){ /*Create Consumer/Slave threads*/
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], Null, Consumer, (void *)t);
        if(rc){
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1)
        }
    }
    p = pthread_create(producer, Null, Producer, void *);
}






