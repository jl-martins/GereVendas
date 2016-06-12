import java.io.Serializable;
import java.util.Arrays;

/**
 * Classe que representa as estatisticas do último ficheiro de vendas lido.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */

public final class EstatisticasFicheiro implements Serializable{
    
    /**
     * Nome do Ficheiro lido.
     */
    private final String ficheiro;
    
    /**
     * Total de vendas inválidas.
     */
    private final int totalVendasErr;
    
    /**
     * Total de produtos.
     */ 
    private final int totalProdutos;

    /**
     * Total de podutos distintos comprados.
     */
    private final int totalDifProdsCompr;

    /**
     * Total de clientes.
     */
    private final int totalClientes;

    /**
     * Total de clientes que compraram.
     */
    private final int totalCliCompr;

    /**
     * Total de compras de valor total igual a 0.0
     */
    private final int totalComprasZero;
    
    /**
     * Facturacao total.
     */ 
    private final double faturacaoTotal;
    
    /**
	 * Cria estatísticas do ficheiro lido, com os dados passados como parâmetros.
	 * @param ficheiro Nome do ficheiro.
	 * @param totalVendasErr Número total de registos de venda errados.
	 * @param totalProdutos Número total de produtos.
	 * @param totalDifProdsCompr Total de produtos diferentes comprados.
	 * @param totalClientes Número total de clientes.
	 * @param totalCliCompr Total de clientes distintos que realizaram compras.
	 * @param totalComprasZero Total de compras de valor total igual a 0.0
	 * @param faturacaoTotal Total faturado.
	 */
    public EstatisticasFicheiro(String ficheiro, int totalVendasErr, int totalProdutos, int totalDifProdsCompr,
                                 int totalClientes, int totalCliCompr, int totalComprasZero, double faturacaoTotal)
    {
        this.ficheiro = ficheiro;
        this.totalVendasErr = totalVendasErr;
        this.totalProdutos = totalProdutos;
        this.totalDifProdsCompr = totalDifProdsCompr;
        this.totalClientes = totalClientes;
        this.totalCliCompr = totalCliCompr;
        this.totalComprasZero = totalComprasZero;
        this.faturacaoTotal = faturacaoTotal;
    }

    /**
     * Cria uma cópia das Estatisticas do ultimo ficheiro lido.
     * @param est EstatisticasFicheiro a copiar.
     */
    public EstatisticasFicheiro(EstatisticasFicheiro est){
        ficheiro = est.getFicheiro();
        totalVendasErr = est.getTotalVendasErr();
        totalProdutos = est.getTotalProdutos();
        totalDifProdsCompr = est.getTotalDifProdsCompr();
        totalClientes = est.getTotalClientes();
        totalCliCompr = est.getTotalCliCompr();
        totalComprasZero = est.getTotalComprasZero();
        faturacaoTotal = est.getFaturacaoTotal();
    }
    
    /** Getters */

    /**
     * Devolve o nome do ficheiro de vendas a que estas estatísticas dizem respeito.
     * @return Nome do ficheiro de vendas.
     */
    public String getFicheiro() { return ficheiro; }
    
    /**
     * Devolve o total de vendas inválidas no ficheiro a que estas estatísticas dizem respeito.
     * @return Total de vendas inválidas.
     */
    public int getTotalVendasErr() { return totalVendasErr; }
    
    /** 
     * Devolve o total de produtos.
     * @return Total de produtos.
     */
    public int getTotalProdutos() { return totalProdutos; }
    
    /** 
     * Devolve o total de produtos diferentes comprados.
     * @return Total de produtos diferentes comprados.
     */
    public int getTotalDifProdsCompr() { return totalDifProdsCompr; }
    
    /** 
     * Devolve o total de clientes.
     * @return Total de clientes.
     */
    public int getTotalClientes() { return totalClientes; }
    
    /**
     * Devolve o número total de clientes distintos que realizaram compras.
     * @return Total de clientes distintso que compraram.
     */
    public int getTotalCliCompr() { return totalCliCompr; }
    
    /** 
     * Devolve o total de compras de custo total 0.0
     * @return Total de compras de valor total igual a 0.0 
     */
    public int getTotalComprasZero() { return totalComprasZero; }
    
    /** 
     * Devolve a faturação total.
     * @return Faturação total.
     */
    public double getFaturacaoTotal() { return faturacaoTotal; }
    
    // (getters por subtracao)
    /** 
     * Devolve o número total de produtos que nunca foram comprados no ano.
     * @return Total de produtos nunca comprados.
     */
    public int getTotalProdsNCompr() { return totalProdutos - totalDifProdsCompr; }
    
    /**
     * Devolve o número total de clientes que não realizaram qualquer compra durante o ano.
     * @return Total de clientes que não realizaram compras.
     */
    public int getTotalCliNCompr() { return totalClientes - totalCliCompr; } 
    
    // Não disponibilizamos setters para garantir a imutabilidade das instâncias de EstatisticasFicheiro.
    
    /**
     * Cria e retorna uma cópia desta EstatisticaFicheiro.
     * @return Cópia desta EstatisticaFicheiro.
     */
    @Override
    public EstatisticasFicheiro clone(){
        return new EstatisticasFicheiro(this);
    }
    
    /**
     * Testa se esta estatistica é igual ao objecto passado como parâmetro.
     * @return <code>true</code> se os objectos comparados forem iguais; <code>false</code> se o contrário se suceder.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        if((o == null) || (this.getClass() != o.getClass()))
            return false;

        EstatisticasFicheiro est = (EstatisticasFicheiro) o;
        return ficheiro.equals(est.getFicheiro()) && totalVendasErr == est.getTotalVendasErr()
            && totalProdutos == est.getTotalProdutos() && totalDifProdsCompr == est.getTotalDifProdsCompr()
            && totalClientes == est.getTotalClientes() && totalCliCompr == est.getTotalCliCompr()
            && totalComprasZero == est.getTotalComprasZero() && faturacaoTotal == est.getFaturacaoTotal();
    }
    
    /**
     * Cria e retorna uma string com toda a informação sobre esta EstatisticaFicheiro.
     * @return String com a informação da instância que invocou o método.
     */
    @Override
    public String toString(){
        StringBuilder str = new StringBuilder();
        String separador = System.getProperty("line.separator");
        str.append("Ficheiro de vendas: " + ficheiro + separador + separador);
        
		str.append("Nº de clientes distintos que compraram: " + totalCliCompr + separador + separador);

		str.append("Total produtos: " + totalProdutos + separador);
		str.append("Total de produtos diferentes comprados: " + totalDifProdsCompr + separador + separador);

		str.append("Total vendas inválidas: " + totalVendasErr + separador);
		str.append("Total compras de valor total igual a 0.0: " + totalComprasZero + separador + separador);

		str.append("Total facturado: " + faturacaoTotal + separador);
        return str.toString();
    }
    
    /**
     * Calcula e devolve o valor do <i>hash code</i> desta EstatisticasFicheiro.
     * @return Valor do <i>hash code</i> desta EstatisticasFicheiro.
     */
    @Override
    public int hashCode(){
        return Arrays.hashCode(new Object[]{
            ficheiro, totalClientes, totalCliCompr, totalProdutos, totalDifProdsCompr, totalComprasZero, faturacaoTotal
        });
    }
}
