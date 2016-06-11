import java.util.Arrays;

/** Classe que representa triplos compostos pelo total de compras, o número de produtos diferentes comprados e o total gasto durante um mês.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class TriploComprasProdutosGasto implements java.io.Serializable{
    /** Número total de compras realizadas */
    private int totalCompras;

    /** Número de produtos distintos comprados */
    private int produtosDistintos;

    /** Total gasto nas compras */
    private double totalGasto;

    /* Construtores */

    /** 
     *  Construtor por omissão 
     *  (declarado como privado para não ser possível construir instâncias
     *  de TriploComprasProdutosGasto sem especificar os campos necessários).
     */
    private TriploComprasProdutosGasto(){} 

    /** 
     * Constrói um TriploComprasProdutosGasto com os dados passados como parâmetros.
     * @param totalCompras Total de compras realizadas.
     * @param produtosDistintos Número de produtos distintos comprados.
     * @param totalGasto Total gasto nas compras.
     */
    public TriploComprasProdutosGasto(int totalCompras, int produtosDistintos, double totalGasto){
        this.totalCompras = totalCompras;
        this.produtosDistintos = produtosDistintos;
        this.totalGasto = totalGasto;
    }

    /**
     * Constrói uma cópia do TriploComprasProdutosGasto passado como parâmetro.
     * @param t TriploComprasProdutosGasto a copiar.
     */
    public TriploComprasProdutosGasto(TriploComprasProdutosGasto t){
        this(t.getTotalCompras(), t.getProdutosDistintos(), t.getTotalGasto());
    }

    /** Getters */

    /**
     * Devolve o total de compras realizadas deste TriploComprasProdutosGasto.
     * @return Total de Compras.
     */
    public int getTotalCompras(){
        return totalCompras;
    }

    /**
     * Devolve o número de produtos distintos comprados deste TriploComprasProdutosGasto.
     * @return Número de produtos distintos comprados.
     */
    public int getProdutosDistintos(){
        return produtosDistintos;
    }

    /**
     * Devolve o total gasto nas compras deste TriploComprasProdutosGasto.
     * @return Total gasto nas compras.
     */
    public double getTotalGasto(){
        return totalGasto;
    }

    /** Outros métodos */
    
    /**
     * Devolve uma cópia deste TriploComprasProdutosGasto.
     * @return Cópia da instância que invocou o método.
     */
    @Override
    public TriploComprasProdutosGasto clone(){
        return new TriploComprasProdutosGasto(this);
    }
    
    /**
     * Testa se este TriploComprasProdutosGasto é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return true;
        
        TriploComprasProdutosGasto t = (TriploComprasProdutosGasto) o;
        return totalCompras == t.getTotalCompras() &&
               produtosDistintos == t.getProdutosDistintos() &&
               totalGasto == t.getTotalGasto();
    }
    
    /**
     * Devolve uma representação textual deste TriploComprasProdutosGasto.
     * @return Representação textual deste TriploComprasProdutosGasto.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("Total de Compras: " + totalCompras + separador);
        sb.append("Produtos Distintos: " + produtosDistintos + separador);
        sb.append("Total Gasto: " + totalGasto + separador);
        return sb.toString();
    }
    
    /**
     * Devolve o <i>hash code</i> deste TriploComprasProdutosGasto.
     * @return Valor do <i>hash code</i> deste TriploComprasProdutosGasto.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{totalCompras, produtosDistintos, totalGasto});
    }
}
