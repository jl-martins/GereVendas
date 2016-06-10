import java.util.Arrays;

/**
 * Classe que representa pares com um código de produto e a quantidade comprada desse produto.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class ParProdQtd {
    /** Código de produto */
    private String prod;
    /** Quantidade comprada */
    private int qtd;
    
    /** Construtores */
    
    /** 
     * Construtor por omissão 
     * (declarado como privado para não ser possível construir instâncias de
     *  ParProdQtd sem especificar o código de produto e a quantidade comprada)
     */
    private ParProdQtd(){
        prod = "";
        qtd = 0;
    }
    
    /** 
     * Constrói um ParProdQtd com os dados passados como parâmetros.
     * @param prod Código de produto.
     * @param qtd Quantidade comprada.
     */
    public ParProdQtd(String prod, int qtd){
        this.prod = prod;
        this.qtd = qtd;
    }
    
    /**
     * Constrói uma cópia do ParProdQtd passado como parâmetro.
     * @param original ParProdQtd a copiar.
     */
    public ParProdQtd(ParProdQtd original){
        prod = original.getProd();
        qtd = original.getQtd();
    }
    
    /** Getters */
    
    /**
     * Devolve o código de produto deste ParProdQtd.
     * @return Código de produto.
     */
    public String getProd(){
        return prod;
    }
    
    /**
     * Devolve a quantidade registada neste ParProdQtd.
     * @return Quantidade comprada.
     */
    public int getQtd(){
        return qtd;
    }
    
    /** Setters */
    
    /** Define o código de produto deste ParProdQtd. */
    public void setProd(String prod){
        this.prod = prod;
    }
    
    /** Define a quantidade deste ParProdQtd. */
    public void setQtd(int qtd){
        this.qtd = qtd;
    }
    
    /** Restantes métodos */
    
    /** Adiciona o valor passado como parâmetro à quantidade registada neste ParProdQtd. */
    public void adicionaQtd(int inc){
        qtd += inc;
    }
    
    /**
     * Cria e devolve uma cópia deste ParProdQtd.
     * @return Cópia deste ParProdQtd.
     */
    @Override
    public ParProdQtd clone(){
        return new ParProdQtd(this);
    }
    
    /**
     * Testa se este ParProdQtd é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;

            ParProdQtd par = (ParProdQtd) o;
        return prod.equals(par.getProd()) && qtd == par.getQtd();
    }
    
    /**
     * Gera e devolve uma representação textual deste ParProdQtd.
     * @return Representação textual da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Código de produto: " + (prod != null ? prod : "n/a") + separador);
        sb.append("Quantidade comprada: " + qtd + separador);
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o valor do hash code deste ParProdQtd.
     * @return Valor do hash code deste ParProdQtd.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{prod, qtd});
    }
}