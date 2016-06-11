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

/**
 * Classe agregadora do projeto <strong>GereVendas</strong>.
 * 
 * <p>Cada instância de Hipermercado permite o registo de produtos, clientes e vendas.
 * O registo das vendas só deve ser efetuado depois do registo de todos os clientes e
 * produtos que possam surgir nas mesmas. Depois de criado um Hipermercado, é possível:
 * <op>
 *      <it> Registar e consultar estatísticas relativas ao ficheiro de vendas lidos ao criá-lo;</it>
 *      <it> Consultar estatísticas gerais sobre os dados guardados no mesmo;</it>
 *      <it> Obter várias informações sobre os produtos, clientes e vendas;</it>
 *      <it> Gravar a instância numa <code>ObjectStream</code> para posterior leitura;</it>
 * </op>
 * 
 * @author LI3_Grupo1
 * @version 1.0 (6/2016)
 */
public class Hipermercado implements Serializable{
    /** Catálogo com todos os produtos  */
    private CatalogoProdutos catalogoProdutos;
    
    /** Catálogo com todos os clientes  */
    private CatalogoClientes catalogoClientes;
    
    /** Guarda informação sobre a faturação mensal e do ano, para cada filial e no global. */
    private Faturacao faturacao;
    
    /** Cada uma das filiais de um hipermercado. */
    private Filiais filiais;
    
    /** Estatísticas relativas ao último ficheiro de vendas lido. */
    private EstatisticasFicheiro estatisticasFicheiro;
    
    /** Estatísticas gerais relativas aos dados registados nas estruturas de um hipermercado. */
    private EstatisticasGerais estatisticasGerais;
    
    /**
     * Constrói um hipermercado vazio, i.e.: sem qualquer cliente, produto ou venda registada.
     */
    public Hipermercado(){
        catalogoProdutos = new CatalogoProdutos();
        catalogoClientes = new CatalogoClientes();
        faturacao = new Faturacao();
        filiais = new Filiais(Constantes.N_FILIAIS);
    }
    
    /** 
     * Constrói uma cópia do hipermercado passado como parâmetro. 
     * @param hipermercado Hipermercado a copiar.
     */
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
    
    /**
     * Cria as estatísticas do último ficheiro lido, a partir dos dados passados 
     * como parâmetros e da informação existente neste Hipermercado.
     * @param fich Ficheiro lido.
     * @param totalVendasErr Número total de registos de venda errados.
     * @param totalProdsDif Total de produtos diferentes comprados.
     * @param totalClisComp Total de clientes que realizaram compras.
     * @param nGratis Número de vendas de valor total igual a 0.0 .
     * @param faturacaoTotal Faturação total.
     */
    public void criaEstatisticasFicheiro(String fich, int totalVendasErr, int totalProdsDif, int totalClisComp, int nGratis, double faturacaoTotal){
        estatisticasFicheiro = new EstatisticasFicheiro(
            fich, totalVendasErr, catalogoProdutos.totalProdutos(), totalProdsDif, 
            catalogoClientes.totalClientes(), totalClisComp, nGratis, faturacaoTotal
        );
    }
    
    /** Cria as estatísticas gerais, com o auxílio das informações contidas na faturação e nas filiais. */
    private void criaEstatisticasGerais(){
        estatisticasGerais = new EstatisticasGerais(
            faturacao.comprasPorMes(),            // total de compras por mes
            faturacao.faturacaoPorFilialPorMes(), // faturacao por filial, para cada mes
            filiais.quantosClientesPorMes()       // numero de clientes distintos que compraram em cada mes
        );
    }
    
    /** Getters */
    
    /** 
     * Devolve uma cópia do catálogo de produtos deste Hipermercado
     * (declarado como privado porque o utilizador pode obter toda a informação relativa aos códigos de produto
     *  existentes sem ter que saber que as instâncias de Hipermercado têm um catálogo de produtos. Este método
     *  é usado como auxiliar no construtor de cópia.)
     */
    private CatalogoProdutos getCatalogoProdutos(){
        return (catalogoProdutos != null) ? catalogoProdutos.clone() : null;
    }
    
    /** 
     * Devolve uma cópia do catálogo de clientes deste Hipermercado.
     * (declarado como privado pelas mesmas razões apresentadas para getCatalogoProdutos() )
     */
    private CatalogoClientes getCatalogoClientes(){
        return (catalogoClientes != null) ? catalogoClientes.clone() : null;
    }
    
    /**
     * Devolve uma cópia da faturação deste Hipermercado.
     * (declarado como privado porque o utilizador pode obter toda a informação que necessita sobre a faturação
     *  mensal e anual sem saber que uma instância de Hipermercado tem um objeto da classe Faturacao. )
     */
    private Faturacao getFaturacao(){
        return (faturacao != null) ? faturacao.clone() : null;
    }
    
    /**
     * Devolve uma cópia das filiais deste Hipermercado.
     * (declarado como privado pelas razões que foram apresentadas para os catálogos e faturação. )
     */
    private Filiais getFiliais(){
        return null; // MUDAR PARA ESTE CODIGO --> (filiais != null) ? filiais.clone() : null;
    }
    
    /**
     * Devolve as estatísticas do ficheiro de vendas usado na construção deste Hipermercado.
     */
    public EstatisticasFicheiro getEstatisticasFicheiro(){
        return estatisticasFicheiro; // as instâncias de EstatisticasFicheiro são imutáveis, logo não quebramos o encapsulamento.
    }
    
    /**
     * Devolve uma cópia das estatísticas gerais relativas aos dados registados neste Hipermercado.
     * @return Cópia das estatísticas gerais deste Hipermercado.
     */
    public EstatisticasGerais getEstatisticasGerais(){
        if(estatisticasGerais == null)
            criaEstatisticasGerais(); // cria estatisticas gerais a partir dos dados da faturacao e das filiais
        return (estatisticasGerais != null) ? estatisticasGerais.clone() : null;
    }
    
    /**
     * Regista um novo produto neste Hipermercado.
     * @param codigoProduto Código do produto a registar.
     * @return <code>true</code> se o produto ainda não estava registado neste Hipermercado.
     */
    public boolean registaProduto(String codigoProduto){
        return catalogoProdutos.registaProduto(codigoProduto) && faturacao.registaProduto(codigoProduto);
    }
    
    /**
     * Regista um novo cliente neste Hipermercado.
     * @param codigoCliente Código do cliente a registar.
     * @return <code>true</code> se o cliente ainda não estava registado neste Hipermercado.
     */
    public boolean registaCliente(String codigoCliente){
        return catalogoClientes.registaCliente(codigoCliente);
    }
    
    /**
     * Regista uma venda neste Hipermercado.
     * @param v Venda a registar.
     */
    public void registaVenda(Venda v){
        faturacao.registaVenda(v);
        filiais.registaVenda(v);
    }
    
    /** 
     * Verifica se este hipermercado tem um produto com o código de produto passado como parâmetro.
     * @param codigoProduto Código de produto a procurar neste Hipermercado.
     * @return <code>true</code> se neste Hipermercado existir um produto com o código de produto passado como parâmetro.
     */
    public boolean existeProduto(String codigoProduto){
        return catalogoProdutos.existeProduto(codigoProduto);
    }
    
    /** 
     * Verifica se este hipermercado tem um produto com o código de produto passado como parâmetro.
     * @param codigoProduto Código de produto a procurar neste Hipermercado.
     * @return <code>true</code> se neste Hipermercado existir um produto com o código de produto passado como parâmetro.
     */
    public boolean existeCliente(String codigoCliente){
        return catalogoClientes.existeCliente(codigoCliente);
    }
    
    /**
     * Devolve o total de produtos registados neste hipermercado.
     * @return Total de produtos.
     */
    public int totalProdutos(){
        return catalogoProdutos.totalProdutos();
    }
    
    /**
     * Devolve o total de clientes registados neste hipermercado.
     * @return Total de clientes.
     */
    public int totalClientes(){
        return catalogoClientes.totalClientes();
    }
    
    /**
     * Verifica se o mês passado como parâmetro é válido.
     * @param mes Mês a validar.
     * @throws MesInvalidoException se o mês passado como parâmetro for inválido (i.e. se não pertencer ao intervalo [1,12])
     */
    public void requerMesValido(int mes) throws MesInvalidoException {
        if(mes < 1 || mes > Constantes.N_MESES)
            throw new MesInvalidoException("O mês '" + mes + "' é inválido!");
    }
    
    /**
     * Verifica se o número de filial passado como parâmetro é válido.
     * @param filial Filial a validar.
     * @throws FilialInvalidaException se este hipermercado não tiver uma filial com o número passado como parâmetro.
     */
    public void requerFilialValida(int filial) throws FilialInvalidaException {
        if(filial < 1 || filial > filiais.quantasFiliais())
            throw new FilialInvalidaException("A filial '" + filial + "' é inválida!");
    }
    
    /**
     * Verifica se o código de produto passado como parâmetro faz parte deste hipermercado.
     * @param codigoProduto Código de produto a considerar.
     * @throws ProdutoInexistenteException se este hipermercado não tiver o produto passado como parâmetro.
     */
    public void requerProdutoExistente(String codigoProduto) throws ProdutoInexistenteException {
        if(!existeProduto(codigoProduto))
            throw new ProdutoInexistenteException("O código de produto '" + codigoProduto + "' não faz parte deste hipermercado.");
    }
    
    /**
     * Verifica se o código de cliente passado como parâmetro faz parte deste hipermercado.
     * @param codigoCliente Código de cliente a considerar.
     * @throws ClienteInexistenteException se este hipermercado não tiver o cliente passado como parâmetro.
     */
    public void requerClienteExistente(String codigoCliente) throws ClienteInexistenteException {
        if(!existeCliente(codigoCliente))
            throw new ClienteInexistenteException("O código de cliente '" + codigoCliente + "' não faz parte deste hipermercado.");
    }
    
    // Query1
    /**
     * Devolve o conjunto de códigos dos produtos que nunca foram comprados no ano.
     * @return Conjunto ordenado alfabeticamente dos códigos dos produtos nunca comprados.
     */
    public Set<String> nuncaComprados(){
        return faturacao.nuncaComprados();
    }
    
    // Query2
    /**
     * Calcula e devolve o total global de vendas realizadas neste hipermercado, no mês passado como parâmetro.
     * @param mes Mês a considerar.
     * @return Total global de vendas registadas no mês <code>mes</code>.
     * @throws MesInvalidoException se o mês passado como parâmetro for inválido.
     */
    public int totalGlobalVendas(int mes) throws MesInvalidoException{
        requerMesValido(mes);
        return faturacao.totalVendasMes(mes);
    }
    
    /**
     * Calcula e devolve o total de clientes distintos que compraram neste hipermercado, no mês passado como parâmetro.
     * @param mes Mês a considerar.
     * @return Total de clientes distintos que compraram no mês <code>mes</code>.
     */
    public int totalClientesCompraram(int mes) throws MesInvalidoException{
        requerMesValido(mes);
        return filiais.quantosClientesCompraramMes(mes);
    }
    
    // Query3
    /**
     * Dado um cliente e um mês, devolve uma lista de triplos (1 por mês, lista indexada de 1 a 12), em que cada 
     * triplo contém o total de compras de um mês, o número de produtos comprados e o total gasto.
     * @param codigoCliente Código do cliente a quem a consulta diz respeito.
     * @return List de TriploComprasProdutosGasto que na posição de índice <code>i</code> tem informação
     *         sobre o total de compras, o número de produtos distintos comprados e o total gasto no mês <code>i</code>.
     * 
     * @throws ClienteInexistenteException se o código de cliente passado como parâmetro não constar neste hipermercado.
     */
    public List<TriploComprasProdutosGasto> infoPorMes(String codigoCliente) throws ClienteInexistenteException {
        requerClienteExistente(codigoCliente);
        
        int[] quantasComprasPorMes = filiais.quantasComprasPorMes(codigoCliente);
        List<TriploComprasProdutosGasto> dadosPorMes = new ArrayList<>(Constantes.N_MESES+1);
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
    
    /**
     * Dado um código de produto e um mês, devolve a faturação desse produto nesse mês.
     * @param codigoProduto Código de produto a quem a consulta diz respeito.
     * @param mes Mês para o qual se pretende obter a faturação do produto identificado por <code>codigoProduto</code>.
     * @return Faturação do produto escolhido, no mês dado.
     * @throws MesInvalidoException se o mês <code>mes</code> for inválido.
     */
    public FatProdMes getFatProdMes(String codigoProduto, int mes) throws MesInvalidoException {
        requerMesValido(mes);
        return faturacao.getFatProdMes(codigoProduto, mes); // getFatProdMes devolve uma cópia do objeto FatProdMes existente na faturação
    }
    
    /**
     * Dado um código de produto e um mês, devolve o total de clientes distintos
     * que compraram o produto passado como parâmetro, no mês selecionado.
     * @param codigoProduto Código de produto a quem a consulta diz respeito.
     * @param mes Mês a considerar na consulta.
     * @return Número de clientes distintos que compraram o produto identificado por <code>codigoProduto</code> no mês <code>mes</code>.
     * @throws MesInvalidoException se o mês passado como parâmetro for inválido.
     */
    public int quantosCompraramProdutoMes(String codigoProduto, int mes) throws MesInvalidoException {
        requerMesValido(mes);
        return filiais.quantosCompraramProdutoMes(codigoProduto, mes);
    }
    
    // Query5
    
    /**
     * Devolve uma {@code List<ParProdQtd> } ordenada por ordem decrescente de quantidade e, para quantidades iguais,
     * por ordem alfabética dos códigos de produto mais comprados pelo cliente passado como parâmetro.
     * @param codigoCliente Código de cliente a considerar na consulta.
     * @return Lista de ParProdQtd ordenada por ordem decrescente de quantidade, com os códigos de produtos comprados 
     *         pelo cliente selecionado e a respetiva quantidade comprada.
     * 
     * @throws ClienteInexistenteException se o cliente passado como parâmetro não estiver registado neste hipermercado.
     */
    public List<ParProdQtd> produtosMaisComprados(String codigoCliente) throws ClienteInexistenteException {
        List<ComprasDoProduto> compras = filiais.comprasFeitasTotal(codigoCliente);
        return filiais.produtosMaisComprados(compras);
    }
    
    // Query6
    
    /**
     * Dado um valor inteiro <code>X</code>, devolve uma {@code List<TriploProdQtdClis> } com os <code>X</code>
     * produtos mais vendidos em todo o ano e a respetiva quantidade vendida e número de clientes distintos que
     * compraram cada um. A lista devolvida está ordenada por ordem decrescente de quantidade e, para quantidades
     * iguais, por ordem alfabética de códigos de produto.
     * @param X Tamanho do <i>top</i> de produtos mais vendidos.
     * @return Lista de TriploProdQtdClis com os <code>X</code> produtos mais vendidos e a respetiva
     *         quantidade vendida e número de clientes distintos que os compraram. Se <code>X</code>
     *         for menor ou igual a 0, é devolvida uma lista vazia.
     */
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
    /**
     * Dada uma filial, devolve um array de comprimento 3 que na posição de índice <code>i</code> tem um
     * ParCliFat com o código e valor gatos pelo iesimo comprador que mais dinheiro gastou na filial escolhida.
     * @param filial Filial a considerar na consulta.
     * @return Array de ParCliFat, ordenado decrescentemente pelo total gasto, com os 3 maiores compradores
     *         da filial <code>filial</code> e a quantia que gastaram.
     * 
     * @throws FilialInvalidaException se a filial passada como parâmetro não fizer parte deste hipermercado.
     */
    public ParCliFat[] tresMaioresCompradores(int filial) throws FilialInvalidaException {
        requerFilialValida(filial);
        return filiais.tresMaioresCompradores(filial);
    }
    
    // Query8
    
    /**
     * Dado um valor inteiro <code>X</code>, devolve uma {@code List<ParCliProdsDif>}, ordenada por ordem
     * decrescente de número de produtos diferentes comprados e, para quantidade iguais, ordenada por
     * ordem albabética dos códigos de cliente.
     * @param X Tamanho do <i>top</i> de produtos diferentes comprados.
     * @return Lista ParCliProdsDif com os códigos e número de produtos diferentes comprados
     *         pelos <code>X</code> clientes que mais produtos diferentes compraram. A lista
     *         devolvida está ordenada por ordem decrescente de número de produtos distintos comprados.
     */
    public List<ParCliProdsDif> clisCompraramMaisProdsDif(int X){
        return filiais.clisCompraramMaisProdsDif(X);
    }
    
    // Query9
    /**
     * Dado um código de produto e um inteiro <code>X</code>, devolve uma {@code List<TriploCliQtdGasto>}
     * com os <code>X</code> clientes que mais compraram o produto passado como parâmetro e a respetiva
     * quantidade comprada e total gasto. A lista devolvida está ordenada decrescentemente pelo número
     * de unidades compradas do produto identificado por <code>codigoProduto</code>.
     * 
     * @param codigoProduto Código de produto a considerar na consulta.
     * @param X Tamanho do <i>top</i> de clientes que mais compraram o produto identificado por <code>codigoProduto</code>.
     * @return Lista de TriploCliQtdGasto em que cada elemento tem o código de um cliente do top <code>X</code> de clientes
     *         que mais compraram o produto passado como parâmetro, bem como a quantidade comprada e o total gasto. A lista
     *         devolvida está ordenada por ordem decrescente de quantidade comprada.
     * 
     * @throws ProdutoInexistenteException se o produto passado como parâmetro não constar neste hipermercado.
     */
    public List<TriploCliQtdGasto> clientesMaisCompraram(String codigoProduto, int X) throws ProdutoInexistenteException {
        requerProdutoExistente(codigoProduto);
        return filiais.clientesMaisCompraram(codigoProduto, X);
    }
}
