public class EstatisticasFicheiro{
    private final String ficheiro; /* Logo no inicio */
    private final int totalVendasErr; /* Durante a leitura */
    private final int totalProdutos; /* Antes leitura */
    private final int totalDifProdsCompr; /* Depois da leitura */
/*  private int totalProdsNCompr; por sub */
    private final int totalClientes; /* Antes da leitura */
    private final int totalCliCompr; /* Depois da leitura */ 
/*  private int totalCliNCompr; por sub */
    private final int totalComprasZero; /* Durante!? */ 
    private final double faturacaoTotal; /* Durante a leitura */

    public EstatisticasFicheiro(EstatisticasFicheiro est){
        ficheiro = est.getFicheiro();
        totalVendasErr = est.getTotalVendasErr();
        totalProdutos = est.getTotalProdutos();
        totalDifProdsCompr = est.getTotalDifProdsCompr();
        totalClientes = est.getTotalDifProdsCompr();
        totalCliCompr = est.getTotalCliCompr();
        totalComprasZero = est.getTotalComprasZero();
        faturacaoTotal = est.getFaturacaoTotal();
    }

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
    
    // Getters
    public String getFicheiro() { return ficheiro; }
    public int getTotalVendasErr() { return totalVendasErr; }
    public int getTotalProdutos() { return totalProdutos; }
    public int getTotalDifProdsCompr() { return totalDifProdsCompr; }
    public int getTotalClientes() { return totalClientes; }
    public int getTotalCliCompr() { return totalCliCompr; }
    public int getTotalComprasZero() { return totalComprasZero; }
    public double getFaturacaoTotal() { return faturacaoTotal; }
    // (getters por subtracao)
    public int getTotalProdsNCompr() { return totalProdutos - totalDifProdsCompr; }
    public int getTotalCliNCompr() { return totalClientes - totalCliCompr; }
    
    // Setters
    /*private void setFicheiro(String ficheiro) { this.ficheiro = ficheiro; }
    private void setTotalVendasErr(int totalVendasErr) { this.totalVendasErr = totalVendasErr; }
    private void setTotalProdutos(int totalProdutos) { this.totalProdutos = totalProdutos; }
    private void setTotalDifProdsCompr(int totalDifProdsCompr) { this.totalDifProdsCompr = totalDifProdsCompr; }
    private void setTotalClientes(int totalClientes) { this.totalClientes = totalClientes; }
    private void setTotalCliCompr(int totalCliCompr) { this.totalCliCompr = totalCliCompr; }
    private void setTotalComprasZero(int totalComprasZero) { this.totalComprasZero = totalComprasZero; }
    private void setFaturacaoTotal(double faturacaoTotal) { this.faturacaoTotal = faturacaoTotal; }

    private void incTotalVendasErr() { ++totalVendasErr; }
    private void incfaturacaoTotal(double aumento) { faturacaoTotal += aumento; }
    private void incTotalComprasZero() { totalComprasZero += 1; }*/
    
    public EstatisticasFicheiro clone(){
        return new EstatisticasFicheiro(this);
    }
    
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
    
    public String toString(){
        StringBuilder str = new StringBuilder();
        return str.toString();
    }
}
