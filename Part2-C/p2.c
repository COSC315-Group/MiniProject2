#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
use the pthread flag with gcc to compile this code
~$ gcc -pthread threadsAndSynchronization-C.c -o program
*/

sem_t mutex;
sem_t full;
sem_t empty;

struct Job{
    int id;
    int length;
};

struct threadArgs{
    int threadId;
    struct Buffer* buffer;
};

struct Buffer{
    int endPointer;
    int startPointer;
    int length;
    struct Job buffer[];
};

void *Producer(struct Buffer *buffer){
    int nextId = 0;
    int randomLength, randomTime; // Add random job length Add random sleep job time;;
    time_t now; // Add time variable
    
    while(1){
	randomLength = rand() % 10; // Add random job length;
	randomTime = rand() % 10; // Add random sleep job time;
        sem_wait(&empty);
        struct Job nextJob;
        nextJob.id = nextId++;
        nextJob.length = randomLength; //CHANGE -i made
	sem_wait(&mutex);

	now = time(0); // Add current time 
	printf("\nProducer: Created job ID %d of length %d seconds at time: %ld", nextJob.id, nextJob.length, now);

        buffer->buffer[buffer->endPointer] = nextJob; /*request ID increases sequentially and have a random request length*/
	buffer->endPointer = (buffer->endPointer + 1)%buffer->length;

	sleep(randomTime); // Add sleep for random time
	//now = time(0); 
	//printf("\nProducer: Finish at time: %ld", now);

        sem_post(&full);
        sem_post(&mutex);

	// printf("\nProducer: sleeping for %d seconds.",1); //CHANGE
        // sleep(1); /*Sleep for a random short duration*/
    }
    pthread_exit(0); // Added

}


void *Consumer(struct threadArgs *thread){
    struct Job nullJob;
    nullJob.id = -1;
    time_t now; // Add time variable
    while(1){
        sem_wait(&full);
        sem_wait(&mutex);
        struct Job currentJob = thread->buffer->buffer[thread->buffer->startPointer];
	thread->buffer->buffer[thread->buffer->startPointer] = nullJob;
	thread->buffer->startPointer = (thread->buffer->startPointer+1)%thread->buffer->length;
	sem_post(&mutex);

	now = time(0);// Add current time 
        printf("\nConsumer %d: assigned job ID %d of length %d seconds at time: %ld ", thread->threadId, currentJob.id, currentJob.length, now);
        sleep(currentJob.length); /*Sleep for length of request length*/
	
	now = time(0);// Add current time 
	printf("\nConsumer %d: completed job ID %d at time: %ld ",thread->threadId, currentJob.length, now);
        sem_post(&empty);
    }
    pthread_exit(0); // Added
}


int main (int argc, char *argv[]){
    int N = 5;
    int M = 4;
    pthread_t producer;
    pthread_t threads[N]; /*Number of Consumers/Slaves threads*/
    
    int requestLengthMax = M; /*Max request length*/
    struct Job requestQ[N]; /*Request queue of buffer size N*/
    struct Buffer* buffer = malloc(sizeof(struct Buffer) + N*sizeof(struct Job));
    buffer->startPointer = 0;
    buffer->endPointer = 0;
    buffer->length = N;

    sem_init(&mutex, 0, 1); // I made change here
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);


    int rc;
    long t;
    void* status;
    for(t=0; t<N; t++){ /*Create Consumer/Slave threads*/
        printf("In main: creating thread %ld\n", t);
    
	struct threadArgs thread;
	thread.threadId = t;
	thread.buffer = buffer;
        rc = pthread_create(&threads[t], NULL, (void *)*Consumer, (void *)t);
        if(rc){
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        
    }
    int p = pthread_create(&producer, NULL, (void *)*Producer, (void *)1);
    

   
    pthread_join(threads[t], status);
}
