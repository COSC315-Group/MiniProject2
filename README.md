# MiniProject2: Threads and Synchronization 

The goal of this project is to implement a multi-threaded request scheduler. Our design for this is a bounded buffer producer-consumer framework. Assume that the request queue is a circular buffer of size N (i.e., an array of size N). The method involves a single producer, which is the master thread, and N consumers, which are the slave threads. The master thread will sleep for a random short duration and produce a request. Each request has a sequentially increasing request ID and a randomly chosen request length (assign each new request a random length between 1 and M seconds). The master thread then inserts the request into the queue and goes back to sleep for a random short duration before it produces another request. Of course, if the request queue, which is a bounded buffer, is full, the master thread must wait before it can insert the request into the queue. Each slave thread can be idle or busy. When a slave thread is idle, it acts as a consumer waiting for a new request in the request queue. After it consumes a request from the queue, the slave thread will be busy for a duration that is equal to the request length for that request. The busy state of a slave thread is emulated by making the thread sleep for that duration. Upon completing the request, the slave thread goes back to idle thread and attempts to consume a pending request from the request queue. If the queue is empty, the slave thread must wait, like a consumer in the producer consumer problem.
  
In part 1: we implement the above problem using Java and Monitors. 

In part 2: we implement the above problem using C and semaphores.


## Design Choices
Explain the design of your program and how syncronization works. Keep it short and to the point.

figures etc

## Priortization/Planning
The following is a breakdown of the project into a list of measurable and timed checkpoints
  Product functionality:
  - Use Java and Monitors to implement producer-consumer framework (Feb, 24th)
  - Use C and semaphores to implement producer-consumer framework (Feb, 29th)
  - Manage code structures and put comments on (Mar, 1st)
  - Design document (Mar, 3rd)
  - README.md (Mar, 4th) 

## Contributions:
Benjamin: @UBCbent
* Java and C code with Logic Design
* Planning
* Documentation (Design document)
* Document Editing

Courtney: @CourtneyGosselin
* Base code in C
* Debugging/implementation of C
* Scrum master 
* Documentation (Design document, README)
* Planning

Alex: @QinAlex
* Planning
* Feedback and Testing
* Syntax research for C and pthreads
* Documentation (Design document)

## Code Structure/Folder layout
![Code structure](https://github.com/COSC315-Group/MiniProject2/blob/master/images/folderstructure.png)

## Code Outline 

### Java outline
Code in Java: First, we construct our classes into: Job, Buffer, MasterThread, SlaveThread, and the main class which we called Run.

Job class: Wrapper class for the Job object, which specifies the job (or request) ID and length.

Buffer class: Defines the shared buffer for the threads. Has private variables startPointer and endPointer to specify the next index to be filled and the currently filled index that has existed the longest, respectively. It also tracks the max length of the buffer and, of course, the buffer data, which is an array of Job objects. The remaining functions are public, synchronized, and used to add or remove things from the buffer data array. This class acts as a monitor and as such the array data and pointers to it are edited atomically.

MasterThread class:  The master thread class extends from thread class. Master thread has its own buffer (described above and shared with the slave threads), and keeps track of maximum request creation delay, maximum job duration, and next job ID. In the constructor, we initialized the next job ID to zero, and the rest are inputted upon the creation of the thread (in our case, decided by user input). We override the Thread’s, or more accurately, the Runnable interface’s run method to define this thread’s functionality. This thread waits for an empty spot on the buffer, then accesses the buffer’s add function to create and place jobs into it. Along the way it produces informative output such as the ID, length, and creation time of a job it produces and puts on the buffer. It then outputs that it will sleep for some amount of time, which is random but has a determined maximum, and does so. 

SlaveThread class: As with the master thread, the slave thread extends from the thread class. Slave threads have a buffer shared with the master thread, and a unique slave ID. The slaves act as consumers, consuming and ‘processing’ the Job objects on the buffer. In the slaves overridden run method, we wait for a Job to be placed on the buffer, then use the buffer’s remove function to get the Job and remove it from the buffer. This job is then used to create an output notifying the user of the job ID, length and the current time. Finally, the thread sleeps to simulate processing of the job and outputs that this request has been satisfied.

Run class: This is our main class and the one used to run the program. First, the user is asked to input the value for buffer size, numbers of slave threads, maximum request length and maximum request creation delay. Then we construct objects from parameters: by using buffer size to generate our buffer; by using this buffer, maximum request length and maximum request creation delay to generate our master thread; and by using numbers of slave threads to generate an array of slave thread type and define how many threads need to be created. We create these slaves in a loop and assign them unique IDs and the buffer, shared with the master. At the end we run the master thread, and then use a for-each loop to run slave thread.

### C outline
4.2 C outline
Code in C: similarly as what we implemented in Java, we first define our structure for job, thread and buffer; then we developed printTime method, Producer method and Consumer method; at the end we compile in main. 

Structure Job: has its own id and length.

Structure Buffer: has its own end pointer, start pointer, length and structure of Job array.

Structure ThreadArgs: has its own thread ID and structure of Buffer.

printTime method: by implementing the “#include <time.h>” and use the “time_t” to generate the parameter to print out the current time in the format of “HH:MM:SS”.

Producer method: we signed the producer ID to zero. Then use a while loop, while it is true: use “sem_wait” to wait for an empty slot using semaphores. Unlike the consumer, the producer will continue through
this line, because empty was initialized to the value of maximum. Thus, empty will be decremented to 0 and the producer will put a data value into the first entry of buffer. Create the job with increment job ID and random job length. After that, wait for the mutual exclusion lock, then print job details and add it to the buffer in the endpointer index. Furthermore, circularly move the endpoint around the array. Near the end, the producer calls sem_post(&full), changing the value of the full semaphore from 0 to 1 and waking the consumer. In the end,  make it sleep for a random period of time and print out that time. 

Consumer method: we signed the null job ID to -1. Then use a while loop, while it is true: use “sem_wait(&full)” to wait for there to be a request in the buffer. Because full was initialized to the value 0, the call will block the consumer and wait for another thread to call sem_post() on the semaphore, as desired. Wait for mutual exclusion lock, then save the job at startpointer index to memory and set that index to nullJob. After that, rotate the startpointer to the next index and release the mutex, then print the consumer with current time, job ID and its length. Furthermore, we make it sleep for a length of request length. In the end, print out the completed job ID with the current time.

The main: We first ask the user to input the value for buffer size, numbers of consumer threads, maximum request length and maximum request creation delay. Then we construct objects from parameters: by using buffer size to generate our buffer; by using pthread to initialize our producer; by using pthread and numbers of consumer threads to initialize an array of consumer thread; and define how many threads need to be created. After that, initialize full to zero so that consumer thread knows buffer is empty and waits for producer thread; initialize empty to buffer size so that producer knows to add that many tasks. Furthermore, we use a for loop to iterate from “int t” equal 0 to  numbers of consumer threads to create the consumer threads and sign each slave thread index by using that “int t”. In the end, generate the producer thread and make it sleep for 30 seconds.


## Getting Started/Build Instructions/Compilation/Running
Getting started in C: the program is compiled already but if you make changes and would like to see it ran do the following
1. Clone this repository to your local machine.
2. Use the pthread flag with gcc to compile this code: ~$ gcc -pthread producer-consumer.c -o producer-consumer
3. Now using the name you placed in gcc after -o type ./producer-consumer to start the program

Getting started in Java:
1. Clone the repository to your local machine
2. Use Javac inside your src/project folder as `Javac *.java` to compile the java code into class files
4. To run the program go out to te src/ folder and type `java project.Run` this will run the program which will begin prompting you

## Sample output

### Java output
![Jave code output](https://github.com/COSC315-Group/MiniProject2/blob/master/images/javaoutput.png)

### C output
![C code output](https://github.com/COSC315-Group/MiniProject2/blob/master/images/Coutput.png)
## Built with

* [C](https://www.gnu.org/software/libc/manual/pdf/libc.pdf) - Programming Language
* [Pthreads](https://computing.llnl.gov/tutorials/pthreads/)
* [Java](https://docs.oracle.com/en/java/)
* [Linux - Debian](https://www.debian.org/doc/) - OS/Terminal 
* [GCC](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/) - How we compiled our C programs
* [Vim](https://www.vim.org/docs.php) - IDE
* [Visual Studio Code](https://code.visualstudio.com/docs) - IDE used for .md files

## Discussion of Implmenetation of the Problem
The following is a brief discussion describing the implementation of the project in both Java and C, as well as comments on the amount of effort and the ease of coding the problem we had as a team programming in both Java and C.

### Implementation Experience for Java
As something we were familiar with, the Java implementation was far easier to complete than the C one. Structuring the project and making use of what the Java language and libraries had to offer was extremely straightforward and we ran into very few problems.

### Implementation Experience for C
Creating this program in C was far more difficult. The struggles we faced affected each other and the difficulty increased exponentially with each layer of unfamiliarity. Compared to coding Java in an IDE with an easy to use thread library and a synchronized keyword, moving on to coding in VIM in a low level language that none of us have used much was a nightmare. We started with a solid skeleton of partial C code and some pseudo-code, and moved on to fill it with logic from our Java implementation and fixed errors from there. The semaphores library was very helpful, but using C pointers and data types like structs made logic very hard to translate into code. In the end we made a program that we are proud of, but the fact remains that it was at least four times longer to create than the Java part.

## Contributers

* Benjamin Tisserand @UBCbent
* Courtney Gosselin @CourtneyGosselin
* Alex Qin @QinAlex
