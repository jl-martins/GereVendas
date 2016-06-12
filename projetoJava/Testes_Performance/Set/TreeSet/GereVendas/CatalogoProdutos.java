import java.util.Set;
import java.util.TreeSet;
import java.io.Serializable;

/**
 * Classe que representa um catálogo de produtos.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class CatalogoProdutos implements Serializable{
    /** Conjunto dos códigos de produtos no catálogo */
    private Set<String> codigosProdutos;
    
    /** Construtores */
    
    /** Constrói um catálogo de produtos vazio. */
    public CatalogoProdutos(){
        codigosProdutos = new TreeSet<>(); // permite inserir e consultar códigos de produtos em tempo tendencialmente constante.
    }
    
    /**
     * Constrói uma cópia do catálogo de produtos passado como parâmetro.
     * @param c Catálogo a copiar.
     */
    public CatalogoProdutos(CatalogoProdutos c){
        this.codigosProdutos = new TreeSet<>(c.codigosProdutos);
    }
    
    /** 
     * Regista um produto neste catálogo de produtos.
     * @param codigoProduto Código do produto a registar.
     * @return <code>true</code> se o catálogo de produtos ainda não tinha o produto passado como parâmetro.
     */
    public boolean registaProduto(String codigoProduto){
        return codigosProdutos.add(codigoProduto);
    }
    
    /**
     * Testa se um produto existe neste catálogo de produtos.
     * @param codigoProduto Código do produto a procurar neste catálogo de produtos.
     * @return <code>true</code> se o código de produto passado como parâmetro estiver neste catálogo de produtos.
     */
    public boolean existeProduto(String codigoProduto){
        return codigosProdutos.contains(codigoProduto);
    }
    
    /**
     * Devolve o número total de produtos neste catálogo de produtos.
     * @return Total de produtos neste catálogo.
     */
    public int totalProdutos(){
        return codigosProdutos.size();
    }
    
    /**
     * Cria e devolve uma cópia deste catálogo de produtos.
     * @return Cópia deste CatalogoProdutos.
     */
    @Override
    public CatalogoProdutos clone(){
        return new CatalogoProdutos(this);
    }
    
    /**
     * Testa se este catálogo de produtos é igual ao objeto passado como parâmetro.
     * @return <code>true</code> se os objetos comparados forem iguais.
     */
    @Override
    public boolean equals(Object o){
        if(this == o)
            return true;
        else if(o == null || this.getClass() != o.getClass())
            return false;
            
        CatalogoProdutos c = (CatalogoProdutos) o;
        return codigosProdutos.equals(c.codigosProdutos);
    }
    
    /** 
     * Gera e devolve uma representação textual deste catálogo de produtos.
     * @return Representação textual desta instância de CatalogoProdutos.
     */
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Catálogo de produtos" + separador);
        for(String codigoProduto : codigosProdutos)
            sb.append(codigoProduto).append(separador);
        
        return sb.toString();
    }
    
    /**
     * Calcula e devolve o <i>hash code</i> deste catálogo de produtos.
     * @return Valor do <i>hash code</i> deste catálogo de produtos.
     */
    @Override
    public int hashCode(){
        int hash = 7;
        
        for(String codigoProduto : codigosProdutos)
            hash = 31*hash + codigoProduto.hashCode();
        return hash;
    }
}
