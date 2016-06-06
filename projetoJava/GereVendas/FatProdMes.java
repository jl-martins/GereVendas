import java.util.Arrays;
import java.io.Serializable;
import java.util.stream.IntStream;
import java.util.stream.DoubleStream;

public class FatProdMes implements Serializable {
    private final int mes;
    private String codigoProduto;
    private int[] unidsVendFilial; // a posicao i da o numero de unidades vendidas na filial i, para o produto e mes considerados
    private double[] faturacao; // a posicao i da o total faturado na filial i, para o produto e mes co nsiderados 
    
    /** Constroi uma FatProdMes com base no mes e codigo de produto dados. */
    public FatProdMes(int mes, String codigoProduto){
        this.mes = mes;
        this.codigoProduto = codigoProduto;
        this.unidsVendFilial = new int[Constantes.N_FILIAIS+1];
        this.faturacao = new double[Constantes.N_FILIAIS+1];
    }
    
    /** 
     * Constroi uma FatProdMes com base no mes e codigo de produto dados.
     * O número de unidades e faturação de @c filial são inicializados com
     * os valores de @c unidadesVendidas e @c faturado, respetivamente.
     */
    public FatProdMes(int mes, String codigoProduto, int unidadesVendidas, double faturado, int filial){
        this(mes, codigoProduto);
        this.unidsVendFilial[filial] = unidadesVendidas;
        this.faturacao[filial] = faturado;
    }

    /** Constroi uma copia da FatProdMes passada como parametro. */
    public FatProdMes(FatProdMes fProdMes){
        mes = fProdMes.getMes();
        codigoProduto = fProdMes.getCodigoProduto();
        unidsVendFilial = fProdMes.getUnidsVendFilial();
        faturacao = fProdMes.getFaturacao();
    }
    
    public int getMes() {
        return mes;
    }
    
    public String getCodigoProduto(){
        return codigoProduto;
    }
    
    public int[] getUnidsVendFilial(){
        return Arrays.copyOf(unidsVendFilial, Constantes.N_FILIAIS+1);
    }

    public double[] getFaturacao(){
        return Arrays.copyOf(faturacao, Constantes.N_FILIAIS+1);
    }

    /**
     * Atualiza os dados desta FatProdMes relativos à filial passada como terceiro argumento.
     * @param unidadesVendidas Número de unidades vendidas a adicionar ao total de unidades vendidas.
     * @param faturado Valor a adicionar à faturação da filial especificada no 3º argumento.
     * @param filial Filial cujos dados serão atualizados.
     */
    public void adiciona(int unidadesVendidas, double faturado, int filial){
        this.unidsVendFilial[filial] += unidadesVendidas;
        this.faturacao[filial] += faturado;
    }

    /** @return Total de unidades vendidas registadas nesta FatProdMes. */
    public int totalUnidsVendidas(){
        return IntStream.of(unidsVendFilial).sum();
    }

    /** @return Faturação total registada nesta FatProdMes. */
    public double totalFaturado(){
        return DoubleStream.of(faturacao).sum();
    }

    /** @return Cópia desta FatProdMes. */
    public FatProdMes clone(){
        return new FatProdMes(this);
    }

    /** @return true se esta FatProdMes for igual ao objeto passado como parâmetro. */
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
    
    /** @return Valor do hash code desta FatProdMes. */
    public int hashCode(){
        int hash = 7;
        long aux;

        hash = 31*hash + codigoProduto.hashCode();
        hash = 31*hash + Arrays.hashCode(unidsVendFilial);
        for(double d : faturacao){
            aux = Double.doubleToLongBits(d);
            hash = 31*hash + (int) (aux ^ (aux >>> 32));
        }
        return hash;
    }
}