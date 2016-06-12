import java.io.Serializable;

import java.util.Map;
import java.util.TreeMap;
import java.util.Arrays;

/**
 * Classe de objetos que guardam informação relativa à faturação de um dado mês,
 * como a faturação de cada um dos produtos vendidos nesse mês, o total de vendas
 * e o total faturado.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public class FatMes implements Serializable {
    /** Mês desta faturação. */
    private final int mes;
    /** Número de filiais. */
    private final int nfiliais;
    /** Total de vendas registadas no mês. */
    private int totalVendas;
    /** Total faturado no mês. */
    private double totalFaturado;
    /** Mapeia códigos de produto na respetiva FatProdMes. */
    private Map<String, FatProdMes> fatProds;
    
    /** Construtores */
    
    /**
     * Constrói a faturação do mês passado como parâmetro, que guarda dados 
     * relativos ao número de filiais por omissão (i.e.: Constantes.N_FILIAIS)
     * @param mes Mês a que a faturação a construir diz respeito.
     */
    public FatMes(int mes){
        this(mes, Constantes.N_FILIAIS);
    }
    
    /** 
     * Constrói uma faturação vazia para o mês passado e número de filiais passados como parâmetro.
     * @param mes Mês a que a faturação a construir diz respeito.
     * @param nfiliais Número de filiais a considerar.
     */
    public FatMes(int mes, int nfiliais) {
        this.mes = mes;
        this.nfiliais = nfiliais;
        totalVendas = 0;
        totalFaturado = 0.0;
        fatProds = new TreeMap<>();
    }

    /** 
     * Constrói uma cópia da faturação do mês passada como parâmetro.
     * @param fatMes Faturação do mês a copiar.
     */
    public FatMes(FatMes fatMes) {
        mes = fatMes.getMes();
        nfiliais = fatMes.getNfiliais();
        totalVendas = fatMes.getTotalVendas();
        totalFaturado = fatMes.getTotalFaturado();
        fatProds = fatMes.getFatProds();
    }
    
    /** Getters */
    
    /** 
     * Devolve o mês a que esta faturação do mês diz respeito.
     * @return Mês desta faturação do mês.
     */
    public int getMes(){
        return mes;
    }
    
    /**
     * Devolve o número de filiais para as quais esta FatMes guarda informação.
     * @return Número de filiais desta faturação do mês.
     */
    public int getNfiliais(){
        return nfiliais;
    }
    
    /** 
     * Devolve o total de vendas realizadas no mês a que esta faturação do mês diz respeito.
     * @return Total de vendas registadas nesta faturação do mês.
     */
    public int getTotalVendas(){
        return totalVendas;
    }

    /**
     * Devolve a faturação total do mês a que esta faturação do mês diz respeito.
     * @return Faturação total registada nesta faturação do mês.
     */
    public double getTotalFaturado(){
        return totalFaturado;
    }
    
    /** @return Cópia do mapeamento de código de produto para a sua faturação do mês. */
    private Map<String, FatProdMes> getFatProds(){
        Map<String, FatProdMes> copia = new TreeMap<>();

        fatProds.forEach( (k,v) -> copia.put(k, v.clone()) );
        return copia;
    }

    /**
     * Devolve uma cópia da faturação do produto passado como parâmetro, no mês desta FatMes.
     * @param codigoProduto Código do produto cuja faturação do mês se pretende obter.
     * @return Se nesta faturação do mês existirem vendas do produto cujo código foi passado parâmetro,
     *         então é devolvida a faturação desse produto no mês em causa; caso contrário é devolvido null.
     */
    public FatProdMes getFatProdMes(String codigoProduto){
        FatProdMes fProdMes = fatProds.get(codigoProduto);

        return (fProdMes != null) ? fProdMes.clone() : null;
    }

    /**
     * Regista uma venda nesta faturação do mês.
     * @param v Venda a registar.
     */
    public void registaVenda(Venda v){
        double faturado;
        int mes, unidadesVendidas, filial;
        String codigoProduto = v.getCodigoProduto();
        // Obtém os campos de Venda necessarios
        mes = v.getMes();
        filial = v.getFilial();
        unidadesVendidas = v.getUnidadesVendidas();
        faturado = unidadesVendidas * v.getPrecoUnitario();
        // Atualiza os totais do mês
        ++totalVendas;
        totalFaturado += faturado;
        // Se a faturação do produto vendido já existir, atualiza-a, se não, cria-a.
        FatProdMes fProdMes = fatProds.get(codigoProduto);
        if(fProdMes != null)
            fProdMes.adiciona(unidadesVendidas, faturado, filial);
        else
            fatProds.put(codigoProduto, new FatProdMes(mes, nfiliais, codigoProduto, unidadesVendidas, faturado, filial));
    }
    
    /**
     * Devolve um array que na posição de índice <code>i</code> tem a faturação
     * total registada nesta faturação do mês, para filial <code>i</code>.
     * @return Faturação de cada filial, no mês a que esta FatMes diz respeito.
     */
    public double[] faturacaoPorFilial(){
        double[] res = new double[nfiliais+1];
        
        for(FatProdMes fProdMes : fatProds.values()){
            double[] faturacaoProd = fProdMes.getFaturacao();
            for(int filial = 1; filial <= nfiliais; ++filial){
                res[filial] += faturacaoProd[filial];
            }
        }
        return res;
    }
    
    /** 
     * Constrói e devolve uma cópia desta faturação do mês.
     * @return Cópia desta faturação do mês.
     */
    @Override
    public FatMes clone(){
        return new FatMes(this);
    }

    /**
     * Indica se esta faturação do mês é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se esta faturação do mês for igual ao objeto passado como parâmetro.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
    
        FatMes fatMes = (FatMes) o;
        return mes == fatMes.getMes() &&
               nfiliais == fatMes.getNfiliais() &&
               totalVendas == fatMes.getTotalVendas() &&
               totalFaturado == fatMes.getTotalFaturado() &&
               fatProds.equals(fatMes.getFatProds());
    }

    /**
     * Gera e devolve uma representação textual desta faturação do mês.
     * @return Representação textual desta faturação do mês.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");

        sb.append("-> Faturação do mês " + mes + separador);
        sb.append("Número de filiais nesta faturação: " + nfiliais + separador);
        sb.append("Total de vendas: " + totalVendas + separador);
        sb.append("Total faturado: " + totalFaturado + separador);
        sb.append("Faturação dos produtos: " + separador);
        for(FatProdMes fProdMes : fatProds.values())
            sb.append(fProdMes.toString());

        return sb.toString();
    }

    /**
     * Calcula e devolve o valor do <i>hash code</i> desta faturação do mês.
     * @return Valor do <i>hash code</i> desta faturação do mês.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{mes, nfiliais, totalVendas, totalFaturado, fatProds}); 
    }
}
