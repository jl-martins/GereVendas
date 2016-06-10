import java.util.Set;
import java.util.HashSet;
import java.io.Serializable;

/**
 * Classe que representa um catálogo de clientes.
 */
public class CatalogoClientes implements Serializable{
    /** Conjunto dos códigos de clientes no catálogo */
    private Set<String> codigosClientes;
    
    /** Construtores */
    
    /** Constrói um catálogo de clientes vazio. */
    public CatalogoClientes(){
        codigosClientes = new HashSet<>();
    }
    
    /**
     * Constrói uma cópia do catálogo de clientes passado como parâmetro.
     * @param c Catálogo a copiar.
     */
    public CatalogoClientes(CatalogoClientes c){
        this.codigosClientes = new HashSet<>(c.codigosClientes);
    }
    
    /** 
     * Regista um cliente neste catálogo de clientes.
     * @param codigoCliente Código do cliente a registar.
     * @return <code>true</code> se o catálogo de clientes ainda não tinha o cliente passado como parâmetro.
     */
    public boolean registaCliente(String codigoCliente){
        return codigosClientes.add(codigoCliente);
    }
    
    /**
     * Testa se um cliente existe neste catálogo de clientes.
     * @param codigoCliente Código do cliente a procurar neste catálogo de clientes.
     * @return <code>true</code> se o código de cliente passado como parâmetro estiver neste catálogo de clientes.
     */
    public boolean existeCliente(String codigoCliente){
        return codigosClientes.contains(codigoCliente);
    }
    
    /**
     * Devolve o número total de clientes neste catálogo de clientes.
     * @return Total de clientes neste catálogo.
     */
    public int totalClientes(){
        return codigosClientes.size();
    }
    
    /**
     * Cria e devolve uma cópia deste catálogo de clientes.
     * @return Cópia deste CatalogoClientes.
     */
    @Override
    public CatalogoClientes clone(){
        return new CatalogoClientes(this);
    }
    
    /**
     * Testa se este catálogo de clientes é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
            
        CatalogoClientes c = (CatalogoClientes) o;
        return codigosClientes.equals(c.codigosClientes);
    }
    
    /** 
     * Gera e devolve uma representação textual deste catálogo de clientes.
     * @return Representação textual desta instância de CatalogoClientes.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Catálogo de clientes" + separador);
        for(String codigoCliente : codigosClientes)
            sb.append(codigoCliente).append(separador);
        
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o hash code deste catálogo de clientes.
     * @return Valor do hash code deste catálogo de clientes.
     */
    @Override
    public int hashCode(){
        int hash = 7;
        
        for(String codigoCliente : codigosClientes)
            hash = 31*hash + codigoCliente.hashCode();
        return hash;
    }
}
