import java.io.Serializable;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;

import java.util.Set;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class Hipermercado implements Serializable{
    private CatalogoProdutos catalogoProdutos;
    private CatalogoClientes catalogoClientes;
    private Faturacao faturacao;
    private Filiais filiais;
    private EstatisticasFicheiro estatisticasFicheiro;
    private EstatisticasGerais estatisticasGerais;
    
    public Hipermercado(){
        catalogoProdutos = new CatalogoProdutos();
        catalogoClientes = new CatalogoClientes();
        faturacao = new Faturacao();
        filiais = new Filiais(Constantes.N_FILIAIS);
    }
    
    /** Constrói uma cópia do hipermercado passado como parâmetro. */
    public Hipermercado(Hipermercado hipermercado){
        catalogoProdutos = hipermercado.getCatalogoProdutos();
        catalogoClientes = hipermercado.getCatalogoClientes();
        faturacao = hipermercado.getFaturacao();
        filiais = hipermercado.getFiliais();
        estatisticasFicheiro = hipermercado.getEstatisticasFicheiro();
        estatisticasGerais = hipermercado.getEstatisticasGerais();
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
    
    public void criaEstatisticasFicheiro(String fich, int totalVendasErr, int totalProdsDif, int totalClisComp, int nGratis, double faturacaoTotal){
        estatisticasFicheiro = new EstatisticasFicheiro(
            fich, totalVendasErr, catalogoProdutos.totalProdutos(), totalProdsDif, 
            catalogoClientes.totalClientes(), totalClisComp, nGratis, faturacaoTotal
        );
    }
    
    private void criaEstatisticasGerais(){
        estatisticasGerais = new EstatisticasGerais(
            faturacao.comprasPorMes(),            // total de compras por mes
            faturacao.faturacaoPorFilialPorMes(), // faturacao por filial, para cada mes
            filiais.quantosClientesPorMes() // numero de clientes distintos que compraram em cada mes
        );
    }
    
    // getters
    private CatalogoProdutos getCatalogoProdutos(){
        return (catalogoProdutos != null) ? catalogoProdutos.clone() : null;
    }
    
    private CatalogoClientes getCatalogoClientes(){
        return (catalogoClientes != null) ? catalogoClientes.clone() : null;
    }
    
    private Faturacao getFaturacao(){
        return (faturacao != null) ? faturacao.clone() : null;
    }
    
    private Filiais getFiliais(){
        return null; // MUDAR PARA ESTE CODIGO --> (filiais != null) ? filiais.clone() : null;
    }
    
    public EstatisticasFicheiro getEstatisticasFicheiro(){
        return (estatisticasFicheiro != null) ? estatisticasFicheiro.clone() : null;
    }
    
    public EstatisticasGerais getEstatisticasGerais(){
        if(estatisticasGerais == null)
            criaEstatisticasGerais(); // cria estatisticas gerais a partir dos dados da faturacao e das filiais
        return (estatisticasGerais != null) ? estatisticasGerais.clone() : null;
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
    
    public void requerMesValido(int mes) throws MesInvalidoException {
        if(mes < 1 || mes > Constantes.N_MESES)
            throw new MesInvalidoException("O mês '" + mes + "' é inválido!");
    }
    
    public void requerFilialValida(int filial) throws FilialInvalidaException {
        if(filial < 1 || filial > filiais.quantasFiliais())
            throw new FilialInvalidaException("A filial '" + filial + "' é inválida!");
    }
    
    // Query1
    /** @return Conjunto ordenado alfabeticamente dos códigos dos produtos que nunca foram comprados. */
    public Set<String> nuncaComprados(){
        return faturacao.nuncaComprados();
    }
    
    // Query2
    /** @return Total global de vendas realizadas no mês passado como parâmetro. */
    public int totalGlobalVendas(int mes) throws MesInvalidoException{
        requerMesValido(mes);
        return faturacao.totalVendasMes(mes);
    }
    
    /** @return Total de clientes distintos que compraram no mês passado como parâmetro. */
    public int totalClientesCompraram(int mes) throws MesInvalidoException{
        requerMesValido(mes);
        return filiais.quantosClientesCompraramMes(mes);
    }
    
    // Query3
    /**
     * Dado um cliente e um mês, devolve uma lista de triplos (1 por mês, lista indexada de 1 a 12). 
     * O triplo contém o total de compras do mes(para obter, invoca-se o método getInt1), o numero de produtos comprados e o total gasto.
     * @param codigoCliente Código do cliente a quem a consulta diz respeito.
     */
    public List<TriploComprasProdutosGasto> infoPorMes(String codigoCliente){
        int[] quantasComprasPorMes = filiais.quantasComprasPorMes(codigoCliente);
        List<TriploComprasProdutosGasto> dadosPorMes = new ArrayList<>(13);
        dadosPorMes.add(null); /* adiciona padding para podermos aceder ao mes pelo seu indice sem fazer -1 */
        for(int i = 1; i <= Constantes.N_MESES; i++){
            List<ComprasDoProduto> comprasDoMes = filiais.comprasFeitasMes(codigoCliente, i);
            int totalCompras = quantasComprasPorMes[i];
            int produtosDistintosComprados = Filiais.quantosProdutosDistintosComprou(comprasDoMes);
            double totalGasto = Filiais.quantoGastou(comprasDoMes); 
            
            dadosPorMes.add(new TriploComprasProdutosGasto(totalCompras, produtosDistintosComprados, totalGasto));
        }
        return dadosPorMes;
    }
    
    // Query4
    public FatProdMes getFatProdMes(String codigoProduto, int mes) throws MesInvalidoException {
        requerMesValido(mes);
        return faturacao.getFatProdMes(codigoProduto, mes);
    }
    
    public int quantosCompraramProdutoMes(String codigoProduto, int mes) throws MesInvalidoException {
        requerMesValido(mes);
        return filiais.quantosCompraramProdutoMes(codigoProduto, mes);
    }
    
    // Query5
    public List<ParProdQtd> produtosMaisComprados(String idCliente){
        List<ComprasDoProduto> compras = filiais.comprasFeitasTotal(idCliente);
        return filiais.produtosMaisComprados(compras);
    }
    
    // Query6
    public List<TriploProdQtdClis> maisVendidos(int X){
        List<ParProdQtd> prodsMaisVendidos = faturacao.maisVendidos(X);
        
        if(prodsMaisVendidos.isEmpty()) // X <= 0
            return Collections.emptyList();
        
        List<TriploProdQtdClis> res = new ArrayList<>(X);
        for(int i = 0; i < X; ++i){
            ParProdQtd par = prodsMaisVendidos.get(i);
            String prod = par.getProd(); // evita invocar getProd() 2 vezes por iteração
            res.add(new TriploProdQtdClis(prod, par.getQtd(), filiais.quantosCompraramProduto(prod)));
        }
        return res;
    }
    // Query7
    public ParCliFat[] tresMaioresCompradores(int filial){
        // requerFilialValida(filial);
        return filiais.tresMaioresCompradores(filial);
    }
    // Query8
    public List<ParCliProdsDif> clisCompraramMaisProdsDif(int X){
        return filiais.clisCompraramMaisProdsDif(X);
    }
    
    // Query9
    public List<TriploCliQtdGasto> clientesMaisCompraram(String codigoProduto, int X){
        return filiais.clientesMaisCompraram(codigoProduto, X);
    }
}
