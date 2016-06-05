public class HipermercadoApp {
/*	private Hipermercado hipermercado;
	private EstatisticaFicheiro estatisticasFicheiro;
	private EstatisticaGeral estatisticasGerais;
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

	private List<String> leLinhasComBuff(String ficheiroPadrao, String tipoFicheiro) {
		List<String> linhas = new ArrayList<>();
		BufferedReader inStream = null;
		String linha = null;



	}
	private void lerDados() {
		CatalogoProdutos catalogoProdutos;
		CatalogoClientes catalogoClientes;
		FaturacaoGlobal faturacaoGlobal;
		Filial[N_FILIAIS+1] filiais;
		Set<String> produtos, clientes;

		produtos = leFicheiroProdutos();
		leFicheiroClientes();
		leFicheiroVendas();

	}

	private Set<String> leFicheiroProdutos() {

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
	}*/
}
