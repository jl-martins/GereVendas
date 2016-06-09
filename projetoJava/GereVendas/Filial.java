import java.util.Set;
import java.util.Map;
import java.util.List;
import java.util.Queue;
import java.util.Comparator;
import java.util.Collections;

import java.util.TreeSet;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.PriorityQueue;

import java.util.Arrays;
import java.util.stream.Collectors;

public class Filial implements java.io.Serializable{
    private List<Map<String, ComprasPorCliente>> clientesOrdenados; /* deve ter tamanho 26 para organizar os clientes pelo 
    1o char do codigo, a cada cliente faz corresponder as suas compras */
    
    public Filial(){
        clientesOrdenados = new ArrayList<>(26);
        for(int i = 0; i < 26; i++){
            clientesOrdenados.add(new HashMap<>());
        }
    }
    
    /** Dado uma String com o código do cliente, devolve o indíce correspondente na clientesOrdenados . */
    private static int indiceCorrespondente(String codigoCliente){
        if(codigoCliente == null)
            ;/*atirar excepçoes*/
        char c = codigoCliente.charAt(0);
        c = Character.toUpperCase(c);
        int indice = ((int) c) - ((int) 'A');
        if(indice < 0 || indice >= 26)
            ;
        return indice;
    }
    
    private ComprasPorCliente comprasDoCliente(String codigoCliente){
        int indice = indiceCorrespondente(codigoCliente);
        Map<String, ComprasPorCliente> mapCorrespondente = clientesOrdenados.get(indice);
        ComprasPorCliente compras = mapCorrespondente.get(codigoCliente);
        return compras;
    }
    
    public void registaVenda(Venda v){
        String codigoCliente = v.getCodigoCliente();
        int indice = indiceCorrespondente(codigoCliente);
        ComprasPorCliente compras;
        Map<String, ComprasPorCliente> aux = clientesOrdenados.get(indice);
        
        compras = aux.get(codigoCliente);
        if(compras == null){
            compras = new ComprasPorCliente(codigoCliente);
            aux.put(codigoCliente, compras);
        }
        
        compras.registaVenda(v);        
    }
    
    public Set<String> clientesCompraramMes(int mes){
        Set<String> clientes = new TreeSet<>();
        for(int i = 0; i < 26; i++){
            clientes.addAll(clientesOrdenados.get(i).entrySet().stream().filter(p->p.getValue().comprouNoMes(mes)).map(p->p.getKey()).collect(Collectors.toSet()));   
        }
        return clientes;
    }
    
    public Set<ComprasDoProduto> comprasFeitasMes(String codigoCliente, int mes){
        ComprasPorCliente compras = comprasDoCliente(codigoCliente);  
        if(compras == null)
            return null;
        return compras.comprasPorMes(mes);        
    }
    
    public int[] quantasComprasPorMes(String codigoCliente){
        return comprasDoCliente(codigoCliente).getQuantasComprasPorMes();
    }
    
    public Set<String> clientesCompraramProdutoMes(String codigoProduto, int mes){
        Set<String> res = new TreeSet<>();
    
        for(Map<String, ComprasPorCliente> m : clientesOrdenados)
            for(ComprasPorCliente cpc : m.values())
                if(cpc.comprouProdutoMes(codigoProduto, mes))
                    res.add(cpc.getCodigoCliente());
        return res;
    }
    
    // Query6
    public Set<String> clientesCompraramProduto(String codigoProduto){
        Set<String> res = new TreeSet<>();
    
        for(Map<String, ComprasPorCliente> map : clientesOrdenados)
            for(ComprasPorCliente cpc : map.values())
                if(cpc.comprouProduto(codigoProduto))
                    res.add(cpc.getCodigoCliente());
        return res;
    }

    // Query7
    public ParCliFat[] tresMaioresCompradores(){
        Comparator<ParCliFat> ordemCrescFaturacao = 
            (p1,p2) -> p1.getFat() < p2.getFat() ? -1 : p1.getFat() > p2.getFat() ? 1 : 0;
        /* o comprador do top3 que gastou menos dinheiro e mantido na cabeca da queue para que      *
         * possa ser facilmente removido, caso surja um comprador que gastou mais dinheiro que ele. */
        Queue<ParCliFat> top3 = new PriorityQueue<>(3, ordemCrescFaturacao);
    
        for(Map<String, ComprasPorCliente> map : clientesOrdenados){
            for(ComprasPorCliente cpc : map.values()){
                ParCliFat par = new ParCliFat(cpc.getCodigoCliente(), cpc.totalGastoAno());
    
                if(top3.size() < 3)
                    top3.add(par);
                else if(ordemCrescFaturacao.compare(top3.peek(), par) < 0){
                    top3.poll();
                    top3.add(par);
                }
            }
        }
        ParCliFat[] arrTop3 = top3.toArray(new ParCliFat[3]);
        Arrays.sort(arrTop3, Collections.reverseOrder(ordemCrescFaturacao));
        return arrTop3;
    }
    
    // Query8
    public Map<String, Set<String>> mapClienteProdsComp(){
    	Map<String, Set<String>> res = new HashMap<>();
    
    	for(Map<String, ComprasPorCliente> map : clientesOrdenados)
    		for(ComprasPorCliente cpc : map.values())
    			res.put(cpc.getCodigoCliente(), cpc.produtosCompradosAno());
    
    	return res;
    }
    
    // Query9
    public Map<String, ParQtdValor> mapClienteParQtdValor(String codigoProduto){
    	Map<String, ParQtdValor> res = new HashMap<>();
    
    	for(Map<String, ComprasPorCliente> map : clientesOrdenados)
    		for(ComprasPorCliente cpc : map.values())
    			res.put(cpc.getCodigoCliente(), cpc.totalCompradoEgasto(codigoProduto));
    
    	return res;
    }
}
