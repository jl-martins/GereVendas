import java.util.Arrays;

/** Classe que guarda informações relativas às compras de um produto.
 * 
 * @author LI3_Grupo1
 * @version 1.2 (06/2016)
 */

public class ComprasDoProduto implements java.io.Serializable, Comparable<ComprasDoProduto>{
    /** Código do Produto ao qual corresponde a info guardada */
    private String codigoProduto;

    /** Unidades compradas do Produto */
    private int unidadesCompradas;

    /** Total gasto nas compras do produto */
    private double faturacao;

    /* Construtores */
    /** Construtor por omissão 
     *  (declarado como privado para não ser possível construir instâncias de TriploComprasProdutosGasto sem 
     *  especificar os campos necessários).
     */
    private ComprasDoProduto(){}

    /** Construtor que inicializa a informação das Compras do produto passado como argumento a 0
     * @param codigoProduto Código do Produto para o qual queremos criar o registo da informação 
     */
    public ComprasDoProduto(String codigoProduto){
        this.codigoProduto = codigoProduto;
        unidadesCompradas = 0;
        faturacao = 0;
    }   

    /** 
     * Constrói um ComprasDoProduto com os dados passados como parâmetros.
     * @param codigoProduto Código do Produto ao qual corresponde a info guardada
     * @param unidadesCompradas Unidades compradas do Produto
     * @param faturacao Total gasto nas compras do produto
     */
    public ComprasDoProduto(String codigoProduto, int unidadesCompradas, double faturacao){
        this.codigoProduto = codigoProduto;
        this.unidadesCompradas = unidadesCompradas;
        this.faturacao = faturacao;
    }

    /**
     * Constrói uma cópia do ComprasDoProduto passado como parâmetro.
     * @param comprasDoProduto ComprasDoProduto a copiar
     */
    public ComprasDoProduto(ComprasDoProduto comprasDoProduto){
        this(comprasDoProduto.getCodigoProduto(), comprasDoProduto.getUnidadesCompradas(), comprasDoProduto.getFaturacao());
    }

    /**
     * Devolve o código do produto cuja informação é guardada.
     * @return Código do Produto.
     */
    public String getCodigoProduto(){
        return codigoProduto;
    }

    /**
     * Devolve o número de unidades compradas do produto.
     * @return Quantas unidades do produto foram compradas.
     */
    public int getUnidadesCompradas(){
        return unidadesCompradas;
    }

    /**
     * Devolve o total gasto nas compras deste produto.
     * @return Total gasto nas compras deste produto.
     */
    public double getFaturacao(){
        return faturacao;
    }

    /**
     * Regista uma venda feita do produto, alterando os campos necessário.
     * Esta função é a única que permite alterar os dados guardados num objeto ComprasDoProduto.
     */
    public void registaVenda(Venda v){
        int unidadesVendidas = v.getUnidadesVendidas();
        unidadesCompradas += unidadesVendidas;
        faturacao += v.getPrecoUnitario() * unidadesVendidas;
    }

    /* Outros métodos */
    /**
     * Devolve uma cópia deste ComprasDoProduto.
     * @return Cópia da instância que invocou o método.
     */
    @Override
    public ComprasDoProduto clone(){
        return new ComprasDoProduto(this);
    }

    /**
     * Calcula o hash code deste ComprasDoProduto.
     * @return Valor do hash code deste ComprasDoProduto.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{codigoProduto, unidadesCompradas, faturacao});
    }

    /**
     * Testa se este ComprasDoProduto é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return true;
        ComprasDoProduto c = (ComprasDoProduto) o;
        String thisCodigoProduto = this.getCodigoProduto();
        String cCodigoProduto = c.getCodigoProduto();
        return  (codigoProduto != null? codigoProduto.equals(cCodigoProduto) : cCodigoProduto == null) &&
        this.getUnidadesCompradas() == c.getUnidadesCompradas() &&
        this.getFaturacao() == c.getFaturacao();
    }

    /**
     * Devolve uma representação textual do ComprasDoProduto sobre o qual o método foi invocado.
     * @return Representação textual do ComprasDoProduto sobre o qual o método foi invocado.
     */
    @Override
    public String toString(){
        String ls = System.getProperty("line.separator");
        StringBuilder sb = new StringBuilder();
        sb.append("Código do Produto: ");
        sb.append(codigoProduto + ls);
        sb.append("Unidades compradas do produto: ");
        sb.append(unidadesCompradas + ls);
        sb.append("Dinheiro gasto no produto: ");
        sb.append(faturacao + ls);
        return sb.toString();
    }

    /** Estabelece a ordem natural dos objetos ComprasDoProduto, sendo esta igual à ordem natural dos códigos de produtos(Strings) */
    public int compareTo(ComprasDoProduto cpd){
        if(this.codigoProduto == null)
            return cpd.getCodigoProduto() == null ? 0 : -1;
        return cpd.codigoProduto.compareTo(cpd.getCodigoProduto());
    }
}
