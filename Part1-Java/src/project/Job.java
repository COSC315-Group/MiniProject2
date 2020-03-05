package project;

/**
 * Wrapper class holding the information a job needs
 */
public class Job {
    private int id;
    private int length;
    public int getID(){
        return id;
    }
    public Job setId(int id){
        this.id = id;
        return this;
    }
    public int getLength(){
        return length;
    }
    public Job setLength(int length){
        this.length = length;
        return this;
    }
}
