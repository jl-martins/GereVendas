import java.util.Arrays;

public class ParStringDouble
{
    private String s;
    private Double d;

    private ParStringDouble(){}
    
    public ParStringDouble(String s, Double d){
        this.s = s;
        this.d = d;
    }

    public ParStringDouble(ParStringDouble p){
        this.s = p.s;
        this.d = p.d;
    }

    public String fst(){
        return s;
    }

    public Double snd(){
        return d;
    }

    public boolean equals(Object o){
        if(this == o) return true;
        if((o == null) || (this.getClass() == o.getClass())) 
            return false;
        ParStringDouble p = (ParStringDouble) o;
        return ((this.s == null && p.s == null) || (this.s != null && this.s.equals(p.s))) &&
               ((this.d == null && p.d == null) || (this.d != null && this.d.equals(p.d)));
    }

    public ParStringDouble clone(){
        return new ParStringDouble(this);
    }

    public String toString(){
        StringBuilder str = new StringBuilder();
        str.append("("); str.append(s);
        str.append(", ");
        str.append(d.toString()); 
        str.append(")");
        return str.toString();
    }

    public int hashCode(){
        return Arrays.hashCode(new Object[]{s, d});
    }
}
