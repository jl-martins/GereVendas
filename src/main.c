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
#include "leitura.h"

#define N_FILIAIS 3
#define TAM_LINHA 1024

static CatClientes catClientes = NULL;
static CatProds catProds = NULL;
static Filial filiais[N_FILIAIS+1] = {NULL}; /* a cada elemento de indice i do vetor faz corresponder a filial i. */
#define filialGlobal filiais[0] /* o indice 0 da filial vai guardar informação relativa a todas as compras em todas as filiais,
				   permite fazer as queries de forma muito mais rapida */ 
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
static char* leCodigo(const char tipo[], int tamanho);

static void perguntaPag(LStrings lStr);
static void apresentaPag(Pagina pag);
static void imprimeInformacaoLStrings(LStrings);

static opcaoNavega opsNavega[] = {
	NULL,
	pagAnt,
	proxPag,
	perguntaPag,
	primPag,
	ultimaPag,
	imprimeInformacaoLStrings
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
	int r;
	splashScreen();
	r = interpretador();
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
	char *linha;

	do{
		system(CLEAR);
		imprimeOpcoes(opcoes);
		linha = leLinha(MAXLINHA);
		r = (linha == NULL) ? SAIR : interpreta(linha); /* se falhar deve-se sair? */
		if(linha)
			free(linha);
	}while(r == CONTINUAR || r == CMD_INVAL); /* enquanto não houver erro ou ordem para sair */
	
	return r;
}

int interpreta(char linha[])
{
	int r;
	int i;

	linha = avancaEspacosInicio(linha);
	if(linha[0] == '\0') /* foi introduzida uma linha em branco */
		r = CONTINUAR;
	else{
		i = atoi(linha);
		if(i > 0 && i <= N_OPCOES){ /* o utilizador introduziu um comando válido */
			queries[i]();
			r = (i == N_OPCOES) ? SAIR : CONTINUAR; /* se for inserida a ultima opção, o programa deve sair */
		}
		else{
			opcaoInvalida(linha);
			r = CMD_INVAL;
		}
		ENTER_PARA_CONTINUAR();
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
		int numTotalPags = obterNumTotalPags(lStr);
		int opcao;
		Pagina pag;
		bool sair = FALSE;
		
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
	if(caminho == NULL || caminho == '\0') 
		caminho = ficheiroPadrao;

	fp = fopen(caminho, "r");
	if(fp == NULL)
		fprintf(stderr, "Nao foi possivel abrir o ficheiro %s\n", caminho);
	else /* imprimir esta mensagem só depois de se ter percorrido o ficheiro todo */
		printf("Ficheiro lido: %s\n", caminho);
	return fp;	
}

int leCatalogoProdutos(){
	FILE* fp;	
	char linha[TAM_LINHA];
	char* linhaLimpa;
	int quantos = 0;
	Produto p;
	CatProds novoCatP;
	FaturacaoGlobal novaFatG;

	fp = perguntaAbreFicheiro(FPRODUTOS, linha, "produtos");
	if(fp == NULL)
		return ERRO;
	while(fgets(linha, TAM_LINHA, fp)){
		linhaLimpa = strtok(linha, "\r\n");
		p = criaProduto(linhaLimpa);
		if(p == NULL) /* falha de alocação ao criar o produto */
			return ERRO; 
		quantos++;
		
		novoCatP = insereProduto(catProds, p);
		if(novoCatP == NULL) /* falha de alocação a inserir o produto */
			return ERRO;
		else
			catProds = novoCatP;
		
		novaFatG = registaProduto(faturacaoGlobal, p);
		if(novaFatG == NULL)
			return ERRO;
		else
			faturacaoGlobal = novaFatG; 
		apagaProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
	}
	printf("Número de produtos lidos: %d\n", totalProdutos(catProds));
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

	fp = perguntaAbreFicheiro(FCLIENTES, linha, "clientes");
	if(fp == NULL)
		return ERRO;

	while(fgets(linha, TAM_LINHA, fp)){
		linhaLimpa = strtok(linha, "\r\n");
		c = criaCliente(linhaLimpa);
		if(c == NULL) /* falha de alocação ao criar o cliente */
			return ERRO;
		quantos++;
		novoCatC = insereCliente(catClientes, c); /*mudar nome para ficar evidente que insere num catalogo */
		if(novoCatC == NULL)
			return ERRO;
		else
			catClientes = novoCatC;
		/* registar código de cliente na filial para ser mais facil distinguir quando um código é invalido de quando nao fez compras */
		/* registaNovoCliente(FILIAL_GLOBAL, c); */
		apagaCliente(c);
	}
	printf("Número de clientes lidos: %d\n", totalClientes(catClientes));
	fclose(fp);

	return quantos;
}

/* por get e verifica na mesma macro */
#define GET strtok(NULL," \r\n");
#define VERIFICA(p) {if ((p) == NULL) return ERRO;}

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
		filialGlobal = registaCompra(filialGlobal, cliente, produto, mes, tipoVenda, unidades, preco);
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
	char linha[TAM_LINHA];
	FILE* fp;
	int validas = 0;

	fp = perguntaAbreFicheiro(FVENDAS, linha, "vendas");
	if(fp == NULL)
		return ERRO;
	
	while(fgets(linha, TAM_LINHA, fp)){
		validas += insereSeValida(linha);
	}

	fclose(fp);
	printf("Número de vendas válidas: %d\n", validas);
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
	for(i = 0; i <= N_FILIAIS; i++)    
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
		FLUSH_STDIN();
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

/* Recebe o tipo do código a ler (i.e.: produto ou cliente) e o tamanho
 * máximo do mesmo e lê o código do stdin. Se a leitura tiver sucesso,
 * é devolvido o código lido, se não é devolvido NULL. O código devolvido
 * deve ser libertado com a função free(), quando já não for necessário. */
static char* leCodigo(const char tipo[], int tamanho)
{	
	printf("Introduza o código de %s: ", tipo);
	return leLinha(tamanho);
}

/* Pergunta ao utilizador se pretende um resultado global (G)
 * ou por filial (F) e lê um carater com a resposta. A validação
 * do carater lido deve ser feita na função chamadora. */
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
	int mes;
	char modo;
	char *codigoProd; /* mudar este nome */
	Produto p;
	FatProdMes fProdMes;

	codigoProd = leCodigo("produto", MAX_CODIGO_PROD);
	printf("Mês: "); mes = leInt();
	modo = obterModoRes();

	p = criaProduto(codigoProd);
	fProdMes = obterFatProdMes(faturacaoGlobal, p, mes);
	free(codigoProd); apagaProduto(p);/* já não precisamos do produto nem do código */

	switch(modo){
		case 'G':
			resultadosGlobaisQuery3(fProdMes);
			break;
		case 'F':
			resultadosFiliaisQuery3(fProdMes);
			break;
		default:
			MSG_ERRO("Erro: Modo inválido\nModos válidos: G | F\n");
			break;
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

	puts("Introduza um intervalo fechado de meses");
	printf("Inicio: "); inicio = leInt();
	printf("Fim: "); fim = leInt();

	totalVendas = totalVendasIntervMeses(faturacaoGlobal, inicio, fim);
	totalFaturado = totalFatIntervMeses(faturacaoGlobal, inicio, fim);

	printf("Intervalo de meses = [%d,%d]\n", inicio, fim);
	printf("Total de unidades vendidas = %d\n"
		   "Total faturado = %.2f\n\n", totalVendas, totalFaturado);
	return 0; /* introduzir verificação de erros */
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
	Cliente * clientes;
	char ** codigosClientes;
	int nClientes, i, j;
	LStrings lista;

	i = 0; j = 0;	
	/* impedir que leia se os ficheiros nao tiverem sido carregados */
	clientes = todosClientes(catClientes, &nClientes);
	/* if(clientes == NULL) ... */
	codigosClientes = malloc(nClientes * sizeof(char *));
	for(i = 0; i < nClientes; i++){
		if(comprouTodasFiliais(clientes[i])){
			codigosClientes[j++] = obterCodigoCliente(clientes[i]);
		}
		apagaCliente(clientes[i]);	
	}
	free(clientes);

	/*fazer um realloc do codigosClientes para gastar menos espaço??*/
	lista = criaLStrings(j, codigosClientes);		
	navega(lista);

	/* Limpeza das estruturas usadas na função */
	for(i = 0; i < j; i++) free(codigosClientes[i]);
	free(codigosClientes);
	apagaLStrings(lista);	

	return 0;
}

int query8(){
	int filial, ret, tamanho, indexP, indexN, i, resposta;
	Cliente * clientes;
	Produto produto;
	int nClientes;
	char ** quemComprouN, ** quemComprouP;
	char * codigo;
	char c;
	bool comprouN, comprouP;
	LStrings compraramN, compraramP;

	printf("Insira o código de Produto: ");
	codigo = leLinha(MAX_CODIGO_PROD+1);
	codigo = strtok(codigo, " \t\r\n");	
	printf("%s\n", codigo)
	/* funçoes de leitura de Produto e caso a strtok ou leLinha falhem */;
	/*verificar se o codigo é valido*/
	produto = criaProduto(codigo);
	free(codigo);

	printf("Insira a filial que pretende consultar:");
	filial = leInt();
	indexP = indexN = 0;	
	if(filial > 0 && filial <= N_FILIAIS){
		printf("bla\n");
		clientes = todosClientes(catClientes, &nClientes);
		/* ver se o tamanho esta certo */
		quemComprouN = malloc(sizeof(char *) * nClientes); 	
		quemComprouP = malloc(sizeof(char *) * nClientes); 	
		for(i = 0; i < nClientes; i++){
			comprou(filiais[filial], clientes[i], produto, &comprouN, &comprouP); 
			if(comprouN){
				quemComprouN[indexN++] = obterCodigoCliente(clientes[i]);
			}
			if(comprouP){
				quemComprouN[indexP++] = obterCodigoCliente(clientes[i]);
			}
			apagaCliente(clientes[i]);
		}
		compraramN = criaLStrings(indexN, quemComprouN);
		compraramP = criaLStrings(indexP, quemComprouP);
		
		do {
			printf("%d clientes compraram o produto em modo normal\n", indexN); 
			printf("%d clientes compraram o produto em Promocao\n", indexP); 

			printf("Insira o modo que pretende consultar(P ou N) ou S para sair: ");
			/* substituir getchar por outra coisa qq */		
			c = getchar();
			if(c == 'P') navega(compraramP);
			else if(c == 'N') navega(compraramN);
		} while(c != 'S');				

		/* libertar tudo*/	
		ret = 0;
		/* limpar estruturas */
			
	}else{
		printf("Filial inválida\n");
		ret = 1;
	}	
	return ret;
}

static int query9( /* faltam os args */)
{
	return 0;
}

static int query10( /* faltam os args */)
{
	return 0;	
}

static int query11()
{
	return 0;
}

/* Devolve TRUE se o cliente passado como parâmetro nao comprou
 * em nenhuma das filiais. Caso contrário, é devolvido FALSE. */
static bool clienteNaoComprou(Cliente c){
	int i;
	bool r = TRUE;

	for(i = 1; r && i <= N_FILIAIS; i++){
		r = clienteComprouNaFilial(filiais[i], c) == FALSE;
	}
	return r;
}

/* !Falta o tratamento de erros */
static int query12()
{
	int nClientesNaoCompr = 0;
	int nProdsNaoVendidos;

	nClientesNaoCompr = totalClientes(catClientes) - quantosClientesCompraram(filialGlobal);
	nProdsNaoVendidos = quantosNaoComprados(faturacaoGlobal);
	printf("Número de clientes que não compraram: %d\n", nClientesNaoCompr);
	printf("Número de produtos não vendidos: %d\n", nProdsNaoVendidos);
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
