package project;

public class Buffer {

    private Job[] jobs;
    private int maxBufferSize;
    private int startPointer; //Oldest index to have been filled
    private int endPointer;   //Next index to fill

    public Buffer(int size){
        maxBufferSize = size;
        jobs = new Job[size];
        startPointer = 0;
        endPointer = 0;
    }

    public synchronized void addJob(Job job) throws InterruptedException {
        while(jobs[endPointer]!=null){
            // If buffer is full, wait
            wait();
        }
        jobs[endPointer] = job;
        endPointer = (endPointer+1)%maxBufferSize;
        notifyAll();
    }
    public synchronized Job removeJob() throws InterruptedException {
        while(jobs[startPointer]==null){
            // If buffer is empty, wait
            wait();
        }
        Job output = jobs[startPointer];
        jobs[startPointer] = null;
        startPointer = (startPointer+1)%maxBufferSize;
        notifyAll();
        return output;
    }

}
