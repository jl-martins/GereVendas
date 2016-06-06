import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.io.FileReader;
import java.io.IOException;

import static java.lang.System.err;
import static java.lang.System.out;

import java.util.Set;
import java.util.TreeSet;

public class HipermercadoApp {
    private Hipermercado hipermercado;
    private Menu menuPrincipal, menuSair;
    private Menu menuEstatisticas, menuQueries;

    private static final String[] opcoesMenuPrincipal = {
        "Ler dados",
        "Consultas estatísticas",
        "Consultas interativas",
        "Gravar estado"
    };
    private static final String[] opcoesMenuEstatisticas = {
        "Dados do último ficheiro lido",
        "Dados gerais",
        "Voltar ao menu principal"
    };
    private static final String[] opcoesMenuQueries = {
        "Códigos dos produtos nunca comprados e total",
        "Vendas gerais e nº de clientes distintos que compraram num mês",
        "Compras, produtos distintos comprados e total gasto por um cliente, mês a mês",
        "Quantas vezes um produto foi comprado, por quantos clientes distintos e total faturado",
        "Lista de códigos de produto mais comprados por um cliente",
        "X produtos mais vendidos em todo o ano",
        "Lista dos 3 maiores compradores, para cada filial",
        "X clientes que compraram mais produtos diferentes",
        "Conjunto dos X clientes que mais compraram um produto e valor gasto",
        "Voltar ao menu principal"
    };
    private static final String[] respostaSimNao = {"Sim", "Não"};

    public HipermercadoApp() {
        hipermercado = null; // indica que ainda não carregamos os dados do hipermercado
        menuPrincipal = new Menu("Menu principal", opcoesMenuPrincipal, true);
        menuEstatisticas = new Menu("Estatísticas", opcoesMenuEstatisticas, true);
        menuQueries = new Menu("Queries interativas", opcoesMenuQueries, true);
        menuSair = new Menu("Deseja guardar o estado da aplicação?", respostaSimNao, false);
    }

    private void splashScreen() {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
        final int N_ESPACOS = 43;

        try{
            bw.write(" _____               _   _                _");           
            bw.write("|  __ \\             | | | |              | |");          
            bw.write("| |  \\/ ___ _ __ ___| | | | ___ _ __   __| | __ _ ___"); 
            bw.write("| | __ / _ | '__/ _ | | | |/ _ | '_ \\ / _` |/ _` / __|");
            bw.write("| |_\\ |  __| | |  _ \\ \\_/ |  __| | | | (_| | (_| \\__ \\");
            bw.write(" \\____/\\___|_|  \\___|\\___/ \\___|_| |_|\\__,_|\\__,_|___/");
            bw.write(new String(new char[N_ESPACOS]).replace("\0", " ")); // centra a mensagem de "prima ENTER para continuar"
            bw.flush();
            enterParaContinuar();
        }
        catch(IOException e){err.println("Erro de I/O: Não foi possível apresentar o ecrã inicial.");}
    }

    private void limparEcra() { out.print("\f"); }

    private void enterParaContinuar() {
        out.print("Prima ENTER para continuar... ");
        Input.lerString();
        limparEcra();
    }

    public static void main(String[] args) {
        HipermercadoApp app = new HipermercadoApp();

        app.executa();
    }

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
                        if(appPopulada())
                            // esta atribuicao permite-nos sair do ciclo 'do while', quando o
                            // utilizador opta por sair do programa em menuEstatisticas().
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
                }
            }
        }
        while(op != 0);

        if(appPopulada()){
            menuSair.executa();
            if(menuSair.getOpcao() == 1)
                gravarEstado();
        }
    }

    private boolean appPopulada() { return hipermercado != null; }

    private String obterNomeFicheiro(String ficheiroPadrao, String tipoFicheiro) {
        String nomeFicheiro;

        out.printf("Insira o caminho do ficheiro de %s (ENTER para abrir ficheiro padrão): ", tipoFicheiro);
        nomeFicheiro = Input.lerString().trim();

        if(nomeFicheiro.isEmpty())
            nomeFicheiro = ficheiroPadrao;

        return nomeFicheiro;
    }
    
    private int leFicheiroProdutos(String fich) {
        int nprodutos = 0;
        String produto = null;
        BufferedReader inStream = null;
        
        try{
            inStream = new BufferedReader(new FileReader(fich));
            while((produto = inStream.readLine()) != null){
                hipermercado.registaProduto(produto);
                ++nprodutos;
            }
        }
        catch(IOException e) { err.println(e.getMessage()); nprodutos = -1; }
        
        return nprodutos;
    }
    
    private int leFicheiroClientes(String fich) {
        int nclientes = 0;
        String cliente = null;
        BufferedReader inStream = null;
        
        try{
            inStream = new BufferedReader(new FileReader(fich));
            while((cliente = inStream.readLine()) != null){
                hipermercado.registaCliente(cliente);
                ++nclientes;
            }
        }
        catch(IOException e) { err.println(e.getMessage()); nclientes = -1; }
        
        return nclientes;
    }
    
    private int leFicheiroVendas(String fich){
        Venda v = null;
        String linha = null;
        double faturacaoTotal;
        int nlinhas, nvalidas, ngratis;
        BufferedReader inStream = null;
        Set<String> produtosVendidos, clientesCompraram;
        
        try{
            inStream = new BufferedReader(new FileReader(fich));
            produtosVendidos = new TreeSet<>();
            clientesCompraram = new TreeSet<>();
            faturacaoTotal = 0.0;
            nlinhas = nvalidas = ngratis = 0;
            
            while((linha = inStream.readLine()) != null){
                v = Venda.parseLinhaVenda(linha);
                ++nlinhas;
                if(v.eValida() && hipermercado.existeProduto(v.getCodigoProduto()) && hipermercado.existeCliente(v.getCodigoCliente())){
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
            hipermercado.criaEstatisticasFicheiro(
                fich,
                nlinhas - nvalidas, // numero de vendas invalidas
                produtosVendidos.size(),
                clientesCompraram.size(),
                ngratis,
                faturacaoTotal
            ); 
        }
        catch(IOException e) { err.println(e.getMessage()); nvalidas = -1; }
        
        return nvalidas;
    }
            
    
    private void lerDados() {
        int nprodutos, nclientes;
        String fichProdutos, fichClientes, fichVendas;
        String caminhoData = "data" + File.separator;
        // Falta verificar se os metodos de leitura devolveram -1 (ver se em vez disso devem atirar excepcoes)
        fichProdutos = obterNomeFicheiro(caminhoData + "Produtos.txt", "produtos");
        nprodutos = leFicheiroProdutos(fichProdutos);
            
        fichClientes = obterNomeFicheiro(caminhoData + "Clientes.txt", "clientes");
        nclientes = leFicheiroClientes(fichClientes);
            
        fichVendas = obterNomeFicheiro(caminhoData + "Vendas_1M.txt", "vendas");
        leFicheiroVendas(fichVendas);
    }

    private int menuEstatisticas() {
        int op;

        do {
            menuEstatisticas.executa();
            op = menuEstatisticas.getOpcao();
            limparEcra();
            if(op > 0){
                switch(op){
                    case 1:
                        out.println(hipermercado.stringEstatisticasFicheiro());
                        enterParaContinuar();
                        break;
                    case 2:
                        out.println(hipermercado.stringEstatisticasGerais());
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

    private void query1() {
        out.println("Por implementar...");
    }

    private void query2() {
        out.println("Por implementar...");
    }

    private void query3() {
        out.println("Por implementar...");
    }

    private void query4() {
        out.println("Por implementar...");
    }

    private void query5() {
        out.println("Por implementar...");
    }

    private void query6() {
        out.println("Por implementar...");
    }

    private void query7() {
        out.println("Por implementar...");
    }

    private void query8() {
        out.println("Por implementar...");
    }

    private void query9() {
        out.println("Por implementar...");
    }
    
    private void gravarEstado(){
        String fichEstado = obterNomeFicheiro("hipermercado.dat", "estado");
        
        try{
            hipermercado.gravaObj(fichEstado);
            out.println("-> Estado gravado com sucesso!");
        }catch(IOException e){ err.println("Não foi possível gravar o estado!"); }
    }
}
