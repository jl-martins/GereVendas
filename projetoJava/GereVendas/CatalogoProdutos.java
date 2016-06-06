import java.util.Set;
import java.util.HashSet;
import java.io.Serializable;

public class CatalogoProdutos implements Serializable{
    private Set<String> codigosProdutos;

    public CatalogoProdutos(){
        codigosProdutos = new HashSet<>();
    }
    
    public CatalogoProdutos(CatalogoProdutos c){
        this.codigosProdutos = new HashSet<>(c.codigosProdutos);
    }

    public boolean registaProduto(String codigoProduto){
        return codigosProdutos.add(codigoProduto);
    }
    
    public boolean existeProduto(String codigoProduto){
        return codigosProdutos.contains(codigoProduto);
    }
    
    public int totalClientes(){
        return codigosProdutos.size();
    }
    
    public CatalogoProdutos clone(){
        return new CatalogoProdutos(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
            
        CatalogoProdutos c = (CatalogoProdutos) o;
        return codigosProdutos.equals(c.codigosProdutos);
    }
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Catálogo de produtos" + separador);
        for(String codigoProduto : codigosProdutos)
            sb.append(codigoProduto).append(separador);
        
        return sb.toString();
    }
    
    public int hashCode(){
        int hash = 7;
        
        for(String codigoProduto : codigosProdutos)
            hash = 31*hash + codigoProduto.hashCode();
        return hash;
    }
}
