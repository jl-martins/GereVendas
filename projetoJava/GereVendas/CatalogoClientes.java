import java.util.Set;
import java.util.HashSet;

/**
 * Write a description of class CatalogoClientes here.
 */
public class CatalogoClientes implements java.io.Serializable
{
    private Set<String> codigosClientes;
    
    public CatalogoClientes(){
        codigosClientes = new HashSet<>();
    }
    
    public CatalogoClientes(CatalogoClientes c){
        this.codigosClientes = new HashSet<>(c.codigosClientes);
    }
    
    public boolean addClientes(String cliente){
        return codigosClientes.add(cliente);
    }
    
    int totalClientes(){
        return codigosClientes.size();
    }
}
