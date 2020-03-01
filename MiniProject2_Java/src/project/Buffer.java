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

    public synchronized void addJob(Job job) {
        try {
            while (jobs[endPointer] != null) {
                // If buffer is full, wait
                wait();
            }
            jobs[endPointer] = job;
            endPointer = (endPointer + 1) % maxBufferSize;
            notifyAll();
        }catch(InterruptedException e){
            Thread.currentThread().interrupt();
        }
    }
    public synchronized Job removeJob() {
        try {
            while (jobs[startPointer] == null) {
                // If buffer is empty, wait
                wait();
            }
            Job output = jobs[startPointer];
            jobs[startPointer] = null;
            startPointer = (startPointer + 1) % maxBufferSize;
            notifyAll();
            return output;
        }catch(InterruptedException e){
            Thread.currentThread().interrupt();
            return new Job().setId(-1).setLength(0); // Default error job
        }
    }

}
