/* Módulo principal do projeto GereVendas 
 * 
 * Tanto a leitura, como a interpretação de comandos e invocação
 * das queries interativas são feitas neste módulo.
 */

#include "cliente.h"
#include "produto.h"
#include "catalogoProds.h"
#include "catalogoClientes.h"
#include "faturacaoGlobal.h"
#include "filial.h"
#include "leitura.h"
#include "LStrings.h"
#include "memUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* >>> Sugestão: Mudar estas definições para um ficheiro gereVendasDef.h */
#define N_FILIAIS 3
#define N_QUERIES 12
#define N_OPCOES (N_QUERIES + 1)
#define TAM_LINHA 1024

/* Definição do comando de limpar o ecrã com base no sistema operativo */
#ifdef _WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

/* Tamanho máximo das linhas lidas com a função fgets() */
#define MAXLINHA 64

/* Valores de retorno de interpreta() e interpretador() */
#define ERRO_MEM -1
#define ERRO_FICH -2
#define INPUT_INVAL -3
#define SAIR 0
#define CONTINUAR 1
#define CMD_INVAL 127

/* Endereços padrão dos ficheiros */
#define FCLIENTES "data/Clientes.txt"
#define FPRODUTOS "data/Produtos.txt"
#define FVENDAS "data/Vendas_1M.txt"

/* Macros utilizadas na navegação, leitura e validação de dados */
#define ENTER_PARA_CONTINUAR() printf("Prima ENTER para continuar: "); getchar()
#define IMPRIME_OPCOES_NAVEGA() \
	printf("1) Pag. ant. | 2) Pag. seg. | 3) Selec. pag. | 4) Prim. pag. | 5) Ult. pag. | 6) Info. | 7) Sair\n")
#define MSG_ERRO(msg) {fputs(msg, stderr); ENTER_PARA_CONTINUAR();}
#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)
#define FILIAL_VALIDA(filial) ((filial) >= 1 && (filial) <= N_FILIAIS)

/* Imprime os tempos se estiver ativado */
#define MODO_MEDICAO_TEMPOS 1
#ifdef MODO_MEDICAO_TEMPOS 
	#define IMPRIME_TEMPOS(s, x) printf("Tempo na %s: %f\n", s, x)
#else
	#define IMPRIME_TEMPOS(s, x)
#endif

typedef int (*Query) (void);
typedef void (*opcaoNavega) (LStrings);

/* Definição das estruturas necessárias ao programa */
static CatClientes catClientes = NULL;
static CatProds catProds = NULL;
static FaturacaoGlobal faturacaoGlobal = NULL;
static Filial filiais[N_FILIAIS+1] = {NULL}; /* a cada elemento de indice i do vetor faz corresponder a filial i. */
static bool fichCarregados = FALSE; /* indica se os ficheiros já foram carregados */

/* o indice 0 da filial guarda informação relativa a todas as compras em 
   todas as filiais. Deste modo, podemos responder às queries mais rapidamente. */ 
#define filialGlobal filiais[0]

/* Opções do interpretador de comandos */
/* A opção de sair deve ser a última apresentada mas deve ter código 0 */
static const char* opcoes[] = {
		NULL,
		"Ler ficheiros",
		"Listar produtos começados por uma letra",
		"Apresentar vendas e faturação totais de um produto, num dado mês",
		"Listar ordenadamente códigos dos produtos que ninguém comprou",
		"Mostrar tabela com as compras efetuadas por um cliente",
		"Determinar vendas e faturação total para um intervalo fechado de meses",
		"Gerar lista ordenada de clientes que realizaram compras em todas as filiais",
		"Determinar códigos e nº total de clientes que compraram um dado produto numa dada filial",
		"Listar (decrescentemente) os produtos mais comprados por um cliente num dado mês",
		"Criar lista com os N produtos mais vendidos do ano",
		"Determinar códigos dos 3 produtos em que um cliente gastou mais dinheiro",
		"Apresentar número de clientes que não compraram e de produtos não vendidos",
		"Sair"
};

void splashScreen();
int interpretador();
int interpreta(char linha[]);
/* Funções de navegação */
void navegaVarias(LStrings lStrArr[], int tamanho);
void navega(LStrings lStr);
/* Recebe um array de strings com opções. Imprime uma opção por linha */
static void imprimeOpcoes(const char *opcoes[N_OPCOES]);
/* Apresentação de mensagens de erro */
static void opcaoInvalida(char opcao[]);

static int query1();
static int query2();
static int query3();
static int query4();
static int query5();
static int query6();
static int query7();
static int query8();
static int query9();
static int query10();
static int query11();
static int query12();

/* Função invocada imediatamente antes de sair */
static int sair();
/* Funções auxiliares das queries */
static void resultadosGlobaisQuery3(FatProdMes);
static void resultadosFiliaisQuery3(FatProdMes);
/* Funções de leitura */
static char obterModoRes();
static char* leCodigo(const char tipo[], int tamanho);

static void perguntaPag(LStrings lStr);
static void apresentaPag(Pagina pag);
static void imprimeInformacaoLStrings(LStrings);

Query queries[] = {NULL, query1, query2, query3, query4, query5, query6, query7, query8,
		     query9, query10, query11, query12, sair};
/* array de apontadores para as funções de navegação de LStrings */
static opcaoNavega opsNavega[] = {
	NULL,
	pagAnt,
	proxPag,
	perguntaPag,
	primPag,
	ultimaPag,
	imprimeInformacaoLStrings
};

int main()
{
	int r;
	splashScreen();
	r = interpretador();
	return r;
}

void splashScreen()
{
	char linha[] = "Prima enter para continuar...";
	system(CLEAR);
	printf(" _____               _   _                _\n");           
	printf("|  __ \\             | | | |              | |\n");          
	printf("| |  \\/ ___ _ __ ___| | | | ___ _ __   __| | __ _ ___\n"); 
	printf("| | __ / _ | '__/ _ | | | |/ _ | '_ \\ / _` |/ _` / __|\n");
	printf("| |_\\ |  __| | |  _ \\ \\_/ |  __| | | | (_| | (_| \\__ \\ \n");
 	printf(" \\____/\\___|_|  \\___|\\___/ \\___|_| |_|\\__,_|\\__,_|___/\n");
	printf("\n%43s", linha);
	getchar();
}

int interpretador()
{
	int r = CONTINUAR;
	char *linha;

	do{
		system(CLEAR);
		imprimeOpcoes(opcoes);
		linha = leLinha(MAXLINHA);
		r = (linha == NULL) ? SAIR : interpreta(linha); /* se falhar deve-se sair? */
		free(linha);
		if(r == ERRO_MEM)
			MSG_ERRO("Não foi possível alocar memória\n");
	}while(r != SAIR); /* enquanto não houver erro ou ordem para sair */
	
	return r;
}

int interpreta(char linha[])
{
	int i;
	int r = CONTINUAR;

	linha = avancaEspacosInicio(linha);
	if(linha[0] == '\0') /* foi introduzida uma linha em branco */
		return CONTINUAR;
	
	i = atoi(linha);
	if(i > 0 && i < N_OPCOES){
		if(i == 1)
			r = queries[1]();
		else if(fichCarregados == FALSE) /* i > 1 && i < N_OPCOES */
			MSG_ERRO("Erro: Ainda não leu os ficheiros de dados\n")
		else
			r = queries[i]();
	}
	else if(i == N_OPCOES)
		r = SAIR;
	else{
		opcaoInvalida(linha);
		r = CMD_INVAL;
	}
	return r;
}

/* Imprime as opções do GereVendas */
static void imprimeOpcoes(const char *opcoes[N_OPCOES])
{
	int i;

	puts("Opções:\n");
	for(i = 1; i <= N_OPCOES; ++i)
		printf("%2d) %s\n", i, opcoes[i]);
	printf("\n>>> ");
}

/* Mensagem de opção inválida */
static void opcaoInvalida(char opcao[])
{
	fprintf(stderr, "A opção '%s' é inválida\n\n", opcao);
}

/* Dado um array de LStrings, permite ao utilizador 
 * escolher em qual LStrings pretende navegar. */
void navegaVarias(LStrings lStrArr[], int tamanho)
{	
	int i;
	bool sair = FALSE;

	do{
		printf("Existem resultados para %d filiais.\n", tamanho);
		printf("Introduza o número da filial que pretende "
			   "ou %d se pretender sair\n\n>>> ", tamanho + 1);
		i = leInt();
		if(i > 0 && i <= tamanho) /* a indexação começa em 1 */
			navega(lStrArr[i]);
		else if(i == (tamanho + 1))
			sair = TRUE;
		else
			MSG_ERRO("Opção inválida\n");
	} while(sair == FALSE);
}

/* Função para navegar numa LStrings */
void navega(LStrings lStr)
{
	if(obterTotal(lStr) == 0){
		puts("Lista vazia");
		ENTER_PARA_CONTINUAR();
	}
	else{
		Pagina pag;
		bool sair = FALSE;
		int opcao, numTotalPags = obterNumTotalPags(lStr);
		
		imprimeInformacaoLStrings(lStr);
		do{
			system(CLEAR);
			
			pag = obterPag(lStr); /* devolve a página atual */
			apresentaPag(pag);
			IMPRIME_OPCOES_NAVEGA();
			printf("(%d/%d): ", obterNumPag(lStr), numTotalPags);
			
			opcao = leInt();
			if(opcao >= 1 && opcao <= 6)
				opsNavega[opcao](lStr); 
			else if(opcao == 7)
				sair = TRUE;
			else
				MSG_ERRO("Opção inválida\n")
		} while(sair == FALSE);
	}
	system(CLEAR);
}

/* Imprime o número de entradas, o total de páginas e o número de entras/página */
static void imprimeInformacaoLStrings(LStrings lStr)
{	
	system(CLEAR);
	printf("Número de entradas: %d\nTotal de páginas: %d\nEntradas por página: %d\n\n",
			  obterTotal(lStr), obterNumTotalPags(lStr), STRINGS_POR_PAG);
	ENTER_PARA_CONTINUAR();
}

/* Pergunta ao utilizador para que página pretende ir e avança para
 * a página especificada, se esta for válida. */
static void perguntaPag(LStrings lStr)
{
	int pag;

	printf("Para que página pretende ir? ");
	pag = leInt();

	if(pag <= 0 || pag > obterNumTotalPags(lStr))
		MSG_ERRO("A página que introduziu é inválida\n")
	else
		irParaPag(pag, lStr);
}

/* Apresenta uma página */
static void apresentaPag(Pagina pag)
{	
	char* linha;

	while((linha = obterLinha(pag)) != NULL){
		puts(linha);
		free(linha); /* obterLinha() devolve uma cópia da linha guardada em pag */
	}
}

static FILE* perguntaAbreFicheiro(char* ficheiroPadrao, char linha[TAM_LINHA], char* tipoDeElems){
	FILE* fp;
	char* caminho;

	printf("Insira o caminho do ficheiro de %s (Enter para abrir ficheiro padrao):", tipoDeElems);	
	fgets(linha, TAM_LINHA, stdin);
	caminho = strtok(linha, "\r\n");

	if(caminho != NULL)
		caminho = avancaEspacosInicio(caminho);
	if(caminho == NULL || caminho[0] == '\0') 
		caminho = ficheiroPadrao;

	fp = fopen(caminho, "r");
	if(fp == NULL)
		fprintf(stderr, "Nao foi possivel abrir o ficheiro %s\n", caminho);
	else
		printf("A ler o ficheiro: %s\n", caminho);
	return fp;	
}

int leCatalogoProdutos(){
	time_t inicio, fim;
	FILE* fp;	
	char linha[TAM_LINHA];
	char* linhaLimpa;
	int quantos = 0;
	Produto p;
	CatProds novoCatP;
	FaturacaoGlobal novaFatG;

	fp = perguntaAbreFicheiro(FPRODUTOS, linha, "produtos");
	inicio = time(NULL); 
	if(fp == NULL)
		return ERRO_FICH;
	
	while(fgets(linha, TAM_LINHA, fp)){
		linhaLimpa = strtok(linha, "\r\n");
		p = criaProduto(linhaLimpa);
		if(p == NULL) /* falha de alocação ao criar o produto */
			return ERRO_MEM; 
		
		novoCatP = insereProduto(catProds, p);
		if(novoCatP == NULL) /* falha de alocação a inserir o produto */
			return ERRO_MEM;
		else
			catProds = novoCatP;
		
		novaFatG = registaProduto(faturacaoGlobal, p);
		if(novaFatG == NULL)
			return ERRO_MEM;
		else
			faturacaoGlobal = novaFatG; 
		++quantos;
		apagaProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("leitura de Produtos", difftime(fim,inicio));
	fclose(fp);
	return quantos;
}

int leCatalogoClientes(){
	FILE* fp;	
	char linha[TAM_LINHA];
	char* linhaLimpa;
	int quantos = 0;
	Cliente c;
	CatClientes novoCatC;
	time_t inicio, fim;

	fp = perguntaAbreFicheiro(FCLIENTES, linha, "clientes");
	inicio = time(NULL); 
	if(fp == NULL)
		return ERRO_FICH;
	
	while(fgets(linha, TAM_LINHA, fp)){
		linhaLimpa = strtok(linha, "\r\n");
		c = criaCliente(linhaLimpa);
		if(c == NULL) /* falha de alocação ao criar o cliente */
			return ERRO_MEM;
		novoCatC = insereCliente(catClientes, c); /*mudar nome para ficar evidente que insere num catalogo */
		if(novoCatC == NULL)
			return ERRO_MEM;
		else
			catClientes = novoCatC;
		apagaCliente(c);
		quantos++;
	}
	fim = time(NULL);
	fclose(fp);
	IMPRIME_TEMPOS("leitura de clientes", difftime(fim,inicio));
	return quantos;
}

/* por get e verifica na mesma macro */
#define GET strtok(NULL," \r\n");

#define MAX_UNIDADES 200
#define MAX_PRECO 999.99

/* Dada uma linha com informação da venda, a função processa a informação da venda e, se for válida, regista a compra */
int insereSeValida(char linha[TAM_LINHA]){
	Cliente cliente;
	Produto produto;
	int unidades, mes, nfilial,  quantos = 0;
	double preco;
	TipoVenda tipoVenda;
	char* it;

	it = strtok(linha, " ");
	produto = criaProduto(it);

	it = GET;
	preco = atof(it);

	it = GET;
	unidades = atoi(it);

	it = GET;
	tipoVenda = it[0] == 'P' ? P : N;

	it = GET;
	cliente = criaCliente(it);

	it = GET;
	mes = atoi(it);

	it = GET;
	nfilial = atoi(it);
			
	if(existeProduto(catProds, produto) && 
	   existeCliente(catClientes, cliente)  &&  
	   unidades > 0 && unidades <= MAX_UNIDADES &&
	   mes > 0 && mes < 13 &&
	   preco >= 0 && preco <= 999.99 
	   && nfilial > 0 && nfilial <= N_FILIAIS)
	{
		filiais[nfilial] = registaCompra(filiais[nfilial], cliente, produto, mes, tipoVenda, unidades, preco);
		filialGlobal = registaCompra(filialGlobal, cliente, produto, mes, tipoVenda, unidades, preco);
		faturacaoGlobal = registaVenda(faturacaoGlobal, produto, preco, unidades, tipoVenda, nfilial, mes) ;
		quantos = 1;
	}
	apagaCliente(cliente);
	apagaProduto(produto);
	return quantos;
}

#undef GET

int carregaVendasValidas(){
	time_t inicio, fim;
	char linha[TAM_LINHA];
	FILE* fp;
	int validas = 0;

	fp = perguntaAbreFicheiro(FVENDAS, linha, "vendas");
	if(fp == NULL)
		return ERRO_FICH;

	inicio = time(NULL);
	while(fgets(linha, TAM_LINHA, fp)){
		validas += insereSeValida(linha);
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("leitura de vendas", difftime(fim, inicio));
	fclose(fp);
	return validas; 
}

/* alterar para inserir os caminhos dos ficheiros */
/* se necessaro, inserir FilialTotal ou guardar informação relativa a todos os meses na filial*/
static int query1()
{
	/* apaga os dados de uma execuçao anterior do programa */
	int i, produtosLidos, vendasValidas, clientesLidos;
	
	catProds = apagaCatProds(catProds);
	catClientes = apagaCatClientes(catClientes);
	faturacaoGlobal = apagaFaturacaoGlobal(faturacaoGlobal);
	fichCarregados = FALSE;
	 
	for(i = 1; i <= N_FILIAIS; i++)    
		filiais[i] = apagaFilial(filiais[i]); 

	catProds = criaCatProds();
	if(catProds == NULL){ /* falha de alocação a criar o catálogo de produtos */
		MSG_ERRO("Erro a criar o catálogo de produtos\n");
		return ERRO_MEM;
	}
	
	catClientes = criaCatClientes();
	if(catClientes == NULL){ /* falha de alocação a criar o catálogo de clientes */
		catProds = apagaCatProds(catProds);
		MSG_ERRO("Erro a criar o catálogo de clientes\n");
		return ERRO_MEM;
	}	
	
	faturacaoGlobal = criaFaturacaoGlobal();
	if(faturacaoGlobal == NULL){ /* falha de alocação a criar a faturação global */
		catProds = apagaCatProds(catProds);
		catClientes = apagaCatClientes(catClientes);
		MSG_ERRO("Erro a criar a faturação global\n");
		return ERRO_MEM;
	}

	/* alocar espaço no 0 para a globlal */
	for(i = 0; i <= N_FILIAIS; i++){   
		filiais[i] = criaFilial();
		if(filiais[i] == NULL){
			catProds = apagaCatProds(catProds); catClientes = apagaCatClientes(catClientes);
			for( ; i >= 0; --i)
				apagaFilial(filiais[i]);
			MSG_ERRO("Erro a criar as filiais\n");
			return ERRO_MEM;
		}
	}
	clientesLidos = leCatalogoClientes();
	produtosLidos = leCatalogoProdutos();
	vendasValidas = carregaVendasValidas();
	fichCarregados = TRUE;
		
	printf("Nº de clientes lidos: %d\n"
		   "Nº de Produtos lidos: %d\n"
		   "Nº de vendas validas: %d\n", clientesLidos, produtosLidos, vendasValidas);
	ENTER_PARA_CONTINUAR();
	return CONTINUAR;
}

static int query2()
{
	int r = CONTINUAR;
	char letra;

	printf("Introduza a 1ª letra dos códigos de produto que pretende consultar: ");

	letra = toupper(getchar());
	FLUSH_STDIN();

	if(isalpha(letra)){
		LStrings lProdsLetra = prodsPorLetra(catProds, letra);

		if(lProdsLetra){
			navega(lProdsLetra);
			apagaLStrings(lProdsLetra);
		}
		else
			r = ERRO_MEM;
	}
	else{
		fprintf(stderr, "O caratere '%c' é inválido\n", letra);
		r = INPUT_INVAL;
	}
	ENTER_PARA_CONTINUAR();
	return r;
}

static char* leCodigo(const char tipo[], int tamanho)
{	
	printf("Introduza o código de %s: ", tipo);
	return leLinha(tamanho);
}

static char obterModoRes()
{	
	char c;

	printf("Resultados globais[G] ou por filial[F]? ");
	c = toupper(getchar());
	FLUSH_STDIN();
	return c;
}

static int query3()
{	
	char modo;
	char *codigoProd; /* >>> mudar este nome??? */
	Produto p;
	FatProdMes fProdMes;
	int mes, r = CONTINUAR;

	codigoProd = leCodigo("produto", MAX_CODIGO_PROD);
	p = criaProduto(codigoProd);

	if(p == NULL) /* falha de alocação a criar o produto */
		return ERRO_MEM;
	if(existeProduto(catProds, p)){
		printf("Mês: "); mes = leInt();
		if(MES_VALIDO(mes)){
			modo = obterModoRes();
			fProdMes = obterFatProdMes(faturacaoGlobal, p, mes);

			switch(modo){
				case 'G':
					resultadosGlobaisQuery3(fProdMes);
					break;
				case 'F':
					resultadosFiliaisQuery3(fProdMes);
					break;
				default:
					MSG_ERRO("Erro: Modo inválido\nModos válidos: G | F\n");
					r = INPUT_INVAL;
					break;
			}
			apagaFatProdMes(fProdMes);
		}
		else{ /* o mês introduzido é inválido */
			fprintf(stderr, "O mês '%d' é inválido\n", mes);
			r = INPUT_INVAL;
		}
		free(codigoProd); apagaProduto(p);/* já não precisamos do produto */
	}
	else{ /* o código de produto introduzido é inválido */
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigoProd);
		free(codigoProd);
		r = INPUT_INVAL;
	}
	ENTER_PARA_CONTINUAR();
	return r;
}

/* calcula e apresenta os resultados globais da query3 */
static void resultadosGlobaisQuery3(FatProdMes fProdMes)
{
	int totalVendas[N_TIPOS_VENDA];
	double totalFaturado[N_TIPOS_VENDA];

	totalVendas[N] = vendasTotaisProdMes(fProdMes, N);
	totalVendas[P] = vendasTotaisProdMes(fProdMes, P);
	totalFaturado[N] = faturacaoTotalProdMes(fProdMes, N);
	totalFaturado[P] = faturacaoTotalProdMes(fProdMes, P);

	puts(" ------------------\n| Resultado global |\n ------------------");
	printf("Vendas N = %d, Vendas P = %d\nFaturação N = %.2f, Faturação P = %.2f\n",
			totalVendas[N], totalVendas[P], totalFaturado[N], totalFaturado[P]);
}

/* calcula e apresenta os resultados da query3 por filial */
static void resultadosFiliaisQuery3(FatProdMes fProdMes)
{
	int filial;
	int* vendasFilial[N_TIPOS_VENDA];
	double* faturacaoFilial[N_TIPOS_VENDA];

	vendasFilial[N] = vendasPorFilialProdMes(fProdMes, N);
	vendasFilial[P] = vendasPorFilialProdMes(fProdMes, P);
	faturacaoFilial[N] = faturacaoPorFilialProdMes(fProdMes, N);
	faturacaoFilial[P] = faturacaoPorFilialProdMes(fProdMes, P);

	for(filial = 1; filial <= N_FILIAIS; ++filial){
		printf(" ----------\n| Filial %d |\n ----------\n", filial);
		printf("Vendas N = %d, Vendas P = %d\n"
				"Faturação N = %.2f, Faturação P = %.2f\n\n",
				vendasFilial[N][filial], vendasFilial[P][filial],
				faturacaoFilial[N][filial], faturacaoFilial[P][filial]);
	}
	free(vendasFilial[N]); free(vendasFilial[P]);
	free(faturacaoFilial[N]); free(faturacaoFilial[P]);
}

static int query4()
{
	char modo = obterModoRes();
	int r = CONTINUAR;

	if(modo == 'G'){
		LStrings naoCompradosG = naoCompradosGlobal(faturacaoGlobal);
		if(naoCompradosG){
			navega(naoCompradosG);
			apagaLStrings(naoCompradosG);
		}
		else
			r = ERRO_MEM;
	}
	else if(modo == 'F'){
		LStrings* naoCompradosF = naoCompradosPorFilial(faturacaoGlobal);
		if(naoCompradosF){
			int i;
			navegaVarias(naoCompradosF, N_FILIAIS);
			for(i = 1; i <= N_FILIAIS; ++i)
				apagaLStrings(naoCompradosF[i]);
		}
		else
			r = ERRO_MEM;
	}
	else{
		MSG_ERRO("Erro: Modo inválido\nModos válidos: G | F\n");
		r = INPUT_INVAL;
	}
	return r;
}

#define IMPRIME_SEPARADOR printf("----------------------------------------------------------------\n");
static int query5()
{
	char buff[16]; /* >>> definir tamanho numa macro */
	char* codigoCliente;
	int r = CONTINUAR;
	Cliente cliente;

	printf("Insira um código de cliente: ");
	fgets(buff, 16, stdin);
	/* mover strtok para a criaProdutos? e parte que avanca codigo em branco*/
	/* versao para testar, sem cuidados para erros */
	codigoCliente = strtok(buff, " \r\n");
	if((cliente = criaCliente(codigoCliente)) == NULL) /* falha de alocação */
		return ERRO_MEM;
	
	if(existeCliente(catClientes, cliente)){
		int i, j, totalMes;
		int* comprasPorFilial[N_FILIAIS+1]; /* ao indice i (!= 0), correspondem as vendas na filial i */

		for(i = 1; i <= N_FILIAIS; i++)
			comprasPorFilial[i] = unidadesClientePorMes(filiais[i], cliente); 	
		/* ver o que acontece se for NULL */	
		IMPRIME_SEPARADOR;
		printf("|--Meses--|");
		for(i = 1; i <= N_FILIAIS; i++)
			printf("|--Filial %d--|", i);	
		printf("|--Total--|\n");
		IMPRIME_SEPARADOR;

		for(j = 1; j < 13; j++){ /* usar macro N_MESES */
			totalMes = 0;
			printf("|%8d |", j);
			for(i = 1 ; i <= N_FILIAIS; i++){
				printf("|%11d |", comprasPorFilial[i][j]);
				totalMes += comprasPorFilial[i][j];
			}
			printf("|%8d |\n", totalMes);	
			IMPRIME_SEPARADOR;
		}
	}
	else{ /* o código de cliente é inválido */
		fprintf(stderr, "O código de cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
	}
	ENTER_PARA_CONTINUAR();
	return r;
}

#undef IMPRIME_SEPARADOR

static int query6()
{	
	int inicio, fim;
	int r = CONTINUAR;

	puts("Introduza um intervalo fechado de meses");
	printf("Inicio: "); inicio = leInt();
	printf("Fim: "); fim = leInt();

	if(MES_VALIDO(inicio) && MES_VALIDO(fim)){
		int totalVendas = totalVendasIntervMeses(faturacaoGlobal, inicio, fim);
		double totalFaturado = totalFatIntervMeses(faturacaoGlobal, inicio, fim);

		printf("Intervalo de meses = [%d,%d]\n", inicio, fim);
		printf("Total de unidades vendidas = %d\n"
			   "Total faturado = %.2f\n\n", totalVendas, totalFaturado);
		ENTER_PARA_CONTINUAR();
	}
	else{
		MSG_ERRO("Pelo menos um dos meses introduzidos é inválido\n");
		r = INPUT_INVAL;
	}
	return r;
}

static bool comprouTodasFiliais(Cliente c){
	int i;
	bool r = TRUE;

	for(i = 1; r && i <= N_FILIAIS; i++){
		r = clienteComprouNaFilial(filiais[i], c) == TRUE;
	}
	return r;
}

int query7()
{
	Cliente* clientes;
	char** codigosClientes;
	int i, j, nClientes;
	LStrings lista;
	
	nClientes = totalClientes(catClientes);
	clientes = todosClientes(catClientes);
	if(clientes == NULL) /* falha de alocação */
		return ERRO_MEM;

	codigosClientes = malloc(nClientes * sizeof(char *));
	if(codigosClientes == NULL){ /* falha a alocar o array 'codigosClientes' */
		for(i = 0; i < nClientes; ++i);
			apagaCliente(clientes[i]);
		return ERRO_MEM;
	}

	for(i = 0, j = 0; i < nClientes; i++){
		if(comprouTodasFiliais(clientes[i])){
			codigosClientes[j++] = obterCodigoCliente(clientes[i]);
		}
		apagaCliente(clientes[i]);	
	}
	free(clientes);

	lista = criaLStrings(j, codigosClientes);
	apagaArray((void **) codigosClientes, j, free); /* os códigos de clientes já estão na LStrings 'lista' */
	navega(lista);
	apagaLStrings(lista);

	return CONTINUAR;
}

int query8(){
	int i, indexP, indexN;
	int filial, nClientes, r = CONTINUAR;
	char c;
	char* codigo;
	char** quemComprouN, **quemComprouP;
	bool comprouN, comprouP;
	Cliente* clientes;
	Produto produto;
	LStrings compraramN, compraramP;

	printf("Insira o código de Produto: "); /* falta validar o produto */
	codigo = leLinha(MAX_CODIGO_PROD); /* >>> MAX_CODIGO_PROD já não é um valor fixo */
	codigo = strtok(codigo, " \t\r\n");
	/* funçoes de leitura de Produto e caso a strtok ou leLinha falhem */;
	/*verificar se o codigo é valido*/
	produto = criaProduto(codigo);
	if(existeProduto(catProds, produto)){
		printf("Insira a filial que pretende consultar: ");
		filial = leInt();
		indexP = indexN = 0;	
		if(FILIAL_VALIDA(filial)){
			nClientes = totalClientes(catClientes);
			clientes = todosClientes(catClientes);
			/* ver se o tamanho esta certo */
			quemComprouN = malloc(sizeof(char *) * nClientes); 	
			quemComprouP = malloc(sizeof(char *) * nClientes); 	
			for(i = 0; i < nClientes; i++){
				comprou(filiais[filial], clientes[i], produto, &comprouN, &comprouP); 
				if(comprouN){
					quemComprouN[indexN++] = obterCodigoCliente(clientes[i]);
				}
				if(comprouP){
					quemComprouP[indexP++] = obterCodigoCliente(clientes[i]);
				}
				apagaCliente(clientes[i]);
			}
			/* >>> a alocação de memória para as LStrings pode falhar */
			compraramN = criaLStrings(indexN, quemComprouN);
			compraramP = criaLStrings(indexP, quemComprouP);
			apagaProduto(produto);
			apagaArray((void **) quemComprouN, indexN, free);
			apagaArray((void **) quemComprouP, indexP, free);
			
			do {
				printf("%d clientes compraram o produto em modo normal\n", indexN); 
				printf("%d clientes compraram o produto em Promocao\n", indexP); 

				printf("Insira o modo que pretende consultar (P ou N) ou S para sair: ");
				/* substituir getchar por outra coisa qq */		
				c = getchar();
				if(c == 'P')
					navega(compraramP);
				else if(c == 'N')
					navega(compraramN);
			} while(c != 'S');						
		}
		else{ /* a filial introduzida é inválida */
			MSG_ERRO("Filial inválida\n");
			r = INPUT_INVAL;
		}
	}
	else{
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigo);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	free(codigo);
	return r;
}

static int query9()
{	
	int mes, r = CONTINUAR;
	char* codigoCliente;
	Cliente c;

	printf("Introduza o código do cliente: ");
	codigoCliente = leLinha(MAX_CODIGO_CLIENTE); /* >>> MAX_CODIGO_CLIENTE já não é fixo */
	c = criaCliente(codigoCliente);
	if(c == NULL)
		return ERRO_MEM;

	if(existeCliente(catClientes, c)){
		printf("Introduza o mês: "); mes = leInt();
		
		if(MES_VALIDO(mes)){
			LStrings lStr = produtosClienteMaisComprou(filialGlobal, c, mes);
			navega(lStr);
			apagaLStrings(lStr);
		}
		else{
			fprintf(stderr, "O mês '%d' é inválido.\n", mes);
			r = INPUT_INVAL;
		}
	}
	else{
		fprintf(stderr, "O cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	free(codigoCliente);
	apagaCliente(c);
	
	return r;
}
/* Limpar o código desta função */
static int query10()
{
	/* fazer tabela em vez de LString */
	char* linha;
	int N, i, filial, nClientes, nUnidades;
	LStrings resultados[N_FILIAIS+1] = {NULL};

	printf("Quantos produtos pretende consultar? ");
	N = leInt();

	do{
		printf("Existem resultados para %d filiais. Insira o numero da filial ou %d para sair\n", N_FILIAIS, N_FILIAIS+1);
		filial = leInt();
		if(FILIAL_VALIDA(filial)){
			if(resultados[filial] == NULL){ /* ainda não calculamos a LString com os resultados da filial pedida */
				char** produtos = NmaisVendidosFilial(faturacaoGlobal, N, filial);
				char** imprimir = malloc(sizeof(char *) * N);
				if(imprimir == NULL) /* falha de alocação */
					return ERRO_MEM;
				
				for(i = 0; i < N; i++){ /* cria as linhas a introduzir na LStrings */
					linha = malloc(sizeof(char *) * (strlen(produtos[i]) + 150));	/* >>> valor exagerado */
					nClientes = numeroClientesCompraramProduto(filiais[filial], produtos[i], &nUnidades);
					/* >>> acrescentar larguras aos campos deste sprintf() */
					sprintf(linha, "%s, Numero clientes: %d, Numero unidades vendidas %d", produtos[i], nClientes, nUnidades);
					imprimir[i] = linha;
				}
				resultados[filial] = criaLStrings(N, imprimir);
				apagaArray((void **) imprimir, N, free);
			}
			navega(resultados[filial]);
		}
		else if(filial != N_FILIAIS + 1)
			MSG_ERRO("Opção inválida\n");
	} while (filial != N_FILIAIS + 1);
	
	for(filial = 1; filial <= N_FILIAIS; ++filial) /* liberta memória alocada para a(s) LStrings */
		apagaLStrings(resultados[filial]);
	
	return CONTINUAR;
}

static int query11()
{	
	int r = CONTINUAR;
	Cliente c;
	char* codigoCliente;

	printf("Introduza o código do cliente: ");
	codigoCliente = leLinha(MAX_CODIGO_CLIENTE); /* <<< O tamanho do código já não é fixo */
	c = criaCliente(codigoCliente);
	if(c == NULL)
		return ERRO_MEM;

	if(existeCliente(catClientes, c)){
		char** top3 = tresProdsEmQueMaisGastou(filialGlobal, c);
		if(top3){
			int i;
			printf("Códigos dos 3 produtos em que o cliente %s gastou mais dinheiro\n", codigoCliente);
			for(i = 0; i < 3; ++i)
				printf("%dº: %s\n", i+1, top3[i]);
			apagaArray((void **) top3, 3, free);
			ENTER_PARA_CONTINUAR();
		}
		else
			r = ERRO_MEM;
	}
	else{
		fprintf(stderr, "O cliente %s não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
	}
	free(codigoCliente);
	apagaCliente(c);
	return r;
}

static int query12()
{
	int nClientesNaoCompr = 0;
	int nProdsNaoVendidos;

	nClientesNaoCompr = totalClientes(catClientes) - quantosClientesCompraram(filialGlobal);
	nProdsNaoVendidos = quantosNaoComprados(faturacaoGlobal);
	printf("Número de clientes que não compraram: %d\n", nClientesNaoCompr);
	printf("Número de produtos não vendidos: %d\n", nProdsNaoVendidos);
	ENTER_PARA_CONTINUAR();

	return CONTINUAR;
}

/* Liberta toda a memória alocada e devolve o valor SAIR para o interpretador */
static int sair()
{	
	int i;

	catProds = apagaCatProds(catProds);
	catClientes = apagaCatClientes(catClientes);
	faturacaoGlobal = apagaFaturacaoGlobal(faturacaoGlobal);
	
	for(i = 0; i <= N_FILIAIS; ++i)
		filiais[i] = apagaFilial(filiais[i]);
	
	fichCarregados = FALSE;
	return SAIR;
}
