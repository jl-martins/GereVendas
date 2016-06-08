
public class ParCliProdsDif{
    private String cli;
    private int prodsDif;

    private ParCliProdsDif(){
        cli = "";
        prodsDif = 0;
    }

    public ParCliProdsDif(String cli, int prodsDif){
        this.cli = cli;
        this.prodsDif = prodsDif;
    }

    public ParCliProdsDif(ParCliProdsDif original){
        cli = original.getCli();
        prodsDif = original.getProdsDif();
    }

    public String getCli(){
        return cli;
    }

    public int getProdsDif(){
        return prodsDif;
    }

    public void setCli(String cli){
        this.cli = cli;
    }

    public void setProdsDif(int prodsDif){
        this.prodsDif = prodsDif;
    }

    public ParCliProdsDif clone(){
        return new ParCliProdsDif(this);
    }

    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        ParCliProdsDif par = (ParCliProdsDif) o;
        return cli.equals(par.getCli()) && prodsDif == par.getProdsDif();
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        return sb.toString();
    }

    public int hashCode(){
        int hash = 7;

        hash = 31*hash + cli.hashCode();
        hash = 31*hash + prodsDif;
        return hash;
    }
}
