import java.util.Set;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.HashMap;
import java.util.stream.Collectors;

/**
 * Write a description of class Filial here.
 */

public class Filial
{
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
            ;
        char c = codigoCliente.charAt(0);
        c = Character.toUpperCase(c);
        int indice = ((int) c) - ((int) 'A');
        if(indice < 0 || indice >= 26)
            ;
        return indice;
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
}
