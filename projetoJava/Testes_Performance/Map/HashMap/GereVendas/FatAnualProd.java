import java.io.Serializable;
import java.util.Arrays;
import java.util.stream.IntStream;

/**
 * Classe de objetos que guardam informação relativa à faturação anual de um produto.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class FatAnualProd implements Serializable, Comparable<FatAnualProd> {
    /** Código do produto a que a que uma instância de FatAnualProd diz respeito. */
    private String codigoProduto;
    /** Array cuja posição de índice i guarda o total de unidades vendidas na filial i. */
    private int[] totalUnids;

    /** 
     * Constrói a faturação anual do produto com o código passado como parâmetro.
     * @param codigoProduto Código do produto cuja faturação anual se pretende criar.
     */
    public FatAnualProd(String codigoProduto){
        this.codigoProduto = codigoProduto;
        totalUnids = new int[Constantes.N_FILIAIS+1];
    }

    /** 
     * Constrói uma cópia da faturação anual do produto passado como parâmetro. 
     * @param fAnualProd FatAnualProd a copiar.
     */
    public FatAnualProd(FatAnualProd fAnualProd){
        codigoProduto = fAnualProd.getCodigoProduto();
        totalUnids = fAnualProd.getTotalUnids();
    }

    /**
     * Devolve o código do produto a que esta FatAnualProd diz respeito.
     * @return Código do produto desta FatAnualProd.
     */
    public String getCodigoProduto(){
        return codigoProduto;
    }

    /**
     * Devolve um array que na posição de índice <code>i</code> tem o total de unidades 
     * vendidas na filial <code>i</code>, para esta FatAnualProd.
     * @return Total de unidades vendidas em cada filial.
     */
    public int[] getTotalUnids(){
        return totalUnids.clone(); // totalUnids é um array de ints, logo podemos fazer uma shallow copy
    }

    /**
     * Adiciona um determinado número de unidades ao total de unidades
     * vendidas na filial passada como primeiro argumento.
     * @param filial Filial cujo número total de unidades vendidas se pretende atualizar.
     * @param unidadesVendidas Número de unidades a acrescentar às vendas da filial <code>filial</code>.
     */
    public void adicionaUnidades(int filial, int unidadesVendidas) {
        totalUnids[filial] += unidadesVendidas;
    }

    /**
     * Calcula e devolve o total de unidades vendidas no ano todo, 
     * para o produto ao qual esta FatAnualProd diz respeito.
     * @return Número total global de unidades vendidas, registadas nesta FatAnualProd.
     */
    public int totalUnidsGlobal(){
        return IntStream.of(totalUnids).sum();
    }

    /**
     * Indica se o produto desta FatAnualProd não foi vendido.
     * @return <code>true</code> se esta FatAnualProd não tem registo de qualquer unidade vendida.
     */
    public boolean zeroUnidsVendidas(){
        return totalUnidsGlobal() == 0;
    }
    
    /**
     * Compara o código de produto desta FatAnualProd com o código de produto registado na FatAnualProd passada como parâmetro.
     * @return Valor de retorno conforme o método <code>compareTo</code> da classe String.
     */
    @Override
    public int compareTo(FatAnualProd fAnualProd){
        return codigoProduto.compareTo(fAnualProd.getCodigoProduto());
    }

    /**
     * Cria e devolve uma cópia desta faturação anual do produto.
     * @return Cópia desta FatAnualProd.
     */
    @Override
    public FatAnualProd clone(){
        return new FatAnualProd(this);
    }

    /**
     * Testa se esta FatAnualProd é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;

        FatAnualProd fAnualProd = (FatAnualProd) o;
        return codigoProduto.equals(fAnualProd.getCodigoProduto()) &&
               Arrays.equals(totalUnids, fAnualProd.getTotalUnids());
    }

    /**
     * Gera e devolve uma representação textual desta faturação anual do produto.
     * @return Representação textual desta FatAnualProd.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("Faturação anual do produto '" + codigoProduto + separador);
        sb.append("Vendas: " + separador);
        for(int i = 1; i <= Constantes.N_FILIAIS; ++i)
            sb.append("Filial ").append(i + ": ").append(totalUnids[i] + separador);

        return sb.toString();
    }

    /**
     * Calcula e devolve o <i>hash code</i> desta faturação anual do produto.
     * @return Valor do <i>hash code</i> desta FatAnualProd.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{codigoProduto, totalUnids});
    }
}