import java.io.Serializable;
import java.io.IOException;

public class Hipermercado implements Serializable{
    private CatalogoProdutos catalogoProdutos;
    private CatalogoClientes catalogoClientes;
    private Faturacao faturacao;
    private Filiais filiais;
    private EstatisticaFicheiro estatisticaFicheiro;
    private EstatisticaGeral estatisticaGeral;
    
    public Hipermercado(){
        catalogoProdutos = new catalogoProdutos();
        catalogoClientes = new catalogoClientes();
        faturacao = new Faturacao();
        filiais = new Filiais(Constantes.N_FILIAIS);
    }
    
    /** Constrói uma cópia do hipermercado passado como parâmetro. */
    public Hipermercado(Hipermercado hipermercado){
        catalogoProdutos = hipermercado.getCatalogoProdutos();
        catalogoClientes = hipermercado.getCatalogoClientes();
        faturacao = hipermercado.getFaturacao();
        filiais = hipermercado.getFiliais();
        estatisticaFicheiro = hipermercado.getEstatisticaFicheiro();
        estatisticaGeral = hipermercado.getEstatisticaGeral();
    }
    
    /**
     * Grava este Hipermercado em ficheiro.
     * @param fich String com o caminho do ficheiro onde este Hipermercado será gravado.
     */
    public void gravaObj(String fich) throws IOException{
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(fich));
        oos.writeObject(this);
        oos.flush();
        oos.close();
    }

    /**
     * Lê um objeto da classe Hipermercado a partir de um ficheiro .dat
     * @param fich Nome do ficheiro de estado.
     * @return O Hipermercado lido.
     */
    public static Hipermercado leObj(String fich) throws IOException, ClassNotFoundException{
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(fich));

        Hipermercado hipermercado = (Hipermercado) ois.readObject();
        ois.close();
        return hipermercado;
    }
    
    // getters
    private CatalogoProdutos getCatalogoProdutos(){
        return (catalgoProdutos != null) ? catalogoProdutos.clone() : null;
    }
    
    private CatalogoClientes getCatalogoClientes(){
        return (catalogoClientes != null) ? catalogoClientes.clone() : null;
    }
    
    private Faturacao getFaturacao(){
        return (faturacao != null) ? faturacao.clone() : null;
    }
    
    private Filiais getFiliais(){
        return (filiais != null) ? filiais.clone() : null;
    }
    
    public EstatisticaFicheiro getEstatisticaFicheiro(){
        return (estatisticaFicheiro != null) ? estatisticaFicheiro.clone() : null;
    }
    
    public EstatisticaGeral getEstatisticaGeral(){
        return (estatisticaGeral != null) ? estatisticaGeral.clone() : null;
    }
    
    /**
     * Regista um novo produto neste Hipermercado.
     * @param codigoProduto Código do produto a registar.
     * @return @c true se o produto ainda não estava registado neste Hipermercado.
     */
    public boolean registaProduto(String codigoProduto){
        return catalogoProdutos.registaProduto(codigoProduto) && faturacao.registaProduto(codigoProduto);
    }
    
    /**
     * Regista um novo cliente neste Hipermercado.
     * @param codigoCliente Código do cliente a registar.
     * @return @c true se o cliente ainda não estava registado neste Hipermercado.
     */
    public boolean registaCliente(String codigoCliente){
        return catalogoClientes.registaCliente(codigoCliente);
    }
    
    /**
     * Regista uma venda neste Hipermercado.
     * @param v Venda a registar
     * @return 
     */
    public void registaVenda(Venda v){
        faturacao.registaVenda(v);
        filiais.registaVenda(v);
    }
    
    /** @return @c true se neste Hipermercado existir um produto com o código de produto passado como parâmetro. */
    public boolean existeProduto(String codigoProduto){
        return catalogoProdutos.existeProduto(codigoProduto);
    }
    
    /** @return @c true se neste Hipermercado existir um cliente com o código de cliente passado como parâmetro. */
    public boolean existeCliente(String codigoCliente){
        return catalogoClientes.existeCliente(codigoCliente);
    }
    
    public int totalProdutos(){
        return catalogoProdutos.totalProdutos();
    }
    
    public int totalClientes(){
        return catalogoClientes.totalClientes();
    }
    
    // Query1
    /** @return Conjunto ordenado alfabeticamente dos códigos dos produtos que nunca foram comprados. */
    public Set<String> nuncaComprados(){
        return faturacao.nuncaComprados();
    }
    
    // Query2
    /** @return Total global de vendas realizadas no mês passado como parâmetro. */
    public int totalGlobalVendas(int mes){
        return faturacao.totalVendasMes(mes);
    }
    
    /** @return Total de clientes distintos que compraram no mês passado como parâmetro. */
    public int totalClientesCompraram(int mes){
        return filiais.totalClientesCompraram(mes); //MUDAR NOME DO MÉTODO CONFORME O NOME DADO EM FILIAIS
    }
    
    // Query3
    /**
     * Dado um cliente e um mês, devolve um triplo com o número de compras, 
     * produtos distintos comprados e total gasto no mês o escolhido.
     * @param codigoCliente Código do cliente a quem a consulta diz respeito.
     * @param mes Mês para qual o resultado será produzido.
     * @return
     */
    
    // Query4
    
    // Query5
    
    // Query6
    
    // Query7
    
    // Query8
    
    // Query9
    
}
