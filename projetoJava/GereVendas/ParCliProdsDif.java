import java.util.Arrays;

/**
 * Classe que representa pares compostos por um código de cliente e pelo número de produtos diferentes comprados pelo mesmo.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class ParCliProdsDif{
    /** Código de cliente */
    private String cli;
    /** Número de produtos diferentes comprados */
    private int prodsDif;
    
    /** Construtores */
    
    /** Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias de ParCliProdsDif sem 
     *  especificar o código de cliente e o número de produtos diferentes comprados pelo mesmo)
     */
    private ParCliProdsDif(){
        cli = "";
        prodsDif = 0;
    }
    
    /** 
     * Constrói um ParCliProdsDif com os dados passados como parâmetros.
     * @param cli Código de cliente
     * @param prodsDif Número de produtos distintos comprados pelo cliente.
     */
    public ParCliProdsDif(String cli, int prodsDif){
        this.cli = cli;
        this.prodsDif = prodsDif;
    }
    
    /**
     * Constrói uma cópia do ParCliProdsDif passado como parâmetro.
     * @param original ParCliProdsDif a copiar.
     */
    public ParCliProdsDif(ParCliProdsDif original){
        cli = original.getCli();
        prodsDif = original.getProdsDif();
    }
    
    /** Getters */
    
    /**
     * Devolve o código de cliente deste ParCliProdsDif.
     * @return Código de cliente.
     */
    public String getCli(){
        return cli;
    }
    
    /**
     * Devolve o número de produtos diferentes registados neste ParCliProdsDif.
     * @return Número de produtos diferentes comprados pelo cliente.
     */
    public int getProdsDif(){
        return prodsDif;
    }
    
    /** Setters */
    
    /** Define o código de cliente deste ParCliProdsDif. */
    public void setCli(String cli){
        this.cli = cli;
    }
    
    /** Define o número de produtos diferentes comprados pelo cliente deste ParCliProdsDif. */
    public void setProdsDif(int prodsDif){
        this.prodsDif = prodsDif;
    }
    
    /**
     * Constrói e devolve uma cópia deste ParCliProdsDif.
     * @return Cópia da instância que invocou o método.
     */
    @Override
    public ParCliProdsDif clone(){
        return new ParCliProdsDif(this);
    }
    
    /**
     * Testa se este ParCliProdsDif é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        ParCliProdsDif par = (ParCliProdsDif) o;
        return cli.equals(par.getCli()) && prodsDif == par.getProdsDif();
    }
    
    /**
     * Gera e devolve uma representação textual deste ParCliProdsDif.
     * @return Representação textual deste ParCliProdsDif.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        return sb.toString();
    }
    
    @Override
    /**
     * Calcula e devolve o hash code deste ParCliProdsDif.
     * @return Valor do hash code deste ParCliProdsDif.
     */
    public int hashCode(){
        return Arrays.hashCode(new Object[]{cli, prodsDif});
    }
}
