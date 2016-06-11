import java.util.Arrays;

/**
 * Classe que representa pares compostos por um código de produto e pelo número de clientes distintos que o compraram.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class ParProdNumClis {
    /** Código de produto */
    private String prod;
    /** Número de clientes distintos que compraram o produto */
    private int numClis;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias de ParProdNumClis sem 
     *  especificar o código de cliente e o número de produtos diferentes comprados pelo mesmo)
     */
    private ParProdNumClis(){
        prod = "";
        numClis = 0;
    }
    
    /**
     * Constrói um ParProdNumClis com os dados passados como parâmetros.
     * @param prod Código de produto.
     * @param numClis Número de clientes distintos que compraram o produto.
     */
    public ParProdNumClis(String prod, int numClis){
        this.prod = prod;
        this.numClis = numClis;
    }
    
    /**
     * Constrói uma cópia do ParProdNumClis passado como parâmetro.
     * @param original ParProdNumClis a copiar.
     */
    public ParProdNumClis(ParProdNumClis original){
        prod = original.getProd();
        numClis = original.getNumClis();
    }
    
    /**
     * Devolve o código de produto deste ParProdNumClis.
     * @return Código de produto.
     */
    public String getProd(){
        return prod;
    }
    
    /**
     * Devolve o número de clientes distintos que compraram o produto deste ParProdNumClis.
     * @return Número de clientes distintos registado neste ParProdNumClis.
     */
    public int getNumClis(){
        return numClis;
    }
    
    /**
     * Adiciona um determinado valor ao número de clientes deste ParProdNumClis.
     * @param inc Valor a adicionar.
     */
    public void adicionaNumClis(int inc){
        numClis += inc;
    }
    
    /** Define o produto deste ParProdNumClis. */
    public void setProd(String prod){
        this.prod = prod;
    }
    
    /** Define o número de clientes deste ParProdNumClis. */
    public void setNumClis(int numClis){
        this.numClis = numClis;
    }
    
    /**
     * Cria e devolve uma cópia deste ParProdNumClis.
     * @return Cópia deste ParProdNumClis.
     */
    @Override
    public ParProdNumClis clone(){
        return new ParProdNumClis(this);
    }
    
    /**
     * Testa se este ParProdNumClis é igual ao objeto passado como parâmetro para o método.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        ParProdNumClis par = (ParProdNumClis) o;
        return prod.equals(par.getProd()) && numClis == par.getNumClis();
    }
    
    /**
     * Gera e devolve uma representação textual deste ParProdNumClis.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("Produto: " + (prod == null ? "n/a" : prod) + separador);
        sb.append("Nº de clientes: " + numClis + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do <i>hash code</i> deste ParProdNumClis.
     * @return Valor do <i>hash code</i> deste ParProdNumClis.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{prod, numClis});
    }
}