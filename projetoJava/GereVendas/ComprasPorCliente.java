import java.util.Map;
import java.util.Set;
import java.util.List;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.HashSet;

import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Write a description of class ComprasPorCliente here.
 */
public class ComprasPorCliente implements java.io.Serializable{
    private String codigoCliente;
    private List<Map<String, ComprasDoProduto>> comprasPorMes; /* Deve ter 12 espaços, 1 por mes 
    está a set como placeholder, pode vir a ser outra coisa */
    /* dinheiro gasto por mes */
    private int[] quantasComprasPorMes; /* tamanho 12 ou 13*/
    
    private ComprasPorCliente(){}
    
    public ComprasPorCliente(String codigoCliente){
        this.codigoCliente = codigoCliente;
        comprasPorMes = new ArrayList<>(13);
        quantasComprasPorMes = new int[13];
        
        for(int i = 0; i < 13; i++){
            comprasPorMes.add(new HashMap<>());
        }
    }
    
    /** @return Código do cliente a que este ComprasPorCliente diz respeito. */
    public String getCodigoCliente(){ return codigoCliente; }
    
    public int[] getQuantasComprasPorMes(){
        int[] ret = new int[13];
        System.arraycopy(quantasComprasPorMes, 1, ret, 1, 12);
        return ret;
    }
    
    /**
     * Regista uma venda neste objeto ComprasPorCliente.
     * @param v Venda a registar.
     */
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
    
    /** @return @c true se o cliente desta ComprasPorCliente comprou no mês passado como parâmetro. */
    public boolean comprouNoMes(int mes){
        return quantasComprasPorMes[mes] > 0;
    }
    
    public Set<ComprasDoProduto> comprasPorMes(int mes){
        Map<String, ComprasDoProduto> mapAux = comprasPorMes.get(mes);
        return mapAux.values().stream().map(ComprasDoProduto::clone).collect(Collectors.toCollection(HashSet::new));        
    }
    
    public boolean comprouProdutoMes(String codigoProduto, int mes){
        return comprasPorMes.get(mes).containsKey(codigoProduto);
    }
    
    // Query6
    public boolean comprouProduto(String codigoProduto){
        boolean comprou = false;
    
        for(int mes = 1; mes <= Constantes.N_MESES && !comprou; ++mes)
            comprou = comprouProdutoMes(codigoProduto, mes);
    
        return comprou;
    }
    
    // Query7
    public double totalGastoAno(){
        double total = 0.0;
    
        for(Map<String, ComprasDoProduto> map : comprasPorMes)
            for(ComprasDoProduto cdp : map.values())
                total += cdp.getFaturacao();
    
        return total;
    }
    
    // Query8
    public Set<String> produtosCompradosAno(){
        Set<String> produtosComprados = new TreeSet<>();
    
        for(Map<String, ComprasDoProduto> map : comprasPorMes)
            produtosComprados.addAll(map.keySet());
    
        return produtosComprados;
    }
    
    // Query9
    public ParQtdValor totalCompradoEgasto(String codigoProduto){
    	int qtdTotal = 0;
    	double valorTotal = 0.0;
    
    	for(Map<String, ComprasDoProduto> map : comprasPorMes){
    		ComprasDoProduto compraMes = map.get(codigoProduto);
    
    		if(compraMes != null){ // o cliente comprou o produto no mes a que o Map<String, ComprasDoProduto> atual diz respeito
    			qtdTotal += compraMes.getUnidadesCompradas();
    			valorTotal += compraMes.getFaturacao();
    		}
    	}
    	return new ParQtdValor(qtdTotal, valorTotal);
    }
    /* definir compareTo a ordenar por nome */
}
