package project;

import java.util.Scanner;

public class Run {
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);

        //Get parameters for execution
        System.out.print("Input the buffer size: ");
        int bufferSize = input.nextInt();
        System.out.print("\nInput the amount of slave threads: ");
        int slaveThreads = input.nextInt();
        System.out.print("\nInput the max request creation delay: ");
        int maxRequestDelay = input.nextInt();
        System.out.print("\nInput the max request length: ");
        int maxRequestLength = input.nextInt();

        //Construct objects from parameters
        Buffer buffer = new Buffer(bufferSize);
        MasterThread master = new MasterThread(buffer,maxRequestDelay,maxRequestLength);
        SlaveThread[] slaves = new SlaveThread[slaveThreads];
        for(int i = 0; i<slaves.length;i++){
            slaves[i] = new SlaveThread(buffer,i);
        }

        //run the threads
        master.start();
        for(SlaveThread slave : slaves){
            slave.start();
        }

        // Automatically end after 30 seconds
        try {
            Thread.sleep(30000);
            System.exit(0);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
