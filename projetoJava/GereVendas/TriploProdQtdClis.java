import java.util.Arrays;

/**
 * Classe que representa triplos com código de produto, quantidade comprada e número de clientes distintos que o compraram.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class TriploProdQtdClis {
    /** Código de produto */
    private String prod;
    /** Quantidade comprada */
    private int qtd;
    /** Número de clientes distintos que compraram o produto */
    private int clis;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias 
     *  de TriploProdQtdClis sem especificar os vários dados do triplo)
     */
    private TriploProdQtdClis(){
        prod = "";
        qtd = 0;
        clis = 0;
    }
    
    /** 
     * Constrói um TriploProdQtdClis com os dados passados como parâmetros.
     * @param prod Código de produto.
     * @param qtd Quantidade comprada.
     * @param clis Número de clientes distintos que compraram <code>prod</code>
     */
    public TriploProdQtdClis(String prod, int qtd, int clis){
        this.prod = prod;
        this.qtd = qtd;
        this.clis = clis;
    }
    
    /**
     * Constrói uma cópia do TriploProdQtdClis passado como parâmetro.
     * @param original TriploProdQtdClis a copiar.
     */
    public TriploProdQtdClis(TriploProdQtdClis original){
        prod = original.getProd();
        qtd = original.getQtd();
        clis = original.getClis();
    }
    
    /** Getters */
    
    /**
     * Devolve o código de produto deste TriploProdQtdClis.
     * @return Código de produto.
     */
    public String getProd(){
        return prod;
    }
    
    /**
     * Devolve a quantidade registada neste TriploProdQtdClis.
     * @return Quantidade comprada.
     */
    public int getQtd(){
        return qtd;
    }
    
    /**
     * Devolve o número de clientes distintos que compraram o produto registado neste TriploProdQtdClis.
     * @return Número de clientes distintos.
     */
    public int getClis(){
        return clis;
    }
    
    /** Setters */
    
    /** Define o código de produto deste TriploProdQtdClis. */
    public void setProd(String prod){
        this.prod = prod;
    }
    
    /** Define a quantidade deste TriploProdQtdClis. */
    public void setQtd(int qtd){
        this.qtd = qtd;
    }
    
    /** Define o número de clientes distintos que compraram o produto deste TriploProdQtdClis. */
    public void setClis(int clis){
        this.clis = clis;
    }
    
    /** Restantes métodos */
    
    /** Adiciona o valor passado como parâmetro à quantidade registada neste TriploProdQtdClis. */
    public void adicionaQtd(int inc){
        qtd += inc;
    }
    
    /** Adiciona o valor passado como parâmetro ao total de clientes distintos que compraram o produto registado neste TriploProdClis. */
    public void adicionaClis(int inc){
        clis += inc;
    }
    
    /**
     * Cria e devolve uma cópia deste TriploProdQtdClis.
     * @return Cópia deste TriploProdQtdClis.
     */
    @Override
    public TriploProdQtdClis clone(){
        return new TriploProdQtdClis(this);
    }
    
    /**
     * Testa se este TriploProdQtdClis é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

        TriploProdQtdClis triplo = (TriploProdQtdClis) o;
        return prod.equals(triplo.getProd()) && qtd == triplo.getQtd() && clis == triplo.getClis();
    }
    
    /**
     * Gera e devolve uma representação textual deste TriploProdQtdClis.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Código de produto: " + (prod != null ? prod : "n/a") + separador);
        sb.append("Quantidade comprada: " + qtd + separador);
        sb.append("Número de clientes distintos que compram o produto: " + clis + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do <i>hash code</i> deste TriploProdQtdClis.
     * @return Valor do <i>hash code</i> deste TriploProdQtdClis.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{prod, qtd, clis});
    }
}
