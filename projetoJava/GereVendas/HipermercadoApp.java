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
import java.util.ArrayList;
import java.util.Formatter;

public class HipermercadoApp {
    private Hipermercado hipermercado;
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
        menuPrincipal = new Menu(" Menu principal", opcoesMenuPrincipal, true);
        menuLeitura = new Menu(" Que tipo de ficheiro pretende ler? ", opcoesMenuLeitura, false); 
        menuEstatisticas = new Menu(" Estatísticas", opcoesMenuEstatisticas, true);
        menuQueries = new Menu(" Queries interativas", opcoesMenuQueries, true);
        menuSair = new Menu(" Deseja guardar o estado da aplicação?", respostaSimNao, false);
    }

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
                        // a atribuicao op = menuEstatisticas() permite-nos sair do ciclo 'do while', 
                        // quando o utilizador opta por sair do programa em menuEstatisticas().
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
            menuSair.executa();
            if(menuSair.getOpcao() == 1)
                gravarEstado();
        }
    }

    private String obterNomeFicheiro(String ficheiroPadrao, String tipoFicheiro) {
        String nomeFicheiro;

        out.printf("Insira o caminho do ficheiro de %s (ENTER para abrir ficheiro padrão): ", tipoFicheiro);
        nomeFicheiro = Input.lerString().trim();

        if(nomeFicheiro.isEmpty())
            nomeFicheiro = ficheiroPadrao;

        return nomeFicheiro;
    }
    
    private void lerDados() {
        int op;
        
        menuLeitura.executa();
        op = menuLeitura.getOpcao();
        if(op == 1)
            lerFicheirosTexto();
        else
            lerEstado();
    }
    
    public boolean appPopulada(){
        return hipermercado != null;
    }
    
    private void lerFicheirosTexto(){
        int nprodutos, nclientes;
        String fichProdutos, fichClientes, fichVendas;
        String caminhoData = "data" + File.separator;
        
        hipermercado = new Hipermercado();
        fichProdutos = obterNomeFicheiro(caminhoData + "Produtos.txt", "produtos");
        nprodutos = leFicheiroProdutos(fichProdutos);
            
        fichClientes = obterNomeFicheiro(caminhoData + "Clientes.txt", "clientes");
        nclientes = leFicheiroClientes(fichClientes);
            
        fichVendas = obterNomeFicheiro(caminhoData + "Vendas_1M.txt", "vendas");
        leFicheiroVendas(fichVendas);
    }
    
    private void lerEstado(){
        String fich = obterNomeFicheiro("data" + File.separator + "hipermercado.dat", "estado");
        
        try{
            Crono.start();
            hipermercado = Hipermercado.leObj(fich);
            Crono.stop();
            out.println("Ficheiro '" + fich + "' lido em " + Crono.print() + "segundos");
        }
        catch(IOException | ClassNotFoundException e){ err.println(e.getMessage()); }
    }
    
    private void imprimeDadosLeitura(String fich, int nLinhasValidas){
        out.println("--------------------------------");
        out.println("Ficheiro lido: " + fich);
        out.println("Número de linhas válidas: " + nLinhasValidas);
        out.println("Tempo: " + Crono.print());
    }
    
    private int leFicheiroProdutos(String fich) {
        int nprodutos = 0;
        String produto = null;
        BufferedReader inStream = null;
        
        try{
            Crono.start();
            inStream = new BufferedReader(new FileReader(fich));
            while((produto = inStream.readLine()) != null){
                hipermercado.registaProduto(produto);
                ++nprodutos;
            }
            Crono.stop();
            imprimeDadosLeitura(fich, nprodutos); 
        }
        catch(IOException e) { err.println(e.getMessage()); nprodutos = -1; }
        
        return nprodutos;
    }
    
    private int leFicheiroClientes(String fich) {
        int nclientes = 0;
        String cliente = null;
        BufferedReader inStream = null;
        
        try{
            Crono.start();
            inStream = new BufferedReader(new FileReader(fich));
            while((cliente = inStream.readLine()) != null){
                hipermercado.registaCliente(cliente);
                ++nclientes;
            }
            Crono.stop();
            imprimeDadosLeitura(fich, nclientes);
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
            Crono.start();
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
            hipermercado.criaEstatisticasFicheiro(fich, nlinhas - nvalidas, produtosVendidos.size(), clientesCompraram.size(), ngratis, faturacaoTotal);
            Crono.stop();
            imprimeDadosLeitura(fich, nvalidas);
        }
        catch(IOException e) { err.println(e.getMessage()); nvalidas = -1; }
        
        return nvalidas;
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
    
     private void imprimeEstatisticasFicheiro(){
        EstatisticasFicheiro est = hipermercado.getEstatisticasFicheiro();
        
        out.println(est.toString());
    }
    
    private void imprimeEstatisticasGerais(){
        EstatisticasGerais est = hipermercado.getEstatisticasGerais();
        
        out.println(est.toString());
    }
    
    private void imprimeOpcoesNavega(int pag, int totalPags){
        final String separador = System.getProperty("line.separator");
        final String opcoesNavega = "[1] Pag. ant.  [2] Pag. seg.   [3] Selec. pag.   [4] Prim. pag.   [5] Ult. pag  [6] Info.  [0] Sair";
        
        out.println(separador + opcoesNavega);
        out.printf("(%d/%d): ", pag, totalPags);
    }
    
    private void navega(LStrings lStr){
        navega(lStr, null);
    }
    
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
                case 1: // Pagina anterior
                    lStr.pagAnt();
                    break;
                case 2: // Proxima pagina
                    lStr.proxPag();
                    break;
                case 3: // Ir para pagina
                    out.print("Para que página pretende ir? ");
                    int numPag = Input.lerInt();
                    if(!lStr.irParaPag(numPag)){
                        out.println("A página '" + numPag + "' não existe!");
                        enterParaContinuar();
                    }
                    break;
                case 4: // Primeira pagina
                    lStr.primPag();
                    break;
                case 5: // Ultima pagina
                    lStr.ultimaPag();
                    break;
                case 6: // Informacao sobre a LStrings
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
                
    private void query1() {
        Crono.start();
        LStrings nuncaComprados = new LStrings(hipermercado.nuncaComprados());
        Crono.stop();
        imprimeTempoQuery();
        navega(nuncaComprados);
    }

    private void query2() {
        int mes;
        int totalVendas, totalClientes;
        
        out.print("Para que mês pretende obter os resultados? ");
        mes = Input.lerInt();
        try{
            Crono.start();
            totalVendas = hipermercado.totalGlobalVendas(mes);
            totalClientes = hipermercado.totalClientesCompraram(mes);
            out.println("-> Mês: " + mes + 
                        "; Total global de vendas: " + totalVendas +
                        "; Número de clientes distintos que compraram: " + totalClientes
                        );
            Crono.stop();
            imprimeTempoQuery();
        }catch(MesInvalidoException e){ err.println(e.getMessage()); }
    }

    private void query3() {
        String codigoCliente;
        List<TriploIntIntDouble> dadosCliente;
        
        out.print("Que cliente pretende consultar? ");
        codigoCliente = Input.lerString();
        
        if(!hipermercado.existeCliente(codigoCliente)){
             out.println("O Cliente que pretende consultar não foi registado");
             return;
        }
        
        Crono.start();
        dadosCliente = hipermercado.infoPorMes(codigoCliente);
        
        for(int i = 1; i < 13; i++){
            TriploIntIntDouble dadosDoMes = dadosCliente.get(i);
            out.println("Mes: " + i + ", Total de compras: " + dadosDoMes.getInt1() + ", Produtos Distintos comprados: " + dadosDoMes.getInt2() + ", Total Gasto: " + dadosDoMes.getDouble());
        }
        Crono.stop();
        imprimeTempoQuery();
    }

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
    
    private void query5() {
        out.print("Que cliente pretende consultar? ");
        String codigoCliente = Input.lerString();
        
        if(!hipermercado.existeCliente(codigoCliente)){
             out.println("O Cliente que pretende consultar não foi registado");
             return;
        }
        
        Crono.start();
        List<ParProdQtd> prodsMaisComprados = hipermercado.produtosMaisComprados(codigoCliente);
        List<String> resultados = new ArrayList<>(prodsMaisComprados.size());
        String header = "Produto | Quantidade Comprada";
        
        for(ParProdQtd par : prodsMaisComprados){
            resultados.add(String.format("%7s | %19d", par.getProd(), par.getQtd()));
        }
        LStrings l = new LStrings(resultados);
        Crono.stop();
        imprimeTempoQuery();
        navega(l, header);        
    }

    private void query6() {
        int X;
        List<ParProdNumClis> topX;
        
        out.print("Quantos elementos quer no top de produtos mais vendidos? ");
        X = Input.lerInt();
        
        Crono.start();
        topX = hipermercado.maisVendidos(X);
        if(topX.isEmpty())
            out.println("Lista vazia");
        else{
            List<String> resultados = new ArrayList<>(topX.size());
            final String header = "Produto | Nº de clientes que compraram";
            
            for(ParProdNumClis par : topX)
                resultados.add(String.format("%7s | %28d", par.getProd(), par.getNumClis()));
            
            LStrings l = new LStrings(resultados);
            Crono.stop();
            imprimeTempoQuery();
            navega(l, header);
        }
    }

    private void query7() {
        int rank, filial;
        ParCliFat[][] res = new ParCliFat[Constantes.N_FILIAIS][];
        
        Crono.start();
        for(filial = 1; filial <= Constantes.N_FILIAIS; ++filial)
            res[filial-1] = hipermercado.tresMaioresCompradores(filial);
        
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
        Formatter formatador = new Formatter(bw);
        
        formatador.format("%9s", "");
        for(rank = 1; rank <= 3; ++rank)
            formatador.format("| %18dº%19s |", rank, "");
        
        formatador.format("%n");
        for(filial = 1; filial <= Constantes.N_FILIAIS; ++filial){
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

    private void query8() {
        out.println("Por implementar...");
    }

    private void query9() {
        out.println("Por implementar...");
    }
    
    private void gravarEstado(){
        if(appPopulada()){
            String fichEstado = obterNomeFicheiro("hipermercado.dat", "estado");
            
            try{
                hipermercado.gravaObj(fichEstado);
                out.println("-> Estado gravado com sucesso!");
            }catch(IOException e){ err.println(e.getMessage()); }
        }
        else
            err.println("Não existem dados para guardar. Primeiro deverá ler os dados.");
    }
    
    private void imprimeTempoQuery(){
        StringBuilder sb = new StringBuilder();
        sb.append("Tempo gasto na query: ").append(Crono.print()).append("s");
        out.println(sb.toString());
        enterParaContinuar();
    }
}
