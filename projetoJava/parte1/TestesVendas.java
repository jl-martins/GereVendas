package parte1;

import java.util.Set;
import java.util.ArrayList;
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
    
    public static long totalComprasFilial(ArrayList<Venda> vendas, int filial){
        long total = 0;
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                ++total;
        
        return total;
    }
    
    public static long totalComprasPrecoNulo(ArrayList<Venda> vendas){
        long total = 0;
        
        for(Venda v : vendas)
            if(v.getPrecoUnitario() == 0.0)
                ++total;
        
        return total;
    }
    
    public static int numeroVendasDuplicadas(ArrayList<Venda> vendas){
        Set<Venda> conjuntoVendas = new HashSet<>(vendas);
        
        return vendas.size() - conjuntoVendas.size();
    }
        
    public static long numProdsPorInicial(ArrayList<Venda> vendas, char inicial){
        long total = 0;
        
        for(Venda v : vendas)
            if(v.getCodigoProduto().charAt(0) == inicial)
                ++total;
        
        return total;
    }
    
    public static HashSet<String> clientesCompraramFilial_v1(ArrayList<Venda> vendas, int filial){
        HashSet<String> codsClientes = new HashSet<>();
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
    
    public static TreeSet<String> clientesCompraramFilial_v2(ArrayList<Venda> vendas, int filial){
        TreeSet<String> codsClientes = new TreeSet<>(new ComparadorStrings());
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
    
    public static TreeSet<String> clientesCompraramFilial_v3(ArrayList<Venda> vendas, int filial){
        Comparator<String> ordemDecStrings = (s1,s2) -> s1.compareTo(s2);
        TreeSet<String> codsClientes = new TreeSet<>(ordemDecStrings);
        
        for(Venda v : vendas)
            if(v.getFilial() == filial)
                codsClientes.add(v.getCodigoCliente());
        
        return codsClientes;
    }
}
