package project;

import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

public class MasterThread extends Thread {

    private DateTimeFormatter dtf = DateTimeFormatter.ofPattern("HH:mm:ss");

    private Buffer buffer;
    private int maxRequestDelay;
    private int maxJobDuration;
    private int nextJobId;

    public MasterThread(Buffer buffer, int maxRequestDelay, int maxJobDuration){
        this.buffer = buffer;
        this.maxRequestDelay = maxRequestDelay;
        this.maxJobDuration = maxJobDuration;
        nextJobId = 0;
    }

    /**
     * Runs the master thread, producing requests and adding them to the buffer
     */
    @Override
    public void run(){
        while(!Thread.interrupted()){
            //Creating job
            int length = (int) (Math.random()*maxJobDuration)+1;
            Job job = new Job().setId(nextJobId++).setLength(length);
            System.out.println("Producer: Produced request ID "+job.getID()+", length "+length+" seconds at time "+ dtf.format(LocalDateTime.now()));

            //Adding job to buffer
            buffer.addJob(job);

            //Sleeping before creating next job
            int sleepTime = (int) (Math.random()*maxRequestDelay)+1;
            System.out.println("Producer: sleeping for "+sleepTime+" seconds");
            try {
                sleep(sleepTime*1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
}
