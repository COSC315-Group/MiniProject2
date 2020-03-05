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

    /**
     * atomically inserts a job into the buffer's data
     * @param job - The job to be inserted
     */
    public synchronized void addJob(Job job) {
        try {
            // Since the endpointer denotes the next slot to be filled, if it holds a value the buffer must be full
            while (jobs[endPointer] != null) {
                // If buffer is full, wait
                wait();
            }

            //Insert the job
            jobs[endPointer] = job;
            // Readjust the pointer circularly around the array
            endPointer = (endPointer + 1) % maxBufferSize;
            notifyAll(); // Wake up any consumers waiting
        }catch(InterruptedException e){
            Thread.currentThread().interrupt();
        }
    }

    /**
     * removes and returns the oldest job in the buffer's data
     * @return - The job removed from the array
     */
    public synchronized Job removeJob() {
        try {
            // Since the startpointer denotes the oldest slot to currently have a job, if it has no job the buffer must be empty
            while (jobs[startPointer] == null) {
                // If buffer is empty, wait
                wait();
            }

            // Save the job to a variable and set its spot in the buffer to null
            Job output = jobs[startPointer];
            jobs[startPointer] = null;
            // Circularly move the startpointer around the array
            startPointer = (startPointer + 1) % maxBufferSize;
            notifyAll(); // Wake up a potentially waiting producer
            return output;
        }catch(InterruptedException e){
            Thread.currentThread().interrupt();
            return new Job().setId(-1).setLength(0); // Default error job
        }
    }

}
