package project;

public class SlaveThread extends Thread {

    private Buffer buffer;

    public SlaveThread(Buffer buffer){
        this.buffer = buffer;
    }

    @Override
    public void run(){

    }
}
