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

//declaring semaphores
sem_t mutex; // Mutual exclusion - for atomically accessing buffer data
sem_t full;  // Full - for tracking how many buffer slots are full - blocks producer from over-producing
sem_t empty; // Empty - for tracking how many buffer slots are empty - blocks consumer from trying to consume nothing

// Global variables filled with user input
int maxLength; // Max length a job can have
int maxWait;   // Max time the producer will wait between producing jobs

// Struct containing info a job (request) needs to provide a consumer during consumption
struct Job{
    int id;
    int length;
};

// Struct containing the arguments for creation of a consumer
struct threadArgs{
    int threadId; // unique thread id
    struct Buffer* buffer; //pointer to the shared buffer
};

// Struct containing buffer data
struct Buffer{
    int endPointer;  // Denotes the next array index to be filled
    int startPointer;// Denotes the array index that holds the oldest job that currently exists
    int length;      // Size of the buffer array
    struct Job buffer[]; // Actual data the buffer holds for the threads
};

//Prints out the current time in the format HH:MM:SS
void printTime(void){
    time_t rawTime = time(NULL);
    struct tm timedat = *localtime(&rawTime);
    printf("%02d:%02d:%02d",timedat.tm_hour,timedat.tm_min,timedat.tm_sec);
}

/**
 * Thread function. Produces requests and adds them to the buffer to be later consumed
 * arg *buffer - pointer to the shared buffer between the threads
 */
void *Producer(struct Buffer *buffer){
    int nextId = 0; // Tracks the request ids to ensure uniqueness
    while(1){

        // Wait for an empty slot using semaphores
        sem_wait(&empty);

        // Construct a Job using nextId and a random length under the limit
        struct Job nextJob;
        nextJob.id = nextId++;
        nextJob.length = rand()%maxLength + 1;

        // Wait for the mutual exclusion lock, then print job details and add it to the buffer in the endpointer index
	    sem_wait(&mutex);
	    printf("\nProducer: Created job ID %d of length %d seconds at time ",nextJob.id,nextJob.length);
	    printTime();
        buffer->buffer[buffer->endPointer] = nextJob;
	    buffer->endPointer = (buffer->endPointer + 1)%buffer->length; // Circularly move the endpoint around the array
        // Add an extra full slot for the consumers and release the mutex
        sem_post(&full);
        sem_post(&mutex);
        
        // Sleep for a random short duration up to the max
	    int sleepTime = rand()%maxWait + 1;
	    printf("\nProducer: sleeping for %d seconds.",sleepTime);
        sleep(sleepTime);
    }


}

/**
 * Thread function. Removes requests from the buffer and consumes them, waiting for the length of the job as specified
 * arg *thread - pointer to the set of arguments (threadArgs struct) that defines this thread
 */
void *Consumer(struct threadArgs *thread){
    // Define nullJob as an 'empty' slot on the queue. This job should never be seen
    struct Job nullJob;
    nullJob.id = -1;
    while(1){

        // Wait for there to be a request in the buffer
        sem_wait(&full);

        // Wait for mutual exclusion lock, then save the job at startpointer index to memory and set that index to nullJob
        sem_wait(&mutex);
        struct Job currentJob = thread->buffer->buffer[thread->buffer->startPointer];
    	thread->buffer->buffer[thread->buffer->startPointer] = nullJob;
        // Rotate startpointer to the next index and release the mutex
	    thread->buffer->startPointer = (thread->buffer->startPointer+1)%thread->buffer->length;
    	sem_post(&mutex);

        // Print info and sleep to simulate time to complete job
        printf("\nConsumer %d: assigned job ID %d of length %d seconds at time ", thread->threadId, currentJob.id, currentJob.length);
	    printTime();
        sleep(currentJob.length); /*Sleep for length of request length*/
    	printf("\nConsumer %d: completed job ID %d at time ",thread->threadId, currentJob.id);
	    printTime();

        // Release a slot to be indicated as empty
        sem_post(&empty);
    }
}


int main (int argc, char *argv[]){
    int N;
    int M;

    // Grabbing user inputs
    printf("Input number of consumer threads: ");
    scanf("%d",&N);
    printf("\nInput size of buffer: ");
    scanf("%d",&M);
    printf("\nInput max request length: ");
    scanf("%d",&maxLength);
    printf("\nInput max producer wait time: ");
    scanf("%d",&maxWait);
    
    // Initialize thread variables
    pthread_t producer;
    pthread_t threads[N]; /*Number of Consumers/Slaves threads*/

    // Create buffer by initializing variables and allocating memory equal to these variables and the job array size
    struct Buffer* buffer = malloc(sizeof(struct Buffer) + M*sizeof(struct Job));
    buffer->startPointer = 0;
    buffer->endPointer = 0;
    buffer->length = M;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, M);

    // Initialize variables needed for consumer threads creation
    void* status;
    int t;
    struct threadArgs cArgs[N];

    for(t=0; t<N; t++){ /*Create Consumer/Slave threads*/
        printf("\nIn main: creating thread %d",t);
        
        // Create the threadArgs struct for this thread, and save it to a unique position in an array to ensure it isn't used by other threads
	    struct threadArgs thread;
	    thread.threadId = t;
	    thread.buffer = buffer;
	    cArgs[t] = thread;
        // Create consumer
        pthread_create(&threads[t], NULL, (void *)*Consumer, (void *)&cArgs[t]);
    }

    // Create producer
    pthread_create(&producer, NULL, (void *)*Producer, (void *)buffer);
    
    //Run for 30 seconds then finish program to ensure it doesn't run forever. You may remove or change this if you wish to see more output
    sleep(30);
}
