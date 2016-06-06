import java.util.Set;
import java.util.HashSet;
import java.io.Serializable;

public class CatalogoClientes implements Serializable{
    private Set<String> codigosClientes;
    
    public CatalogoClientes(){
        codigosClientes = new HashSet<>();
    }
    
    public CatalogoClientes(CatalogoClientes c){
        this.codigosClientes = new HashSet<>(c.codigosClientes);
    }
    
    public boolean registaCliente(String codigoCliente){
        return codigosClientes.add(codigoCliente);
    }
    
    public boolean existeCliente(String codigoCliente){
        return codigosClientes.contains(codigoCliente);
    }
    
    public int totalClientes(){
        return codigosClientes.size();
    }
    
    public CatalogoClientes clone(){
        return new CatalogoClientes(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
            
        CatalogoClientes c = (CatalogoClientes) o;
        return codigosClientes.equals(c.codigosClientes);
    }
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Catálogo de clientes" + separador);
        for(String codigoCliente : codigosClientes)
            sb.append(codigoCliente).append(separador);
        
        return sb.toString();
    }
    
    public int hashCode(){
        int hash = 7;
        
        for(String codigoCliente : codigosClientes)
            hash = 31*hash + codigoCliente.hashCode();
        return hash;
    }
}
