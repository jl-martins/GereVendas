import java.io.Serializable;

import java.util.Map;
import java.util.TreeMap;
import java.util.HashMap;
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
    /** Total de vendas registadas no mês. */
    private int totalVendas;
    /** Total faturado no mês. */
    private double totalFaturado;
    /** Mapeia códigos de produto na respetiva FatProdMes. */
    private Map<String, FatProdMes> fatProds;
    
    /** Construtores */
    
    /** 
     * Constrói uma faturação vazia para o mês passado como parâmetro.
     * @param mes Mês a que a faturação a construir diz respeito.
     */
    public FatMes(int mes) {
        this.mes = mes;
        totalVendas = 0;
        totalFaturado = 0.0;
        fatProds = new HashMap<>();
    }

    /** Constrói uma cópia da faturação do mês passada como parâmetro. */
    public FatMes(FatMes fatMes) {
        mes = fatMes.getMes();
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
        /* O fator de carga por omissao de um HashMap e 0.75, por isso se dividirmos o tamanho do map 
         * original por 0.75 e arredondarmos o resultado por excesso, evitamos a realizacao de rehashing. */
        Map<String, FatProdMes> copia = new HashMap<>((int) Math.ceil(fatProds.size() / 0.75));

        for(Map.Entry<String, FatProdMes> fProd : fatProds.entrySet())
            copia.put(fProd.getKey(), fProd.getValue().clone());
            
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
        // Obtem os campos de Venda necessarios
        mes = v.getMes();
        filial = v.getFilial();
        unidadesVendidas = v.getUnidadesVendidas();
        faturado = unidadesVendidas * v.getPrecoUnitario();
        // Atualiza os totais do mes
        ++totalVendas;
        totalFaturado += faturado;
        // Se a faturacao do produto vendido ja existir, atualiza-a, se nao, cria-a.
        FatProdMes fProdMes = fatProds.get(codigoProduto);
        if(fProdMes != null)
            fProdMes.adiciona(unidadesVendidas, faturado, filial);
        else
            fatProds.put(codigoProduto, new FatProdMes(mes, codigoProduto, unidadesVendidas, faturado, filial));
    }
    
    /**
     * Devolve um array que na posição de índice <code>i</code> tem a faturação
     * total registada nesta faturação do mês, para filial <code>i</code>.
     * @return Faturação de cada filial, no mês a que esta FatMes diz respeito.
     */
    public double[] faturacaoPorFilial(){
        double[] res = new double[Constantes.N_FILIAIS+1];
        
        for(FatProdMes fProdMes : fatProds.values()){
            double[] faturacaoProd = fProdMes.getFaturacao();
            for(int i = 1; i <= Constantes.N_FILIAIS; ++i){
                res[i] += faturacaoProd[i];
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
        return totalVendas == fatMes.getTotalVendas() &&
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
        String separadorLinhas = System.getProperty("line.separator");

        sb.append("-> Faturação do mês " + mes + separadorLinhas);
        sb.append("Total de vendas: " + totalVendas + separadorLinhas);
        sb.append("Total faturado: " + totalFaturado + separadorLinhas);
        sb.append("Faturação dos produtos: " + separadorLinhas);
        for(FatProdMes fProdMes : fatProds.values())
            sb.append(fProdMes.toString());

        return sb.toString();
    }

    /**
     * Calcula e devolve o valor do hash code desta faturação do mês.
     * @return Valor do hash code desta faturação do mês.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{mes, totalVendas, totalFaturado, fatProds});
    }
}
