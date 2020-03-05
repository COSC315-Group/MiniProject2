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

## Code Outline 

### Java outline

### C outline


## Getting Started/Build Instructions
Getting started in C: the program is compiled already but if you make changes and would like to see it ran do the following
1. Clone this repository to your local machine.
2. Use the pthread flag with gcc to compile this code: ~$ gcc -pthread producer-consumer.c -o producer-consumer
3. Now using the name you placed in gcc after -o type ./producer-consumer to start the program

Getting started in Java:
1. Clone the repository to your local machine
2. Use Javac inside your src/project folder as `Javac *.java` to compile the java code into class files
4. To run the program go out to te src/ folder and type `java project.Run` this will run the program which will begin prompting you

## Built with

* [C](https://www.gnu.org/software/libc/manual/pdf/libc.pdf) - Programming Language
* [Pthreads](https://computing.llnl.gov/tutorials/pthreads/)
* [Java](https://docs.oracle.com/en/java/)
* [Linux - Debian](https://www.debian.org/doc/) - OS/Terminal 
* [GCC](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/) - How we compiled our C programs
* [Vim](https://www.vim.org/docs.php) - IDE
* [Visual Studio Code](https://code.visualstudio.com/docs) - IDE used for .md files

## Discussion of Implmenetation of the Problem
Add a brief discussion describing your experience implementing this problem in Java and C/C++. Comment on the amount of effort and ease of coding the problem in different languages.

### Java

### C

## Output Examples

### Java

### C

## Contributers

* Benjamin Tisserand @UBCbent
* Courtney Gosselin @CourtneyGosselin
* Alex Qin @QinAlex

## Acknowledgements 

* [Pthreads](https://computing.llnl.gov/tutorials/pthreads/)

