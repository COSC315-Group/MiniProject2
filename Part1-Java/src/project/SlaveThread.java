package project;

import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

public class SlaveThread extends Thread {

    private DateTimeFormatter dtf = DateTimeFormatter.ofPattern("HH:mm:ss");

    private Buffer buffer;
    private int slaveId;

    public SlaveThread(Buffer buffer, int slaveId){
        this.buffer = buffer;
        this.slaveId = slaveId;
    }

    /**
     * Runs the slave thread, taking requests off the buffer and sleeping for their lengths.
     */
    @Override
    public void run(){
        while(!Thread.interrupted()){
            // Take a job off the buffer
            Job currentJob = buffer.removeJob();
            System.out.println("Consumer "+slaveId+": assigned request ID "+currentJob.getID()+", processing request for the next "+currentJob.getLength()+" seconds, current time is "+dtf.format(LocalDateTime.now()));

            // Sleep for the amount of time specified in the job
            try {
                sleep(currentJob.getLength()*1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            System.out.println("Consumer "+slaveId+": completed request ID "+currentJob.getID()+" at time "+dtf.format(LocalDateTime.now()));
        }
    }
}
