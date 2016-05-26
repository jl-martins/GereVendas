import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.TreeSet;
import java.util.Comparator;

/**
 * Write a description of class Testes_Vendas here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class TestesVendas{
    
    public static int totalComprasFilial(List<Venda> vendas, int filial){
        int total = 0;
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                ++total;
        
        return total;
    }
    
    public static int totalComprasPrecoNulo(List<Venda> vendas){
        int total = 0;
        
        for(Venda v : vendas)
            if(v.getPrecoUnitario() == 0.0)
                ++total;
        
        return total;
    }
    
    public static int numeroVendasDuplicadas(List<Venda> vendas){
        Set<Venda> conjuntoVendas = new HashSet<>(vendas);
        
        return vendas.size() - conjuntoVendas.size();
    }
        
    public static int numProdsPorInicial(List<Venda> vendas, char inicial){
        int total = 0;
        
        for(Venda v : vendas)
            if(v.getCodigoProduto().charAt(0) == inicial)
                ++total;
        
        return total;
    }
    
    public static HashSet<String> clientesCompraramFilial_v1(List<Venda> vendas, int filial){
        HashSet<String> codsClientes = new HashSet<>();
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
    
    public static TreeSet<String> clientesCompraramFilial_v2(List<Venda> vendas, int filial){
        TreeSet<String> codsClientes = new TreeSet<>(new ComparadorStrings());
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
    
    public static TreeSet<String> clientesCompraramFilial_v3(List<Venda> vendas, int filial){
        Comparator<String> ordemDecStrings = (s1,s2) -> s1.compareTo(s2);
        TreeSet<String> codsClientes = new TreeSet<>(ordemDecStrings);
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
}
