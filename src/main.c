/* Módulo principal do projeto GereVendas 
 * 
 * Tanto a leitura, como a interpretação de comandos e invocação
 * das queries interativas são feitas neste módulo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "produto.h"
#include "catalogoProds.h"
#include "catalogoClientes.h"
#include "faturacaoGlobal.h"
#include "filial.h"
#include "LStrings.h"

#define N_FILIAIS 3
#define BUF_SIZE 1024

static CatClientes catClientes = NULL;
static CatProds catProds = NULL;
static Filial filiais[N_FILIAIS+1] = {NULL}; /* a cada elemento de indice i do vetor faz corresponder a filial i. */
static FaturacaoGlobal faturacaoGlobal = NULL;

#define N_QUERIES 12
#define N_OPCOES (N_QUERIES + 1)
 
/* Tamanho máximo da linha do interpretador de comandos */
#define MAXLINHA 32

/* Valores de retorno de interpreta() e interpretador() */
#define ERRO -1
#define SAIR 0
#define CONTINUAR 1
#define CMD_INVAL 127

/*endereço padrão dos ficheiros */
#define FCLIENTES "data/Clientes.txt" /* caminho do ficheiro de clientes */
#define FPRODUTOS "data/Produtos.txt" /* caminho do ficheiro de produtos */
#define FVENDAS "data/Vendas_1M.txt"     /* caminho do ficheiro de vendas */

/* Macros utilizadas na navegação e leitura de dados */
#define ENTER_PARA_CONTINUAR() printf("Prima ENTER para continuar: "); getchar()
#define IMPRIME_OPCOES_NAVEGA() \
	printf("1) Pag. ant. | 2) Pag. seg. | 3) Selec. pag. | 4) Prim. pag. | 5) Ult. pag. | 6) Info. | 7) Sair\n")
#define MSG_ERRO(msg) {fputs(msg, stderr); ENTER_PARA_CONTINUAR();}
#define LE_INT_BUFF 16

void splashScreen();
int interpretador();
int interpreta(char linha[]);

/* Funções de navegação exportadas */
void navegaVarias(LStrings lStrArr[], int tamanho);
void navega(LStrings lStr);

static void imprimeOpcoes(const char *opcoes[N_OPCOES]);

/* Apresentação de mensagens de erro */
static void opcaoInvalida(char opcao[]);
static void erroNaoLeuFich();

/* Queries interativas */
typedef int (*Query) (void);
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

Query queries[] = {NULL, query1, query2, query3, query4, query5, query6, query7, query8,
		     query9, query10, query11, query12, sair};

typedef void (*opcaoNavega) (LStrings);

/* Funções de leitura */
static char obterModoRes();
static int leInt(const char msg[]);
static char* leCodigo(const char tipo[], int tamanho, char buffer[]);

static char* avancaEspacosInicio(char str[]);
static void perguntaPag(LStrings lStr);
static void apresentaPag(Pagina pag);
static void imprimeInformacaoLStrings(int total, int numTotalPags);

static opcaoNavega opsNavega[] = {
	NULL,
	pagAnt,
	proxPag,
	perguntaPag,
	primPag,
	ultimaPag
};

/* Opções do interpretador de comandos */
/* A opção de sair deve ser a última apresentada mas deve ter código 0 */
static const char* opcoes[] = {
		NULL,
		"Ler ficheiros",
		"Listar produtos começados por uma letra (maiúscula)",
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

int main()
{
	splashScreen();
	int r = interpretador();
	return r;
}

/* Definição do comando de limpar o ecrã com base no sistema operativo */
#ifdef _WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

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
	char linha[MAXLINHA];

	do{
		system(CLEAR);
		imprimeOpcoes(opcoes);
		r = (fgets(linha, MAXLINHA, stdin) == NULL) ? SAIR : interpreta(linha); /* se falhar deve-se sair? */
	}while(r == CONTINUAR || r == CMD_INVAL); /* enquanto não houver erro ou ordem para sair */
	
	return r;
}

int interpreta(char linha[])
{
	int r;
	int i = atoi(linha);

	linha = avancaEspacosInicio(linha);
	if(linha[0] == '\0')
		r = CONTINUAR;
	else if(i > 0 && i <= N_OPCOES){ /* o utilizador introduziu um comando válido */
		queries[i]();
		r = i == N_OPCOES? SAIR : CONTINUAR; /* se for inserida a ultima opção, o programa deve sair */
	}
	else{
		opcaoInvalida(linha);
		r = CMD_INVAL;
	}
	return r;
}

static char* avancaEspacosInicio(char str[])
{
	int i = 0;

	while(str[i] != '\0' && isspace(str[i]))
		++i;
	return &str[i];
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
{	/* vamos ter que fazer strtok(opcao, "\r\n") de opcao[] porque esta vem com '\n' */
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
		printf("Introduza o número da filial que pretende"
			   "ou %d se pretender sair\n\n>>> ", tamanho + 1);
		i = leInt(NULL);
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
	Pagina pag;
	int opcao;
	int total, numTotalPags; 
	bool sair = FALSE;

	total = obterTotal(lStr); /* número total de entradas na LStrings */
	numTotalPags = obterNumTotalPags(lStr);
	imprimeInformacaoLStrings(total, numTotalPags);
	do{
		system(CLEAR);
		
		pag = obterPag(lStr); /* lê a página atual */
		apresentaPag(pag);
		IMPRIME_OPCOES_NAVEGA();
		printf("(%d/%d): ", obterNumPag(lStr), numTotalPags);
		
		opcao = leInt(NULL);
		if(opcao >= 1 && opcao <= 5)
			opsNavega[opcao](lStr);
		else if(opcao == 6)
			imprimeInformacaoLStrings(total, numTotalPags);
		else if(opcao == 7)
			sair = TRUE;
		else
			MSG_ERRO("Opção inválida\n")
	} while(sair == FALSE);
	system(CLEAR);
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

/* Pergunta ao utilizador para que página pretende ir e avança para
 * a página especificada, se esta for válida. */
static void perguntaPag(LStrings lStr)
{
	int pag;

	pag = leInt("Para que página pretende ir? ");

	if(pag <= 0 || pag > obterNumTotalPags(lStr))
		MSG_ERRO("A página que introduziu é inválida\n")
	else
		irParaPag(pag, lStr);
}

/* Imprime o número de entradas, o total de páginas e o número de entras/página */
static void imprimeInformacaoLStrings(int total, int numTotalPags)
{	
	system(CLEAR);
	printf("Número de entradas: %d\nTotal de páginas: %d\nEntradas por página: %d\n\n",
			total, numTotalPags, STRINGS_POR_PAG);
	ENTER_PARA_CONTINUAR();
}

static FILE * perguntaAbreFicheiro(char * ficheiroPadrao, char buf[BUF_SIZE], char * tipoDeElems){
	FILE * fp;
	char * caminho;

	printf("Insira o caminho do ficheiro de %s (Enter para abrir ficheiro padrao):", tipoDeElems);	
	fgets(buf, BUF_SIZE, stdin);
	caminho = strtok(buf, "\r\n");

	if (caminho) for(; *caminho && isspace(*caminho); caminho++) 
		;
	if(caminho == NULL || *caminho == '\0') 
		caminho = ficheiroPadrao;

	fp = fopen(caminho, "r");
	if(fp == NULL) fprintf(stderr, "Nao foi possivel abrir o ficheiro %s\n", caminho);
	else printf("Ficheiro lido: %s\n", caminho);
	return fp;	
}

int leCatalogoProdutos(){
	Produto p;
	FILE * fp;	
	char buf[BUF_SIZE];
	char * tmp;
	int quantos = 0;

	fp = perguntaAbreFicheiro(FPRODUTOS, buf, "produtos");
	if(fp == NULL) return ERRO;
	while(fgets(buf, BUF_SIZE, fp)){
		tmp = strtok(buf, "\r\n");
		p = criaProduto(tmp);
		if(p == NULL)return ERRO; 
		quantos++;
		catProds = insereProduto(catProds, p); /*inserir tratamento de erros */
		registaProduto(faturacaoGlobal, p);
		apagaProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
	}
	printf("Produtos Lidos: %d\n", totalProdutos(catProds));
	fclose(fp);
	return quantos;
}

int leCatalogoClientes(){
	FILE * fp;	
	char buf[BUF_SIZE];
	Cliente c;
	char * tmp;
	int quantos = 0;

	fp = perguntaAbreFicheiro(FCLIENTES, buf, "clientes");
	if(fp == NULL) return ERRO;

	while(fgets(buf, BUF_SIZE, fp)){
		tmp = strtok(buf, "\r\n");
		c = criaCliente(tmp);
		if(c == NULL) return ERRO;
		quantos++;
		insereCliente(catClientes, c); /*mudar nome para ficar evidente que insere num catalogo */
		/* registar código de cliente na filial para ser mais facil distinguir quando um código é invalido de quando nao fez compras */
		/* registaNovoCliente(FILIAL_GLOBAL, c); */
		apagaCliente(c);
	}
	
	printf("Clientes Lidos: %d\n", totalClientes(catClientes));
	fclose(fp);

	return quantos;
}

/* por get e verifica na mesma macro */
#define GET strtok(NULL," \r\n");
#define VERIFICA(p) {if ((p) == NULL) return ERRO;}

#define MAX_UNIDADES 200
#define MAX_PRECO 999.99

/* Dada uma linha com informação da venda, a função processa a informação da venda e, se for válida, regista a compra */
int insereSeValida(char buf[BUF_SIZE]){
	Cliente cliente;
	Produto produto;
	int unidades, mes, nfilial,  quantos = 0;
	double preco;
	TipoVenda tipoVenda;
	char * it;

	it = strtok(buf, " ");
	VERIFICA(it);
	produto = criaProduto(it);

	it = GET;
	VERIFICA(it);
	preco = atof(it);

	it = GET;
	VERIFICA(it);
	unidades = atoi(it);

	it = GET;
	VERIFICA(it);
	tipoVenda = it[0] == 'P' ? P : N;

	it = GET;
	VERIFICA(it);
	cliente = criaCliente(it);

	it = GET;
	VERIFICA(it);
	mes = atoi(it);

	it = GET;
	VERIFICA(it);
	nfilial = atoi(it);
	
			
	if(existeProduto(catProds, produto) && 
	   existeCliente(catClientes, cliente)  &&  
	   unidades > 0 && unidades <= MAX_UNIDADES &&
	   mes > 0 && mes < 13 &&
	   preco >= 0 && preco <= 999.99 
	   && nfilial > 0 && nfilial <= N_FILIAIS)

	{
		filiais[nfilial] = registaCompra(filiais[nfilial], cliente, produto, mes, tipoVenda, unidades, preco);
		faturacaoGlobal = registaVenda(faturacaoGlobal, produto, preco, unidades, tipoVenda, nfilial, mes) ;
		quantos = 1;
	}
	apagaCliente(cliente);
	apagaProduto(produto);
	return quantos;
}

#undef GET
#undef VERIFICA

int carregaVendasValidas(){
	char buf[BUF_SIZE];
	FILE * fp;
	int validas = 0;

	fp = perguntaAbreFicheiro(FVENDAS, buf, "vendas");
	if(fp == NULL) return ERRO;
	
	while(fgets(buf, BUF_SIZE, fp)){
		validas += insereSeValida(buf);
	}
	
	fclose(fp);
	printf("Vendas Validas: %d\n", validas);
	return 0; /* mudar valor */
}

/* alterar para inserir os caminhos dos ficheiros */
/* se necessaro, inserir FilialTotal ou guardar informação relativa a todos os meses na filial*/
static int query1()
{
	/* apaga os dados de uma execuçao anterior do programa */
	int resL1, resL2, resL3, i;
	/*	
	apagaCatProds(catProds);
	apagaCatClientes(catClientes);
	apagaFaturacaoGlobal(faturacaoGlobal);
	*/
	/*		
	for(i = 1; i <= N_FILIAIS; i++)    
		filiais[i] = apagaFilial(filiais[i]); 

	*/
	/*ver verificacao de erros */
	
	catProds = criaCatProds();
	catClientes = criaCatClientes();	
	faturacaoGlobal = criaFaturacaoGlobal();

	/* alocar espaço no 0 para a globlal */
	for(i = 1; i <= N_FILIAIS; i++)    
		filiais[i] = criaFilial(); 
		
	resL1 = leCatalogoProdutos();		
	resL2 = leCatalogoClientes();
	resL3 = carregaVendasValidas();
	
	return 0;
}

static int query2()
{
	int erro = 0;

	if(catProds)
	{
		char letra;
		LStrings lProdsLetra;

		printf("Introduza a 1ª letra dos códigos de produto que pretende consultar: ");

		letra = toupper(getchar());
		lProdsLetra = prodsPorLetra(catProds, letra);

		if(lProdsLetra){
			navega(lProdsLetra);
			apagaLStrings(lProdsLetra);
		}
		else
			erro = 1;
	}
	return erro;
}

/* Lê um inteiro e devolve-o à função chamadora. Se o utilizador passar
 * uma mensagem como argumento, leInt() apresenta-a antes de ler o inteiro */
static int leInt(const char msg[])
{
	char buffer[LE_INT_BUFF];

	if(msg)
		printf("%s", msg);
	fgets(buffer, LE_INT_BUFF, stdin);
	fflush(stdin);
	return atoi(buffer);
}

/* Recebe o tipo do código a ler (i.e.: produto ou cliente), o seu tamanho
 * e um buffer com o tamanho especificado e lê o código para esse buffer. */
static char* leCodigo(const char tipo[], int tamanho, char* buffer)
{	
	printf("Introduza o código de %s: ", tipo);
	fgets(buffer, tamanho, stdin);
	strtok(buffer, "\r\n");

	return buffer;
}

/* Pergunta ao utilizador se pretende um resultado global (G)
 * ou por filial (F) e lê um carater com a resposta. A validação
 * do carater lido deve ser feita na função chamadora. */
static char obterModoRes()
{	
	char c;

	printf("Resultados globais[G] ou por filial[F]? ");
	c = toupper(getchar());
	fflush(stdin); /* ver se funciona */
	return c;
}

static int query3()
{
	int mes;
	char modo;
	char codigoProd[MAX_CODIGO_PROD], *cod; /* mudar este nome */
	Produto p;
	FatProdMes fProdMes;

	mes = leInt("Introduza o mês: ");
	cod = leCodigo("produto", MAX_CODIGO_PROD, codigoProd);
	modo = obterModoRes();

	p = criaProduto(cod);
	fProdMes = obterFatProdMes(faturacaoGlobal, p, mes);
	apagaProduto(p); /* já não precisamos do produto */

	switch(modo){
		case 'G':
			resultadosGlobaisQuery3(fProdMes);
			break;
		case 'F':
			resultadosFiliaisQuery3(fProdMes);
			break;
		default:
			MSG_ERRO("Erro: Modo inválido\nModos válidos: G | F\n");
	}
	return 0;
}

/* calcula e apresenta os resultados globais da query3 */
static void resultadosGlobaisQuery3(FatProdMes fProdMes)
{
	int totalVendas[N_TIPOS_VENDA];
	double totalFaturado[N_TIPOS_VENDA];

	totalVendas[N] = vendasTotaisProdMes(fProdMes, N),
	totalVendas[P] = vendasTotaisProdMes(fProdMes, P);
	totalFaturado[N] = faturacaoTotalProdMes(fProdMes, N);
	totalFaturado[P] = faturacaoTotalProdMes(fProdMes, P);
		
	puts(" ------------------\n| Resultado global |\n -------------------");
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
}

static int query4()
{
	char modo = obterModoRes();
	int erro = 0;

	if(modo == 'G'){
		LStrings naoCompradosG = naoCompradosGlobal(faturacaoGlobal);
		if(naoCompradosG){
			navega(naoCompradosG);
			apagaLStrings(naoCompradosG);
		}
		else
			erro = 1;
	}
	else if(modo == 'F'){
		LStrings* naoCompradosF = naoCompradosPorFilial(faturacaoGlobal);
		if(naoCompradosF){
			int i;
			navegaVarias(naoCompradosF, N_FILIAIS);
			for(i = 1; i <= N_FILIAIS; ++i)
				apagaLStrings(naoCompradosF[i]);
		}
		else erro = 1; /* definir macros de erros */
	}
	else{
		MSG_ERRO("Erro: Modo inválido\nModos válidos: G | F\n");
		erro = 2;
	}
	return erro;
}

#define IMPRIME_SEPARADOR printf("----------------------------------------------------------------\n");
static int query5()
{
	char buff[16]; /*definir tamanho numa macro */
	char * tok;
	Cliente cliente;
	int totalMes, i, j;
	int * comprasPorFilial[N_FILIAIS+1]; /* ao indice i (!= 0), correspondem as vendas na filial i */	
	printf("Insira um código de cliente: ");
	fgets(buff, 16, stdin);
	/* mover strtok para a criaProdutos? e parte que avanca codigo em branco*/
	/* versao para testar, sem cuidados para erros */
	tok = strtok(buff, " \r\n");
	cliente = criaCliente(tok);
	/*validar codigo de cliente */

	for(i = 1; i <= N_FILIAIS; i++)
		comprasPorFilial[i] = unidadesClientePorMes(filiais[i], cliente); 	
	/* ver o que acontece se for NULL */	
	IMPRIME_SEPARADOR;
	printf("|--Meses--|");
	for(i = 1; i <= N_FILIAIS; i++)
		printf("|--Filial %d--|", i);	
	printf("|--Total--|\n");
	IMPRIME_SEPARADOR;

	for(j = 1; j < 13; j++){
		totalMes = 0;
		printf("|%8d |", j);
		for(i = 1 ; i <= N_FILIAIS; i++){
			printf("|%11d |", comprasPorFilial[i][j]);
			totalMes += comprasPorFilial[i][j];
		}
		printf("|%8d |\n", totalMes);	
		IMPRIME_SEPARADOR;
	}		
	return 0;
}

#undef IMPRIME_SEPARADOR

static int query6()
{	
	int inicio, fim;
	int totalVendas;
	double totalFaturado;

	puts("Intervalo fechado de meses");
	inicio = leInt("Inicio: ");
	fim = leInt("Fim: ");

	totalVendas = totalVendasIntervMeses(faturacaoGlobal, inicio, fim);
	totalFaturado = totalFatIntervMeses(faturacaoGlobal, inicio, fim);

	printf("Intervalo de meses = [%d,%d]\n", inicio, fim);
	printf("Total de unidades vendidas = %d\n"
		   "Total faturado = %.2f", totalVendas, totalFaturado);
	return 0; /* introduzir verificação de erros */
}

static int query7( /* faltam os args */)
{
	return 0;
}

static int query8( /* faltam os args */)
{
	return 0;
}

static int query9( /* faltam os args */)
{
	return 0;
}

static int query10( /* faltam os args */)
{
	return 0;	
}

static int query11( /* faltam os args */)
{
	return 0;
}

static int query12( /* faltam os args */)
{
	return 0;	
}

/* Liberta toda a memória alocada e devolve o valor SAIR para o interpretador */
static int sair( /* faltam os args */ )
{
	return SAIR;
}
/* Função que apresenta uma mensagem de erro quando o utilizador 
 * tenta realizar uma query se ter lido os ficheiros de dados. */
static void erroNaoLeuFich()
{
	fputs("Erro: Ainda não leu os ficheiros de dados\n"
		  "Introduza '1' e prima ENTER para o fazer\n", stderr);
}
