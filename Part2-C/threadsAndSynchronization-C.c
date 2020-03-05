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

int maxLength;
int maxWait;

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

//Prints out the current time in the format HH:MM:SS
void printTime(void){
    time_t rawTime = time(NULL);
    struct tm timedat = *localtime(&rawTime);
    printf("%02d:%02d:%02d",timedat.tm_hour,timedat.tm_min,timedat.tm_sec);
}

void *Producer(struct Buffer *buffer){
    printf("\nproducer created");
    int nextId = 0;
    while(1){
        sem_wait(&empty);
        struct Job nextJob;
        nextJob.id = nextId++;
        nextJob.length = rand()%maxLength + 1;
	sem_wait(&mutex);
	
	printf("\nProducer: Created job ID %d of length %d seconds at time ",nextJob.id,nextJob.length);
	printTime();
        buffer->buffer[buffer->endPointer] = nextJob; /*request ID increases sequentially and have a random request length*/
	buffer->endPointer = (buffer->endPointer + 1)%buffer->length;
        sem_post(&full);
        sem_post(&mutex);
        
	int sleepTime = rand()%maxWait + 1;
	printf("\nProducer: sleeping for %d seconds.",sleepTime);
        sleep(sleepTime); /*Sleep for a random short duration*/
    }


}


void *Consumer(struct threadArgs *thread){
    struct Job nullJob;
    nullJob.id = -1;
    while(1){
        sem_wait(&full);
        sem_wait(&mutex);
        struct Job currentJob = thread->buffer->buffer[thread->buffer->startPointer];
	thread->buffer->buffer[thread->buffer->startPointer] = nullJob;
	thread->buffer->startPointer = (thread->buffer->startPointer+1)%thread->buffer->length;
	sem_post(&mutex);
        printf("\nConsumer %d: assigned job ID %d of length %d seconds at time ", thread->threadId, currentJob.id, currentJob.length);
	printTime();
        sleep(currentJob.length); /*Sleep for length of request length*/
	printf("\nConsumer %d: completed job ID %d at time ",thread->threadId, currentJob.id);
	printTime();
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

    int requestLengthMax = M; /*Max request length*/
    struct Buffer* buffer = malloc(sizeof(struct Buffer) + M*sizeof(struct Job));
    buffer->startPointer = 0;
    buffer->endPointer = 0;
    buffer->length = M;

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, M);

    void* status;
    int t;
    struct threadArgs cArgs[N];

    for(t=0; t<N; t++){ /*Create Consumer/Slave threads*/
        printf("\nIn main: creating thread %d",t);
        
	struct threadArgs thread;
	thread.threadId = t;
	thread.buffer = buffer;
	cArgs[t] = thread;
        pthread_create(&threads[t], NULL, (void *)*Consumer, (void *)&cArgs[t]);
    }

    pthread_create(&producer, NULL, (void *)*Producer, (void *)buffer);
    
    //Run for 30 seconds
    sleep(30);
}






