
/**
 * Write a description of class ComprasDoProduto here.
 */
public class ComprasDoProduto
{
    private String codigoProduto;
    private int unidadesCompradas;
    private double faturacao;
    /* private boolean comprouModoN, comprouModoP; -> manter só se for preciso para alguma query */
    
    private ComprasDoProduto(){}
    
    public ComprasDoProduto(String codigoProduto, int unidadesCompradas, double faturacao){
        this.codigoProduto = codigoProduto;
        this.unidadesCompradas = unidadesCompradas;
        this.faturacao = faturacao;
    }
    
    public ComprasDoProduto(ComprasDoProduto comprasDoProduto){
        this(comprasDoProduto.getCodigoProduto(), comprasDoProduto.getUnidadesCompradas(), comprasDoProduto.getFaturacao());
    }
    
    public ComprasDoProduto clone(){
        return new ComprasDoProduto(this);
    }
}
