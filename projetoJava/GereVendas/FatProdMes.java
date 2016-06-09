import java.util.Arrays;
import java.io.Serializable;
import java.util.stream.IntStream;
import java.util.stream.DoubleStream;


/**
 * Classe de objetos que guardam informação relativa à faturação de um produto num mês.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class FatProdMes implements Serializable, Comparable<FatProdMes> {
    /** Mês a que uma instância de FatProdMes diz respeito. */
    private final int mes;
    /** Código de produto da FatProdMes. */
    private String codigoProduto;
    /** array que na posição i guarda o número de unidades vendidas na filial i, para o produto e mês considerados. */
    private int[] unidsVendFilial;
    /** array que na posição i guarda a faturação total da filial i, para o produto e mês considerados. */
    private double[] faturacao;
    
    /** Construtores */
    
    /** Constrói uma FatProdMes com base no mês e código de produto dados. */
    public FatProdMes(int mes, String codigoProduto){
        this.mes = mes;
        this.codigoProduto = codigoProduto;
        this.unidsVendFilial = new int[Constantes.N_FILIAIS+1];
        this.faturacao = new double[Constantes.N_FILIAIS+1];
    }
    
    /** 
     * Constrói uma FatProdMes com base no mês e código de produto dados.
     * O número de unidades e faturação de <code>filial</code> são inicializados com
     * os valores de <code>unidadesVendidas</code> e <code>faturado</code>, respetivamente.
     */
    public FatProdMes(int mes, String codigoProduto, int unidadesVendidas, double faturado, int filial){
        this(mes, codigoProduto);
        this.unidsVendFilial[filial] = unidadesVendidas;
        this.faturacao[filial] = faturado;
    }

    /** Constrói uma cópia da FatProdMes passada como parâmetro. */
    public FatProdMes(FatProdMes fProdMes){
        mes = fProdMes.getMes();
        codigoProduto = fProdMes.getCodigoProduto();
        unidsVendFilial = fProdMes.getUnidsVendFilial();
        faturacao = fProdMes.getFaturacao();
    }
    
    /**
     * Devolve o mês a que esta FatProdMes diz respeito.
     * @return Mês desta FatProdMes.
     */
    public int getMes() {
        return mes;
    }
    
    /**
     * Devolve o código de produto a que esta FatProdMes diz respeito.
     * @return Código de produto desta FatProdMes.
     */
    public String getCodigoProduto(){
        return codigoProduto;
    }
    
    /**
     * Devolve um array que na posição de índice <code>i</code> tem o número de unidades vendidades
     * na filial <code>i</code>, para o produto e mês desta FatProdMes.
     * @return Array com o número de unidades vendidas em cada filial, para o produto e mês desta FatProdMes.
     */
    public int[] getUnidsVendFilial(){
        return Arrays.copyOf(unidsVendFilial, Constantes.N_FILIAIS+1);
    }
    
    /**
     * Devolve um array que na posição de índice <code>i</code> o valor faturado
     * na filial <code>i</code>, para o produto e mês desta FatProdMes.
     * @return Array com a faturação de cada filial, para o produto e mês desta FatProdMes.
     */
    public double[] getFaturacao(){
        return Arrays.copyOf(faturacao, Constantes.N_FILIAIS+1);
    }

    /**
     * Atualiza os dados desta FatProdMes relativos à filial passada como 3º argumento.
     * @param unidadesVendidas Número de unidades vendidas a adicionar ao total de unidades vendidas.
     * @param faturado Valor a adicionar à faturação da filial dada pelo 3º argumento.
     * @param filial Filial cujos dados serão atualizados.
     */
    public void adiciona(int unidadesVendidas, double faturado, int filial){
        this.unidsVendFilial[filial] += unidadesVendidas;
        this.faturacao[filial] += faturado;
    }

    /** 
     * Calcula e devolve o total global de unidades vendidas registadas nesta FatProdMes.
     * @return Total de unidades vendidas registadas nesta FatProdMes.
     */
    public int totalUnidsVendidas(){
        return IntStream.of(unidsVendFilial).sum();
    }

    /**
     * Calcula e devolve a faturação total registada nesta FatProdMes.
     * @return Faturação total registada nesta FatProdMes.
     */
    public double totalFaturado(){
        return DoubleStream.of(faturacao).sum();
    }
    
    /**
     * Compara o código de produto desta FatProdMes com o da FatProdMes passada como parâmetro.
     * @param fProdMes FatProdMes cujo código de produto será comparado com o da instância que invoca o método.
     * @return -1, 0 ou 1, conforme o código de produto deste FatProdMes seja menor, igual ou 
     *         maior que o do objeto passado como parâmetro para o método.
     */
    @Override
    public int compareTo(FatProdMes fProdMes){
        String outroCodigoProduto = fProdMes.getCodigoProduto();
        if(codigoProduto == null)
            return (outroCodigoProduto == null ? 0 : -1);
        else if(outroCodigoProduto == null)
            return 1;
        else
            return codigoProduto.compareTo(outroCodigoProduto);
    }        
    
    /** 
     * Cria e devolve uma cópia desta FatProdMes.
     * @return Cópia desta FatProdMes. 
     */
    public FatProdMes clone(){
        return new FatProdMes(this);
    }

    /**
     * Indica se esta FatProdMes é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se esta FatProdMes for igual ao objeto passado como parâmetro.
     */
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;

        FatProdMes fProdMes = (FatProdMes) o;
        return codigoProduto.equals(fProdMes.getCodigoProduto()) &&
               Arrays.equals(unidsVendFilial, fProdMes.getUnidsVendFilial()) &&
               Arrays.equals(faturacao, fProdMes.getFaturacao());
    }
    
    /** 
     * Calcula e devolve o hash code desta FatProdMes
     * @return Valor do hash code desta FatProdMes.
     */
    public int hashCode(){
        return Arrays.hashCode(new Object[]{mes, codigoProduto, unidsVendFilial, faturacao});
    }
}