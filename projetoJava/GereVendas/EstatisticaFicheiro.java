public class EstatisticaFicheiro
{
	private String ficheiro; /* Logo no inicio */
	private int totalVendasErr; /* Durante a leitura */
	private int totalProdutos; /* Antes leitura */
	private int totalDifProdsCompr; /* Depois da leitura */
/*	private int totalProdsNCompr; por sub */
	private int totalClientes; /* Antes da leitura */
	private int totalCliCompr; /* Depois da leitura */ 
/*	private int totalCliNCompr; por sub */
	private int totalComprasZero; /* Durante!? */ 
	private double facturacaoTotal; /* Durante a leitura */

	public EstatisticaFicheiro(EstatisticaFicheiro est)
	{
		ficheiro = est.getFicheiro();
		totalVendasErr = est.getTotalVendasErr();
		totalProdutos = est.getTotalProdutos();
		totalDifProdsCompr = est.getTotalDifProdsCompr();
		totalClientes = est.getTotalDifProdsCompr();
		totalCliCompr = est.getTotalCliCompr();
		totalComprasZero = est.getTotalComprasZero();
		facturacaoTotal = est.getFacturacaoTotal();
	}

	public EstatisticaFicheiro(String ficheiro)
	{
		this.ficheiro = ficheiro;
		totalVendasErr = 0;
		totalProdutos = 0;
		totalDifProdsCompr = 0;
		totalClientes = 0;
		totalCliCompr = 0;
		totalComprasZero = 0;
		facturacaoTotal = 0;
	}

	public EstatisticaFicheiro(String ficheiro, int totalVendasErr, int totalProdutos,int totalDifProdsCompr,int totalClientes, int totalCliCompr, int totalComprasZero, double facturacaoTotal)
	{
		this.ficheiro = ficheiro;
		this.totalVendasErr = totalVendasErr;
		this.totalProdutos = totalProdutos;
		this.totalDifProdsCompr = totalDifProdsCompr;
		this.totalClientes = totalClientes;
		this.totalCliCompr = totalCliCompr;
		this.totalComprasZero = totalComprasZero;
		this.facturacaoTotal = facturacaoTotal;
	}

	public String getFicheiro() { return ficheiro; }
	public int getTotalVendasErr() { return totalVendasErr; }
	public int getTotalProdutos() { return totalProdutos; }
	public int getTotalDifProdsCompr() { return totalDifProdsCompr; }
	public int getTotalClientes() { return totalClientes; }
	public int getTotalCliCompr() { return totalCliCompr; }
	public int getTotalComprasZero() { return totalComprasZero; }
	public double getFacturacaoTotal() { return facturacaoTotal; }

	/*gets por sub*/
	public int getTotalProdsNCompr() { return totalProdutos - totalDifProdsCompr; }
	public int getTotalCliNCompr() { return totalClientes - totalCliCompr; }

	private void setFicheiro(String fich) { ficheiro = fich; }
	private void setTotalVendasErr(int t) { totalVendasErr = t; }
	private void setTotalProdutos(int t) { totalProdutos = t; }
	private void setTotalDifProdsCompr(int t) { totalDifProdsCompr = t; }
	private void setTotalClientes(int t) { totalClientes = t; }
	private void setTotalCliCompr(int t) { totalCliCompr = t; }
	private void setTotalComprasZero(int t) { totalComprasZero = t; }
	private void setFacturacaoTotal(double f) { facturacaoTotal = f; }

	private void incTotalVendasErr() { totalVendasErr+=1; }
	private void incFacturacaoTotal(double aumento) { facturacaoTotal += aumento; }
	private void incTotalComprasZero() { totalComprasZero += 1; }

	public boolean equals(Object o)
	{
		if(this == o) return true;
		if((o == null) || (this.getClass() != o.getClass())) return false;

		EstatisticaFicheiro obj = (EstatisticaFicheiro) o;
		return ficheiro.equals(obj.getFicheiro()) && totalVendasErr == obj.getTotalVendasErr()
			&& totalProdutos == obj.getTotalProdutos() && totalDifProdsCompr == obj.getTotalDifProdsCompr()
			&& totalClientes == obj.getTotalClientes() && totalCliCompr == obj.getTotalCliCompr()
			&& totalComprasZero == obj.getTotalComprasZero() && facturacaoTotal == obj.getFacturacaoTotal();
	}

	public EstatisticaFicheiro clone()
	{
		return new EstatisticaFicheiro(this);
	}

	public String toString()
	{
		StringBuilder str = new StringBuilder();
		return str.toString();
	}
}
