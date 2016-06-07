
/**
 * Write a description of class TriploIntIntDouble here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class TriploIntIntDouble implements java.io.Serializable
{
    private int i1, i2;
    double d;
    
    public TriploIntIntDouble(int i1, int i2, double d){
        this.i1 = i1;
        this.i2 = i2;
        this.d = d;
    }
    
    public int getInt1(){
        return i1;
    }

    public int getInt2(){
        return i2;
    }
    
    public double getDouble(){
        return d;
    }
}
