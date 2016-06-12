import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;

import static java.lang.System.err;
import static java.lang.System.out;

import java.util.Set;
import java.util.TreeSet;
import java.util.List;
import java.util.Vector;
import java.util.Formatter;

/**
 * Classe principal do projeto <strong>GereVendas</strong>, que permite
 * a criação e execução de instâncias de HipermercadoApp.
 * 
 * <p>Cada instância de HipermercadoApp tem 5 menus:
 * <op>
 *      <li> Menu principal; </li>
 *      <li> Menu de leitura </li>
 *      <li> Menu de estatísticas </li>
 *      <li> Menu de queries interativas </li>
 *      <li> Menu sair </li>
 * </op>
 * O menu principal é apresentado quando uma instância de HipermercadoApp
 * inicia a sua execução. A partir do menu principal, o utilizador pode
 * ler os dados da aplicação a partir de ficheiros de texto ou de uma
 * <code>ObjectStream</code>; entrar no menu de estatística ou no menu
 * de queries interativas, se já tiver lido os dados; guardar o estado
 * da aplicação, caso esta esteja populada e, por fim, sair da aplicação.
 * 
 * <p> O menu de estatísticas dá ao utilizador a possibilidade de escolher
 * entre as estatísticas do último ficheiro de vendas lido e as estatísticas
 * gerais, relativas aos dados atuais da aplicação. Além dessas duas
 * opções, o utilizador pode ainda optar por regressar ao menu principal
 * ou por sair da aplicação.
 * 
 * <p> No menu de queries interativas, o utilizador pode selecionar qualquer uma
 * de 9 queries interativas que lhe permitem obter várias informações
 * relativas às vendas mensais e/ou anuais do hipermercado em questão.
 * Estando no menu de queries interativas, o utilizador pode também optar
 * por regressar ao menu principal, ou sair diretamente da aplicação.
 * 
 * <p> Se a aplicação estiver populada quando o utilizador opta por sair,
 * é-lhe apresentado o "menu sair", em que é dada a possibilidade de guardar
 * o estado da aplicação antes de concluir a execução da mesma.
 */

public class HipermercadoApp {
    /** Guarda os produtos, clientes e vendas e permite obter informações relativas aos mesmos */
    private Hipermercado hipermercado;
    
    /** Menus da aplicação */
    private Menu menuPrincipal, menuSair;
    private Menu menuLeitura, menuEstatisticas, menuQueries;

    private static final String[] opcoesMenuPrincipal = {
            "Ler dados",
            "Consultas estatísticas",
            "Consultas interativas",
            "Gravar estado"
        };
    private static final String[] opcoesMenuLeitura = {"Ficheiros de texto", "Ficheiro de estado"};

    private static final String[] opcoesMenuEstatisticas = {
            "Dados do último ficheiro lido",
            "Dados gerais",
            "Voltar ao menu principal"
        };
    private static final String[] opcoesMenuQueries = {
            "Produtos nunca comprados",
            "Vendas gerais e nº de clientes distintos que compraram num mês",
            "Compras, produtos distintos comprados e total gasto por um cliente, mês a mês",
            "Quantas vezes um produto foi comprado, por quantos clientes distintos e total faturado",
            "Produtos mais comprados por um cliente",
            "Top de produtos mais vendidos em todo o ano",
            "Lista dos 3 maiores compradores, para cada filial",
            "Top de clientes que compraram mais produtos diferentes",
            "Top de clientes que mais compraram um produto e valor gasto",
            "Voltar ao menu principal"
        };
    private static final String[] respostaSimNao = {"Sim", "Não"};
    
    /** 
     * Constrói uma instância de HipermercadoApp com os vários menus predefinidos.
     * Nota: A instância construída não tem à partida quaisquer dados de
     *       produtos, clientes ou vendas.
     */
    public HipermercadoApp() {
        hipermercado = null; // indica que ainda não carregamos os dados do hipermercado
        menuPrincipal = new Menu(" Menu principal", opcoesMenuPrincipal, true);
        menuLeitura = new Menu(" Que tipo de ficheiro pretende ler? ", opcoesMenuLeitura, false); 
        menuEstatisticas = new Menu(" Estatísticas", opcoesMenuEstatisticas, true);
        menuQueries = new Menu(" Queries interativas", opcoesMenuQueries, true);
        menuSair = new Menu(" Deseja guardar o estado da aplicação?", respostaSimNao, false);
    }
    
    /** Splash screen apresentado quando uma instância de ImoobiliariaApp começa a executar. */
    private void splashScreen() {
        String separador = System.getProperty("line.separator");
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
        final int N_ESPACOS = 11;

        try{
            bw.write(" _____               _   _                _" + separador);           
            bw.write("|  __ \\             | | | |              | |" + separador);          
            bw.write("| |  \\/ ___ _ __ ___| | | | ___ _ __   __| | __ _ ___" + separador); 
            bw.write("| | __ / _ | '__/ _ | | | |/ _ | '_ \\ / _` |/ _` / __|" + separador);
            bw.write("| |_\\ |  __| | |  _ \\ \\_/ |  __| | | | (_| | (_| \\__ \\" + separador);
            bw.write(" \\____/\\___|_|  \\___|\\___/ \\___|_| |_|\\__,_|\\__,_|___/" + separador);
            bw.write(new String(new char[N_ESPACOS]).replace("\0", " ")); // centra a mensagem de "prima ENTER para continuar"
            bw.flush();
            enterParaContinuar();
        }
        catch(IOException e){err.println("Erro de I/O: Não foi possível apresentar o ecrã inicial.");}
    }

    /**
     * Limpa o ecrã 
     * (só funciona no BlueJ, dado que nas várias pesquisas que realizámos não conseguimos
     *  encontrar uma forma de limpar o ecrã que fosse independente da consola/IDE utilizado.)
     */
    private void limparEcra() { out.print("\f"); }
    
    /** Apresenta a mensagem "Prima ENTER para continuar", aguarda que o utilizador prima enter e depois disso limpa o ecrã. */
    private void enterParaContinuar() {
        out.print("Prima ENTER para continuar... ");
        Input.lerString();
        limparEcra();
    }
    
    /** 
     * Ponto de entrada do executável.
     * Cria uma HipermercadoApp e envia-lhe a mensagem executa().
     */
    public static void main(String[] args) {
        HipermercadoApp app = new HipermercadoApp();

        app.executa();
    }
    
    /**
     * Apresenta o menu principal, lê uma opção do mesmo e, se esta for válida
     * executa-a, se não apresenta uma mensagem de erro e lê outra opção.
     */
    public void executa() {
        int op;

        splashScreen();
        do{
            menuPrincipal.executa();
            op = menuPrincipal.getOpcao();
            limparEcra();

            if(op > 0){
                switch(op){
                    case 1:
                    lerDados();
                    enterParaContinuar();
                    break;
                    case 2:
                    // a atribuição op = menuEstatisticas() permite-nos sair do ciclo 'do while', 
                    // de executa() quando o utilizador opta por sair do programa em menuEstatisticas().
                    if(appPopulada())
                        op = menuEstatisticas();
                    else
                        err.println("Erro: Primeiro precisa de ler os ficheiros de dados.");
                    break;
                    case 3:
                    if(appPopulada())
                        op = menuQueries();
                    else
                        err.println("Erro: Primeiro precisa de ler os ficheiros de dados.");
                    break;
                    case 4:
                    gravarEstado();
                    break;
                }
            }
        }
        while(op != 0);

        if(appPopulada()){
            menuSair.executa(); // Pergunta ao utilizador se pretende guardar o estado da aplicação, antes de sair.
            if(menuSair.getOpcao() == 1)
                gravarEstado();
        }
    }
    
    /**
     * Método invocado antes de ler/escrever num ficheiro, para ler e devolver o nome do mesmo.
     * O utilizador pode optar por introduzir uma linha em branco e nesse caso o nome de
     * ficheiro devolvido é igual a <code>ficheiroPadrao</code> (1º argumento do método.
     */
    private String obterNomeFicheiro(String ficheiroPadrao, String tipoFicheiro) {
        String nomeFicheiro;

        out.printf("Insira o caminho do ficheiro de %s (ENTER para selecionar o ficheiro padrão): ", tipoFicheiro);
        nomeFicheiro = Input.lerString().trim();

        if(nomeFicheiro.isEmpty())
            nomeFicheiro = ficheiroPadrao;

        return nomeFicheiro;
    }
    
    /**
     * Executa o menu de leitura para que o utilizador indique se pretende ler os dados do hipermercado
     * a partir de ficheiros de texto ou de uma <code>ObjectStream</code> e executa a opção adequada.
     * Se houver uma falha a ler os dados, a instância anterior de Hipermercado é recuperada.
     */
    private void lerDados() {
        int op;
        Hipermercado anterior = hipermercado; // guarda a instância anterior de hipermercado para a recuperarmos se a leitura falhar

        menuLeitura.executa();
        op = menuLeitura.getOpcao();
        try{
            if(op == 1)
                lerFicheirosTexto();
            else
                lerEstado();
        }catch(IOException | ClassNotFoundException e){ hipermercado = anterior; err.println(e.getMessage()); }
    }
    
    /** @return <code>true</true> se esta aplicação estiver populada. */
    public boolean appPopulada(){
        return hipermercado != null;
    }
    
    /**
     * Método responsável por perguntar ao utilizador o nome dos ficheiros de produtos, 
     * clientes e vendas e invocar os métodos adqueados para ler cada um desses ficheiros.
     */
    private void lerFicheirosTexto() throws IOException{
        int nprodutos, nclientes;
        String fichProdutos, fichClientes, fichVendas;
        String caminhoData = "data" + File.separator;
        Hipermercado anterior = hipermercado;

        hipermercado = new Hipermercado();
        fichProdutos = obterNomeFicheiro(caminhoData + "Produtos.txt", "produtos");
        nprodutos = leFicheiroProdutos(fichProdutos);

        fichClientes = obterNomeFicheiro(caminhoData + "Clientes.txt", "clientes");
        nclientes = leFicheiroClientes(fichClientes);

        fichVendas = obterNomeFicheiro(caminhoData + "Vendas_1M.txt", "vendas");
        leFicheiroVendas(fichVendas);
    }
    
    /**
     * Pergunta ao utilizador o nome do ficheiro de estado e, se o mesmo for válido,
     * lê o ficheiro de estado e popula a variável hipermercado, desta HipermercadoApp.
     */
    private void lerEstado() throws IOException, ClassNotFoundException{
        String fich = obterNomeFicheiro("data" + File.separator + "hipermercado.dat", "estado");
        
        Crono.start();
        hipermercado = Hipermercado.leObj(fich);
        Crono.stop();
        out.println("Ficheiro '" + fich + "' lido em " + Crono.print() + "segundos");
    }
    
    /** 
     * Imprime dados relativos à leitura de um dos ficheiros de texto, como o nome 
     * do ficheiro lido, o número de linhas válidas e o tempo gasto a ler o ficheiro.
     */
    private void imprimeDadosLeitura(String fich, int nLinhasValidas){
        out.println("--------------------------------");
        out.println("Ficheiro lido: " + fich);
        out.println("Número de linhas válidas: " + nLinhasValidas);
        out.println("Tempo: " + Crono.print());
    }
    
    /** Lê o ficheiro de produtos cujo nome foi passado como parâmetro. */
    private int leFicheiroProdutos(String fich) throws IOException {
        int nprodutos = 0;
        String produto = null;
        BufferedReader inStream = null;

        Crono.start();
        inStream = new BufferedReader(new FileReader(fich));
        while((produto = inStream.readLine()) != null){
            hipermercado.registaProduto(produto);
            ++nprodutos;
        }
        Crono.stop();
        imprimeDadosLeitura(fich, nprodutos); 

        return nprodutos;
    }
    
    /** Lê o ficheiro de clientes cujo nome foi passado como parâmetro. */
    private int leFicheiroClientes(String fich) throws IOException{
        int nclientes = 0;
        String cliente = null;
        BufferedReader inStream = null;

        Crono.start();
        inStream = new BufferedReader(new FileReader(fich));
        while((cliente = inStream.readLine()) != null){
            hipermercado.registaCliente(cliente);
            ++nclientes;
        }
        Crono.stop();
        imprimeDadosLeitura(fich, nclientes);

        return nclientes;
    }
    
    /**
     * Lê o ficheiro de vendas, validando cada uma das linhas lidas de forma a evitar
     * o registo de vendas inválidas. Este método reúne as várias informações necessárias
     * para criar as estatísticas do ficheiro de vendas e, por esse motivo, invoca o método
     * criaEstatisticasFicheiro() sobre a instância de hipermercado desta HipermercadoApp.
     */
    private int leFicheiroVendas(String fich) throws IOException{
        Venda v = null;
        String linha = null;
        double faturacaoTotal;
        int nlinhas, nvalidas, ngratis;
        BufferedReader inStream = null;
        Set<String> produtosVendidos, clientesCompraram;

        Crono.start();
        inStream = new BufferedReader(new FileReader(fich));
        produtosVendidos = new TreeSet<>();
        clientesCompraram = new TreeSet<>();
        faturacaoTotal = 0.0;
        nlinhas = nvalidas = ngratis = 0;

        while((linha = inStream.readLine()) != null){
            v = Venda.parseLinhaVenda(linha);
            ++nlinhas;
            
            if(vendaValida(v)){
                ++nvalidas;
                hipermercado.registaVenda(v);
                produtosVendidos.add(v.getCodigoProduto());
                clientesCompraram.add(v.getCodigoCliente());

                if(v.getPrecoUnitario() == 0.0)
                    ++ngratis;
                else
                    faturacaoTotal += v.getUnidadesVendidas() * v.getPrecoUnitario();
            }
        }
        hipermercado.criaEstatisticasFicheiro(fich, nlinhas - nvalidas, produtosVendidos.size(), clientesCompraram.size(), ngratis, faturacaoTotal);
        Crono.stop();
        imprimeDadosLeitura(fich, nvalidas);

        return nvalidas;
    }
    
    /** 
     * Verifica se todos os dados de uma venda são válidos, tendo em conta os clientes e produtos registados no Hipermercado.
     * @param v Venda cuja validade queremos testar.
     * @return <code>true</true> se todos os dados da venda são válidos.
     */
    private boolean vendaValida(Venda v){
        int filial = v.getFilial();
        return filial >= 1 && filial <= hipermercado.quantasFiliais() && v.eValida() &&
               hipermercado.existeProduto(v.getCodigoProduto()) && hipermercado.existeCliente(v.getCodigoCliente());
    }
    
    /** Executa o menu de estatísticas */
    private int menuEstatisticas() {
        int op;

        do {
            menuEstatisticas.executa();
            op = menuEstatisticas.getOpcao();
            limparEcra();
            if(op > 0){
                switch(op){
                    case 1:
                    imprimeEstatisticasFicheiro();
                    enterParaContinuar();
                    break;
                    case 2:
                    imprimeEstatisticasGerais();
                    enterParaContinuar();
                    break;
                    case 3:
                    break;
                }
            }
        }
        while(op != 0 && op != 3);

        return op;
    }

    private int menuQueries() {
        int op;

        do {
            menuQueries.executa();
            op = menuQueries.getOpcao();
            limparEcra();
            if(op > 0){
                switch(op){
                    case 1:
                    query1();
                    break;
                    case 2:
                    query2();
                    break;
                    case 3:
                    query3();
                    break;
                    case 4:
                    query4();
                    break;
                    case 5:
                    query5();
                    break;
                    case 6:
                    query6();
                    break;
                    case 7:
                    query7();
                    break;
                    case 8:
                    query8();
                    break;
                    case  9:
                    query9();
                    break;
                    case 10:
                    break;
                }
            }
        }
        while(op != 0 && op != 10);

        return op;
    }
    
    /** Imprime as estatísticas do último ficheiro de vendas lido. */
    private void imprimeEstatisticasFicheiro(){
        EstatisticasFicheiro est = hipermercado.getEstatisticasFicheiro();

        out.println(est.toString());
    }
    
    /** Imprime estatísticas gerais relativas aos dados registados nas estruturas. */
    private void imprimeEstatisticasGerais(){
        EstatisticasGerais est = hipermercado.getEstatisticasGerais();

        out.println(est.toString());
    }
    
    /** Imprime as opções do menu de navegação em LStrings. */
    private void imprimeOpcoesNavega(int pag, int totalPags){
        final String separador = System.getProperty("line.separator");
        final String opcoesNavega = "[1] Pag. ant.  [2] Pag. seg.   [3] Selec. pag.   [4] Prim. pag.   [5] Ult. pag  [6] Info.  [0] Sair";

        out.println(separador + opcoesNavega);
        out.printf("(%d/%d): ", pag, totalPags);
    }
    
    /** Navega a LStrings passada como parâmetro, sem especificar um título. */
    private void navega(LStrings lStr){
        navega(lStr, null);
    }
    
    /**
     * Navega a LStrings passada como parâmetro, apresentado o título
     * passado como 2º argumento, antes de cada página da LStrings
     * passada como 1º argumento. O utilizador tem as opções:
     * 
     * 1. Ir para a página anterior;
     * 2. Ir para a página seguinte;
     * 3. Selecionar uma página;
     * 4. Ir para a primeira página;
     * 5. Ir para a última página;
     * 6. Apresentar informação sobre a LStrings que está a ser navegada;
     * 0. Sair;
     */
    private void navega(LStrings lStr, String header){
        if(lStr.estaVazia()){
            out.println("Lista vazia");
            enterParaContinuar();
            return;
        }

        int op, totalPags;
        List<String> pagina;
        String info = lStr.getInfo();

        out.println(info);
        enterParaContinuar();

        header = (header == null) ? ""  : (header + System.getProperty("line.separator"));
        totalPags = lStr.getTotalPags();
        do{
            System.out.print(header);
            pagina = lStr.getPagina();
            pagina.forEach(System.out :: println);
            imprimeOpcoesNavega(lStr.getNumPag(), totalPags);
            op = Input.lerInt();
            switch(op){
                case 0: // Sair
                break;
                case 1: // Página anterior
                lStr.pagAnt();
                break;
                case 2: // Próxima página
                lStr.proxPag();
                break;
                case 3: // Ir para página
                out.print("Para que página pretende ir? ");
                int numPag = Input.lerInt();
                if(!lStr.irParaPag(numPag)){
                    out.println("A página '" + numPag + "' não existe!");
                    enterParaContinuar();
                }
                break;
                case 4: // Primeira página
                lStr.primPag();
                break;
                case 5: // Última página
                lStr.ultimaPag();
                break;
                case 6: // Informação sobre a LStrings
                out.println(info);
                enterParaContinuar();
                break;
                default:
                out.println("Opção inválida!");
                enterParaContinuar();
            }
            limparEcra();
        }
        while(op != 0);
    }
    
    /**
     * Apresenta uma lista de Strings navegável ordenada alfabeticamente, com
     * os códigos dos produtos nunca comprados. Se todos os produtos tiverem
     * sido comprados, em vez disso é apresentada a mensagem "Lista vazia".
     * (o total de produtos nunca comprados pode ser obtido 
     *  através da opção 'info' do menu de navegação na LStrings)
     */
    private void query1() {
        Crono.start();
        LStrings nuncaComprados = new LStrings(hipermercado.nuncaComprados());
        Crono.stop();
        imprimeTempoQuery();
        navega(nuncaComprados);
    }
    
    /**
     * Lê um mês do stdin e se este for válido, apresenta no stdout o total global 
     * de vendas realizadas nesse mês e o número total de clientes distintos que as fizeram.
     */
    private void query2() {
        int mes;
        int totalVendas, totalClientes;
        String separador = "-------------------------------------------------------";

        out.print("Para que mês pretende obter os resultados? ");
        mes = Input.lerInt();
        try{
            Crono.start();
            totalVendas = hipermercado.totalGlobalVendas(mes);
            totalClientes = hipermercado.totalClientesCompraram(mes);
            out.println(separador);
            out.println("-> Mês: " + mes);
            out.println("-> Total global de vendas: " + totalVendas);
            out.println("-> Número de clientes distintos que compraram: " + totalClientes);
            out.println(separador);
            Crono.stop();
            imprimeTempoQuery();
        }catch(MesInvalidoException e){ err.println(e.getMessage()); }
    }
    
    /**
     * Lê um código de cliente do stdin e, se este existir no hipermercado
     * desta instância de HipermercadoApp, apresenta no stdout o número de
     * compras que o cliente fez, o número de produtos distintos que comprou
     * e o total gasto, para cada um dos meses do ano.
     */
    private void query3() {
        String codigoCliente;
        List<TriploComprasProdutosGasto> dadosCliente;

        out.print("Que cliente pretende consultar? ");
        codigoCliente = Input.lerString();
        
        try{
            Crono.start();
            dadosCliente = hipermercado.infoPorMes(codigoCliente);
    
            out.println("Mes | Total de Compras | Produtos distintos comprados | Total gasto");
            for(int mes = 1; mes <= Constantes.N_MESES; mes++){
                TriploComprasProdutosGasto dadosDoMes = dadosCliente.get(mes);
                out.printf("%3d | %16d | %28d | %11.2f%n",
                            mes, dadosDoMes.getTotalCompras(), dadosDoMes.getProdutosDistintos(), dadosDoMes.getTotalGasto());
            }
            Crono.stop();
            imprimeTempoQuery();
        }catch(ClienteInexistenteException e){ err.println(e.getMessage()); }
    }
    
    /**
     * Lê um código de produto do stdin e se este existir no hipermercado desta HipermercadoApp,
     * apresenta, para cada mês, o número de vezes que esse produto foi comprado, o número de clientes
     * distintos que o compraram e o total faturado.
     */
    private void query4() {
        String codigoProduto;

        out.print("Código de produto: ");
        codigoProduto = Input.lerString();
        if(!hipermercado.existeProduto(codigoProduto)){
            out.println("O produto que pretende consultar não foi registado");
            return;
        }
        Crono.start();
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
        Formatter formatador = new Formatter(bw);

        formatador.format("Mês | Quantidade comprada | Nº de clientes distintos que compraram | Faturação%n");
        try{
            for(int mes = 1; mes <= Constantes.N_MESES; ++mes){
                FatProdMes fProdMes = hipermercado.getFatProdMes(codigoProduto, mes);
                int quantosClisCompraram = hipermercado.quantosCompraramProdutoMes(codigoProduto, mes);

                formatador.format("%3d | %19d | %38d | %.2f%n", mes, fProdMes.totalUnidsVendidas(), quantosClisCompraram, fProdMes.totalFaturado());
            }
            formatador.flush();
            Crono.stop();
            imprimeTempoQuery();
        }catch(MesInvalidoException e) { err.println(e.getMessage()); }
    }
    
    /**
     * Lê um código de cliente e, se este for válido, apresenta a lista de códigos
     * de produtos que esse cliente mais comprou e a quantidade comprada. A lista
     * apresentada encontra-se ordenada por ordem decrescente de quantidade comprada
     * e, para quantidades iguais, por ordem alfabética dos códigos de produto.
     */
    private void query5() {
        out.print("Que cliente pretende consultar? ");
        String codigoCliente = Input.lerString();
        
        try{
            Crono.start();
            List<ParProdQtd> prodsMaisComprados = hipermercado.produtosMaisComprados(codigoCliente);
            List<String> resultados = new Vector<>(prodsMaisComprados.size());
            String header = "Produto | Quantidade Comprada";
    
            for(ParProdQtd par : prodsMaisComprados){
                resultados.add(String.format("%7s | %19d", par.getProd(), par.getQtd()));
            }
            LStrings l = new LStrings(resultados);
            Crono.stop();
            imprimeTempoQuery();
            navega(l, header);
        }catch(ClienteInexistenteException e){ err.println(e.getMessage()); }
    }
    
    /**
     * Lê um inteiro X do stdin e:
     *  - Se X for positivo, navega numa LStrings com os códigos dos X produtos mais vendidos
     *    no ano, o número total de clientes distintos que compraram cada produto e a quantidade
     *    vendida. A lista apresentada está ordenada por ordem decrescente de quantidade comprada
     *    e, para quantidades iguais, por ordem alfabética dos códigos de produto.
     *  - Caso contrário apresenta a mensagem "Lista vazia";
     */
    private void query6() {
        int X;
        List<TriploProdQtdClis> topX;

        out.print("Quantos elementos quer no top de produtos mais vendidos? ");
        X = Input.lerInt();

        Crono.start();
        topX = hipermercado.maisVendidos(X);
        if(topX.isEmpty())
            out.println("Lista vazia");
        else{
            List<String> resultados = new Vector<>(topX.size());
            final String header = "Produto | Quantidade comprada | Nº de clientes que compraram";

            for(TriploProdQtdClis triplo : topX)
                resultados.add(String.format("%7s | %19d | %28d", triplo.getProd(), triplo.getQtd(), triplo.getClis()));

            LStrings l = new LStrings(resultados);
            Crono.stop();
            imprimeTempoQuery();
            navega(l, header);
        }
    }
    
    /**
     * Apresenta no stdout a lista dos 3 maiores compradores de cada filial.
     */
    private void query7() {
        int rank, filial;
        ParCliFat[][] res = new ParCliFat[Constantes.N_FILIAIS][];

        Crono.start();
        try{
            for(filial = 1; filial <= Constantes.N_FILIAIS; ++filial)
                res[filial-1] = hipermercado.tresMaioresCompradores(filial);
        }
        catch(FilialInvalidaException e){
            err.println(e.getMessage());
            return;
        }

        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
        Formatter formatador = new Formatter(bw);

        formatador.format("%9s", "");
        for(rank = 1; rank <= 3; ++rank) // Imprime linha tabela com as posicoes do ranking
            formatador.format("| %18dº%19s |", rank, "");

        formatador.format("%n");
        for(filial = 1; filial <= Constantes.N_FILIAIS; ++filial){ // Imprime resultados, filial a filial
            formatador.format("Filial %d ", filial);
            for(rank = 1; rank <= 3; ++rank){ 
                ParCliFat par = res[filial-1][rank-1];
                formatador.format("| Cliente: %s, Faturado: %12.2f |", par.getCli(), par.getFat());
            }
            formatador.format("%n");
        }
        formatador.flush();
        Crono.stop();
        imprimeTempoQuery();
    }
    
    /**
     * Lê um inteiro X do stdin e apresenta os códigos dos X clientes que compraram mais
     * produtos diferentes e indica a quantidade comprada de cada um desses produtos.
     * Os códigos de cliente são apresentados por ordem decrescente do número de produtos
     * distintos comprados e, para valores iguais, por ordem alfabética de códigos de cliente.
     */
    private void query8() {
        int X;

        out.print("Número de clientes: ");
        X = Input.lerInt();

        Crono.start();
        List<ParCliProdsDif> topX = hipermercado.clisCompraramMaisProdsDif(X);
        List<String> resultados = new Vector<>(topX.size());
        final String header = "Cliente | Nº de produtos diferentes comprados";

        for(ParCliProdsDif par : topX)
            resultados.add(String.format("%7s | %35d", par.getCli(), par.getProdsDif()));

        LStrings l = new LStrings(resultados);
        Crono.stop();
        imprimeTempoQuery();
        navega(l, header);
    }
    
    /**
     * Lê um código de produto e um inteiro X do stdin e se o código de produto
     * lido for válido, apresenta o conjunto dos X clientes que mais compraram
     * esse produto, a quantidade comprada por cada um e o valor gasto. Os
     * resultados são apresentados por ordem decrescente de quantidade comprada
     * e, para quantidades iguais, por ordem alfabética de códigos de cliente.
     */
    private void query9() {
        int X;
        String codigoProduto;

        out.print("Código de produto a considerar: ");
        codigoProduto = Input.lerString();
        
        out.print("Número de clientes: ");
        X = Input.lerInt();
        try{
            Crono.start();
            List<TriploCliQtdGasto> topX = hipermercado.clientesMaisCompraram(codigoProduto, X);
            List<String> resultados = new Vector<>(topX.size());
            final String header = "Cliente | Quantidade comprada | Valor total gasto ";
    
            for(TriploCliQtdGasto triplo : topX)
                resultados.add(String.format("%7s | %19d | %17.2f", triplo.getCli(), triplo.getQtd(), triplo.getGasto()));
    
            LStrings l = new LStrings(resultados);
            Crono.stop();
            imprimeTempoQuery();
            navega(l, header);
        }catch(ProdutoInexistenteException e){ err.println(e.getMessage()); }
    }
    
    /**
     * Imprime no stdout o tempo (em segundos) que a última query levou a executar.
     */
    private void imprimeTempoQuery(){
        StringBuilder sb = new StringBuilder();
        sb.append("Tempo gasto na query: ").append(Crono.print()).append("s");
        out.println(sb.toString());
        enterParaContinuar();
    }
    
    /**
     * Se a aplicação estiver populada, lê o nome do ficheiro onde o utilizador pretende
     * gravar o estado atual da mesma e guarda o estado da aplicação nesse ficheiro. 
     * Nota: O ficheiro de gravação por omissão é hipermercado.dat
     */
    private void gravarEstado(){
        if(appPopulada()){
            String fichEstado = obterNomeFicheiro("data" + File.separator + "hipermercado.dat", "estado");

            try{
                hipermercado.gravaObj(fichEstado);
                out.println("-> Estado gravado com sucesso!");
            }catch(IOException e){ err.println(e.getMessage()); }
        }
        else
            err.println("Não existem dados para guardar. Primeiro deverá ler os dados.");
    }
}
