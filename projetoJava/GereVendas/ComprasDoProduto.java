import java.util.Arrays;

public class ComprasDoProduto implements java.io.Serializable, Comparable<ComprasDoProduto>{
    private String codigoProduto;
    private int unidadesCompradas;
    private double faturacao;

    /* Construtores */
    private ComprasDoProduto(){}

    public ComprasDoProduto(String codigoProduto){
        this.codigoProduto = codigoProduto;
        unidadesCompradas = 0;
        faturacao = 0;
    }   

    public ComprasDoProduto(String codigoProduto, int unidadesCompradas, double faturacao){
        this.codigoProduto = codigoProduto;
        this.unidadesCompradas = unidadesCompradas;
        this.faturacao = faturacao;
    }

    public ComprasDoProduto(ComprasDoProduto comprasDoProduto){
        this(comprasDoProduto.getCodigoProduto(), comprasDoProduto.getUnidadesCompradas(), comprasDoProduto.getFaturacao());
    }

    public String getCodigoProduto(){
        return codigoProduto;
    }

    public int getUnidadesCompradas(){
        return unidadesCompradas;
    }

    public double getFaturacao(){
        return faturacao;
    }

    public void registaVenda(Venda v){
        int unidadesVendidas = v.getUnidadesVendidas();
        unidadesCompradas += unidadesVendidas;
        faturacao += v.getPrecoUnitario() * unidadesVendidas;
    }

    /* Outros métodos */
    public ComprasDoProduto clone(){
        return new ComprasDoProduto(this);
    }

    public int hashCode(){
        return Arrays.hashCode(new Object[]{codigoProduto, unidadesCompradas, faturacao});
    }

    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || o.getClass() != this.getClass())
            return true;
        ComprasDoProduto c = (ComprasDoProduto) o;
        String thisCodigoProduto = this.getCodigoProduto();
        String cCodigoProduto = c.getCodigoProduto();
        return  (codigoProduto != null? codigoProduto.equals(cCodigoProduto) : cCodigoProduto == null) &&
                this.getUnidadesCompradas() == c.getUnidadesCompradas() &&
                this.getFaturacao() == c.getFaturacao();
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Código do Produto: ");
        sb.append(codigoProduto);
        sb.append("\nUnidades compradas do produto: ");
        sb.append(unidadesCompradas);
        sb.append("\nDinheiro gasto no produto: ");
        sb.append(faturacao + "\n");
        return sb.toString();
    }
    public int compareTo(ComprasDoProduto cpd){
        if(this.codigoProduto == null)
            return cpd.getCodigoProduto() == null ? 0 : -1;
        return cpd.codigoProduto.compareTo(cpd.getCodigoProduto());
    }
}
