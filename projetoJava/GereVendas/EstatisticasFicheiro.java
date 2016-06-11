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

	/**
	 * Cria estatísticas do ficheiro passado como argumentos, e inicializa todos os campos a zero.
	 */
    public EstatisticasFicheiro(String ficheiro){
        this.ficheiro = ficheiro;
        totalVendasErr = 0;
        totalProdutos = 0;
        totalDifProdsCompr = 0;
        totalClientes = 0;
        totalCliCompr = 0;
        totalComprasZero = 0;
        faturacaoTotal = 0;
    }

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
    public EstatisticasFicheiro(String ficheiro, int totalVendasErr, int totalProdutos,int totalDifProdsCompr, int totalClientes, int totalCliCompr, int totalComprasZero, double faturacaoTotal){
        this.ficheiro = ficheiro;
        this.totalVendasErr = totalVendasErr;
        this.totalProdutos = totalProdutos;
        this.totalDifProdsCompr = totalDifProdsCompr;
        this.totalClientes = totalClientes;
        this.totalCliCompr = totalCliCompr;
        this.totalComprasZero = totalComprasZero;
        this.faturacaoTotal = faturacaoTotal;
    }
    
    /** Getters */

	/** @return Nome do último ficheiro. */
    public String getFicheiro() { return ficheiro; }
	/** @return Total de vendas inválidas. */
    public int getTotalVendasErr() { return totalVendasErr; }
	/** @return Total de produtos. */
    public int getTotalProdutos() { return totalProdutos; }
	/** @return Total de produtos diferentes comprados. */
    public int getTotalDifProdsCompr() { return totalDifProdsCompr; }
	/** @return Total de clientes. */
    public int getTotalClientes() { return totalClientes; }
	/** @return Total de clientes que compraram. */
    public int getTotalCliCompr() { return totalCliCompr; }
	/** @return Total de compras de valor total igual a 0.0 */
    public int getTotalComprasZero() { return totalComprasZero; }
	/** @return Faturação total. */
    public double getFaturacaoTotal() { return faturacaoTotal; }
    
	// (getters por subtracao)
    /** @return Total de produtos nao comprados. */
	public int getTotalProdsNCompr() { return totalProdutos - totalDifProdsCompr; }
	/** @return Total de clientes que não compraram. */
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
		str.append("Ficheiro de vendas: ").append(ficheiro).append("\n\n");
		
		str.append("Total clientes: ").append(totalClientes).append("\n");
		str.append("Nº de clientes distintos que compraram: ").append(totalCliCompr).append("\n\n");

		str.append("Total produtos: ").append(totalProdutos).append("\n");
		str.append("Total de produtos diferentes comprados: ").append(totalDifProdsCompr).append("\n\n");

		str.append("Total vendas inválidas: ").append(totalVendasErr).append("\n");
		str.append("Total compras de valor total igual a 0.0: ").append(totalComprasZero).append("\n\n");

		str.append("Total facturado: ").append(faturacaoTotal).append("\n");
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
