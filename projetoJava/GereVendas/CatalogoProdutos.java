import java.util.Set;
import java.util.HashSet;

/**
 * Write a description of class CatalogoProdutos here.
 */
public class CatalogoProdutos implements java.io.Serializable
{
    private Set<String> codigosProdutos;

    public CatalogoProdutos(){
        codigosProdutos = new HashSet<>();
    }

    public CatalogoProdutos(CatalogoProdutos c){
        this.codigosProdutos = new HashSet<>(c.codigosProdutos);
    }

    public boolean addClientes(String cliente){
        return codigosProdutos.add(cliente);
    }

    int totalClientes(){
        return codigosProdutos.size();
    }
}
