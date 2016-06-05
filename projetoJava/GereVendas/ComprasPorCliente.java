import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
/**
 * Write a description of class ComprasPorCliente here.
 */
public class ComprasPorCliente
{
    private String codigoCliente;
    private List<Map<String, ComprasDoProduto>> comprasPorMes; /* Deve ter 12 espaços, 1 por mes 
    está a set como placeholder, pode vir a ser outra coisa */
    /* dinheiro gasto por mes */
    private int[] quantasComprasPorMes; /* tamanho 12 ou 13*/
    
    public ComprasPorCliente(String codigoCliente){
        this.codigoCliente = codigoCliente;
        comprasPorMes = new ArrayList<>(13);
        quantasComprasPorMes = new int[13];
        
        for(int i = 0; i < 13; i++){
            comprasPorMes.add(new HashMap<>());
        }
    }
    
    public void registaVenda(Venda v){
        int mes = v.getMes();
        String codigoProduto = v.getCodigoProduto();
        quantasComprasPorMes[mes]++;
        Map<String, ComprasDoProduto> aux = comprasPorMes.get(mes);
        ComprasDoProduto comprasProduto = aux.get(codigoProduto);
        
        if(comprasProduto == null){
            comprasProduto = new ComprasDoProduto(codigoProduto);
            aux.put(codigoProduto, comprasProduto);
        }
        
        comprasProduto.registaVenda(v);
    }
    
    
}
