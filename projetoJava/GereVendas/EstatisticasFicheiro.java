import java.io.Serializable;

/**
 * <p>
 * Classe que representa as estatisticas do ultimo ficheiro de vendas lido 
 */

public class EstatisticasFicheiro implements Serializable{
    
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
	 * Total de compras a zero.
	 */
    private final int totalComprasZero;
	
	/**
	 * Facturacao total.
	 */ 
    private final double faturacaoTotal;

	/**
	 * Cria uma cópia das Estatisticas do ultimo ficheiro lido. 
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
	 * Cria Estatisticas do ficheiro passado como argumentos, e inicializa todos os campos a zero.
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
	 * Cria estatisticas do ficheiro lido.
	 */
    public EstatisticasFicheiro(String ficheiro, int totalVendasErr, int totalProdutos,int totalDifProdsCompr,int totalClientes, int totalCliCompr, int totalComprasZero, double faturacaoTotal){
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

	/** @return nome do ultimo ficheiro. */
    public String getFicheiro() { return ficheiro; }
	/** @return total de vendas inválidas. */
    public int getTotalVendasErr() { return totalVendasErr; }
	/** @return total de produtos. */
    public int getTotalProdutos() { return totalProdutos; }
	/** @return total de produtos diferentes comprados. */
    public int getTotalDifProdsCompr() { return totalDifProdsCompr; }
	/** @return total de clientes. */
    public int getTotalClientes() { return totalClientes; }
	/** @return total de clientes que compraram. */
    public int getTotalCliCompr() { return totalCliCompr; }
	/** @return total de compras a zero. */
    public int getTotalComprasZero() { return totalComprasZero; }
	/** @return faturacao total. */
    public double getFaturacaoTotal() { return faturacaoTotal; }
    
	// (getters por subtracao)
    /** @return total de produtos nao comprados. */
	public int getTotalProdsNCompr() { return totalProdutos - totalDifProdsCompr; }
	/** @return total de clientes que não compraram. */
    public int getTotalCliNCompr() { return totalClientes - totalCliCompr; } 
    
	/**
	 * Cria e retorna uma copia desta estatistica.
	 * @return cópia desta estatistica.
	 */
    public EstatisticasFicheiro clone(){
        return new EstatisticasFicheiro(this);
    }
    
	/**
	 * Testa se esta estatistica é igual ao objecto passado como parametro.
	 * @return <code>true</code> se os objectos comparados forem iguais, <code>false</code> se o contrário se suceder.
	 */
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
	 * Cria e retorna uma string com toda a informacao sobre a instancia desta classe.
	 * @return string com a informacao sobre a instancia desta classe.
	 */
    public String toString(){
        StringBuilder str = new StringBuilder();
		str.append("Ficheiro: ").append(ficheiro).append("\n\n");
		
		str.append("Total Clientes: ").append(totalClientes).append("\n");
		str.append("Clientes que compraram: ").append(totalCliCompr).append("\n\n");

		str.append("Total Produtos: ").append(totalProdutos).append("\n");
		str.append("Total de produtos diferentes comprados: ").append(totalDifProdsCompr).append("\n\n");

		str.append("Total vendas inválidas: ").append(totalVendasErr).append("\n");
		str.append("Total compras a zero: ").append(totalComprasZero).append("\n\n");

		str.append("Total facturado: ").append(faturacaoTotal).append("\n");
        return str.toString();
    }
}
