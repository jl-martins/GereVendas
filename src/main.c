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

#define N_FILIAIS 3
#define N_QUERIES 12
#define N_OPCOES (N_QUERIES + 1)

/* Definição do comando de limpar o ecrã com base no sistema operativo */
#ifdef _WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

/* Tamanho dos vários buffers usados */
#define MAX_BUFFER_PROD 64
#define MAX_BUFFER_CLIS 64
#define MAX_BUFFER_VENDAS 128
#define MAX_BUFFER_INTERP 128
#define MAX_NOME_FICH 256

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
#define ENTER_PARA_CONTINUAR() printf("Prima ENTER para continuar: "); leChar()
#define IMPRIME_OPCOES_NAVEGA() \
	puts("\n[1] Pag. ant.  [2] Pag. seg.   [3] Selec. pag.   [4] Prim. pag.   [5] Ult. pag  [6] Info.  [7] Sair")
#define MSG_ERRO(msg) {fputs(msg, stderr); ENTER_PARA_CONTINUAR();}
#define IMPRIME_SEPARADOR() puts("----------------------------------------------------------------");

/* Limites superiores usados na validação das vendas */
#define MAX_UNIDADES 200
#define MAX_PRECO 999.99

/* Macros de validação */
#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)
#define FILIAL_VALIDA(filial) ((filial) >= 1 && (filial) <= N_FILIAIS)
#define PRECO_VALIDO(preco) ((preco) >= 0 && preco <= MAX_PRECO)
#define NUM_UNIDS_VALIDO(nunids) ((nunids) >= 0 && nunids <= MAX_UNIDADES)

/* Imprime os tempos se estiver ativado */
#define MODO_MEDICAO_TEMPOS 0
#ifdef MODO_MEDICAO_TEMPOS 
	#define IMPRIME_TEMPOS(s, x) printf("Tempo na %s: %f\n", s, x)
	#define IMPRIME_TEMPOS_ENTER(s, x) {printf("Tempo na %s: %f\n", s, x); ENTER_PARA_CONTINUAR();}
#else
	#define IMPRIME_TEMPOS(s, x)
	#define IMPRIME_TEMPOS_ENTER(s, x) 
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
static const char* opcoes[] = {
		NULL,
		"Ler ficheiros",
		"Produtos começados por uma letra",
		"Quantidade vendida e faturação total de um produto, num dado mês",
		"Lista ordenada de códigos dos produtos que ninguém comprou",
		"Tabela com as compras efetuadas por um cliente",
		"Vendas e faturação total para um intervalo fechado de meses",
		"Lista ordenada de clientes que realizaram compras em todas as filiais",
		"Códigos e nº total de clientes que compraram um produto numa dada filial",
		"Produtos mais comprados por um cliente num dado mês",
		"Informação sobre os N produtos mais vendidos do ano",
		"Códigos dos 3 produtos em que um cliente gastou mais dinheiro",
		"Número de clientes que não compraram e de produtos não vendidos",
		"Sair"
};

void splashScreen();
int interpretador();
int interpreta(char linha[]);

/* Funções de navegação */
void navegaVarias(LStrings lStrArr[], int tamanho);
void navega(LStrings lStr);

/* Recebe um array de strings com opções. Imprime uma opção por linha */
static void imprimeOpcoes(const char* opcoes[N_OPCOES]);

/* Apresentação de mensagens de erro */
static void opcaoInvalida(const char opcao[]);

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

/* Funções auxiliares das queries */

/* Funções auxiliares da query1 */
static void apagaEstruturas();
static int criaEstruturas();
static int leFicheiros();

/* Funções auxiliares da query3 */
static void resultadosGlobaisQuery3(FatProdMes);
static void resultadosFiliaisQuery3(FatProdMes);

/* Função auxiliar da query10 */
static LStrings criaResultadosQuery10(int N, int filial);

/* Funções de leitura */
static char obterModoRes();
/* Funções auxiliares da navegação */
static void perguntaPag(LStrings lStr);
static void apresentaPag(Pagina pag);
static void imprimeInformacaoLStrings(LStrings);

/* Função invocada imediatamente antes de sair */
static int sair();

Query queries[] = {NULL, query1, query2, query3, query4, query5, query6,
					query7, query8, query9, query10, query11, query12, sair};

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
	puts(" _____               _   _                _");           
	puts("|  __ \\             | | | |              | |");          
	puts("| |  \\/ ___ _ __ ___| | | | ___ _ __   __| | __ _ ___"); 
	puts("| | __ / _ | '__/ _ | | | |/ _ | '_ \\ / _` |/ _` / __|");
	puts("| |_\\ |  __| | |  _ \\ \\_/ |  __| | | | (_| | (_| \\__ \\");
 	puts(" \\____/\\___|_|  \\___|\\___/ \\___|_| |_|\\__,_|\\__,_|___/");
	printf("\n%43s", linha);
	leChar();
}

int interpretador()
{
	int r = CONTINUAR;
	char buffer[MAX_BUFFER_INTERP];

	do{
		system(CLEAR);
		imprimeOpcoes(opcoes);
		leLinha(buffer, MAX_BUFFER_INTERP, stdin);
		r = (buffer == NULL) ? SAIR : interpreta(buffer);
		if(r == ERRO_MEM)
			MSG_ERRO("Não foi possível alocar memória\n");
	}while(r != SAIR); /* enquanto não houver ordem para sair */
	
	return r;
}

/* Imprime as opções do GereVendas */
static void imprimeOpcoes(const char* opcoes[N_OPCOES])
{
	int i;

	puts("Opções:\n");
	for(i = 1; i <= N_OPCOES; ++i)
		printf("%2d) %s\n", i, opcoes[i]);
	printf("\n>>> ");
}

int interpreta(char linha[])
{
	int i, iPrimNaoEsp; /* iPrimNaoEsp - índice do primeiro carater da linha que não é um espaço */
	int r = CONTINUAR;

	iPrimNaoEsp = avancaEspacosInicio(linha);
	if(linha[iPrimNaoEsp] == '\0') /* foi introduzida uma linha em branco */
		return CONTINUAR;
	
	i = atoi(linha);
	if(i > 0 && i < N_OPCOES){
		if(i == 1)
			r = queries[1]();
		else if(fichCarregados == FALSE) /* i > 1 && i < N_OPCOES */
			MSG_ERRO("Erro: Precisa de ler os ficheiros de dados\n")
		else
			r = queries[i]();
	}
	else if(i == N_OPCOES)
		r = sair();
	else{
		opcaoInvalida(linha);
		r = CMD_INVAL;
	}
	return r;
}

/* Mensagem de opção inválida */
static void opcaoInvalida(const char opcao[])
{
	fprintf(stderr, "A opção '%s' é inválida\n\n", opcao);
	ENTER_PARA_CONTINUAR();
}

/* Dado um array de LStrings, permite ao utilizador escolher em qual LStrings pretende navegar. */
void navegaVarias(LStrings lStrArr[], int tamanho)
{	
	int i;
	bool sair = FALSE;

	do{
		printf("-> Existem resultados para %d filiais.\n", tamanho);
		printf("Introduza o número da filial que pretende "
			   "ou %d se pretender sair\n\n>>> ", tamanho + 1);
		i = leInt();
		if(i > 0 && i <= tamanho) /* a indexação de lStrArr[] começa em 1 */
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
			pag = apagaPag(pag);
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

static FILE* perguntaAbreFicheiro(char* ficheiroPadrao, char buffer[MAX_NOME_FICH], char* tipoDeElems)
{
	FILE* fp;
	char* caminho;
	int iPrimNaoEsp; /* índice do primeiro caratere de buffer que não é um espaço */

	printf("Insira o caminho do ficheiro de %s (Enter para abrir ficheiro padrao): ", tipoDeElems);	
	if(leLinha(buffer, MAX_NOME_FICH, stdin) == NULL) /* chegamos ao EOF */
		return NULL;

	iPrimNaoEsp = avancaEspacosInicio(buffer);
	if(buffer[iPrimNaoEsp] == '\0')
		caminho = ficheiroPadrao;
	else
		caminho = &buffer[iPrimNaoEsp];

	fp = fopen(caminho, "r");
	if(fp == NULL)
		fprintf(stderr, "Nao foi possivel abrir o ficheiro '%s'\n", caminho);
	else
		printf("A ler o ficheiro: '%s'\n", caminho);
	return fp;	
}

int leCatalogoProdutos()
{
	time_t inicio, fim;
	FILE* fp;
	int quantos = 0;
	char nomeFich[MAX_NOME_FICH];
	char codigoProd[MAX_BUFFER_PROD];
	Produto p;
	CatProds novoCatP;
	FaturacaoGlobal novaFatG;

	fp = perguntaAbreFicheiro(FPRODUTOS, nomeFich, "produtos");
	if(fp == NULL)
		return ERRO_FICH;
	
	inicio = time(NULL); 
	while(leLinha(codigoProd, MAX_BUFFER_PROD, fp)){ /* leLinha() já limpa o(s) caratere(s) de newline */
		p = criaProduto(codigoProd);
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
		p = apagaProduto(p); /* sao inseridas copias pelo que o original deve ser apagado*/
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("Leitura de Produtos", difftime(fim,inicio));
	fclose(fp);
	return quantos;
}

int leCatalogoClientes()
{
	time_t inicio, fim;
	FILE* fp;
	int quantos = 0;
	char nomeFich[MAX_NOME_FICH];
	char codigoCliente[MAX_BUFFER_CLIS];
	Cliente c;
	CatClientes novoCatC;

	fp = perguntaAbreFicheiro(FCLIENTES, nomeFich, "clientes");
	if(fp == NULL)
		return ERRO_FICH;
	
	inicio = time(NULL); 
	while(leLinha(codigoCliente, MAX_BUFFER_CLIS, fp)){ /* leLinha() já limpa o(s) caratere(s) de newline */
		c = criaCliente(codigoCliente);
		if(c == NULL) /* falha de alocação ao criar o cliente */
			return ERRO_MEM;
		novoCatC = insereCliente(catClientes, c);
		if(novoCatC == NULL){
			c = apagaCliente(c);
			return ERRO_MEM;
		}
		else
			catClientes = novoCatC;
		c = apagaCliente(c);
		++quantos;
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("Leitura de Clientes", difftime(fim,inicio));
	fclose(fp);
	return quantos;
}
/* a função leLinha() já retira o carater de 'newline' da linha, logo podemos usar
 * strok() com o delimitador " ", ao processar as linhas do ficheiro de vendas */ 
#define GET strtok(NULL," ");

/* Dada uma linha com informação da venda, a função processa a informação da venda e, se for válida, regista a compra */
int insereSeValida(char linhaVenda[MAX_BUFFER_VENDAS])
{
	Cliente cliente;
	Produto produto;
	int unidades, mes, nfilial,  quantos = 0;
	double preco;
	TipoVenda tipoVenda;
	char* it;

	it = strtok(linhaVenda, " ");
	produto = criaProduto(it);
	if(produto == NULL) /* falha de alocação */
		return ERRO_MEM;

	it = GET;
	preco = atof(it);

	it = GET;
	unidades = atoi(it);

	it = GET;
	tipoVenda = it[0] == 'P' ? P : N;

	it = GET;
	cliente = criaCliente(it);
	if(cliente == NULL) {
		produto = apagaProduto(produto);
		return ERRO_MEM;
	}

	it = GET;
	mes = atoi(it);

	it = GET;
	nfilial = atoi(it);
			
	if(existeProduto(catProds, produto) && 
	   existeCliente(catClientes, cliente)  &&  
	   NUM_UNIDS_VALIDO(unidades) &&
	   MES_VALIDO(mes) &&
	   PRECO_VALIDO(preco) && 
	   FILIAL_VALIDA(nfilial))
	{
		filiais[nfilial] = registaCompra(filiais[nfilial], cliente, produto, mes, tipoVenda, unidades, preco);
		filialGlobal = registaCompra(filialGlobal, cliente, produto, mes, tipoVenda, unidades, preco);
		faturacaoGlobal = registaVenda(faturacaoGlobal, produto, preco, unidades, tipoVenda, nfilial, mes) ;
		quantos = 1;
	}
	cliente = apagaCliente(cliente);
	produto = apagaProduto(produto);
	return quantos;
}

#undef GET

int carregaVendasValidas()
{
	time_t inicio, fim;
	FILE* fp;
	int validas = 0;
	char linhaVenda[MAX_BUFFER_VENDAS];
	
	fp = perguntaAbreFicheiro(FVENDAS, linhaVenda, "vendas");
	if(fp == NULL)
		return ERRO_FICH;

	inicio = time(NULL);
	while(leLinha(linhaVenda, MAX_BUFFER_VENDAS, fp)){
		validas += insereSeValida(linhaVenda);
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("Leitura de Vendas", difftime(fim, inicio));
	fclose(fp);
	return validas; 
}

static int query1()
{
	int r;
	
	if(fichCarregados == TRUE){
		apagaEstruturas(); /* apaga os dados de uma execuçao anterior do programa */
		fichCarregados = FALSE;
	}
	r = criaEstruturas();
	if(r == CONTINUAR)
		r = leFicheiros();
	return r;
}

static void apagaEstruturas()
{
	int i;

	catProds = apagaCatProds(catProds);
	catClientes = apagaCatClientes(catClientes);
	faturacaoGlobal = apagaFaturacaoGlobal(faturacaoGlobal);
	for(i = 0; i <= N_FILIAIS; i++)    
		filiais[i] = apagaFilial(filiais[i]); 
}

static int criaEstruturas()
{	
	int i;

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
				filiais[i] = apagaFilial(filiais[i]);
			MSG_ERRO("Erro a criar as filiais\n");
			return ERRO_MEM;
		}
	}
	return CONTINUAR;
}

static int leFicheiros()
{
	int produtosLidos, vendasValidas, clientesLidos;

	clientesLidos = leCatalogoClientes();
	if(clientesLidos < 0){ /* erro a ler o ficheiro de clientes */
		MSG_ERRO("Erro a ler o ficheiro de clientes\n");
		return clientesLidos; /* indica o erro que ocorreu */
	}
	
	produtosLidos = leCatalogoProdutos();
	if(produtosLidos < 0){
		catClientes = apagaCatClientes(catClientes);
		MSG_ERRO("Erro a ler o ficheiro dos produtos\n");
		return produtosLidos;
	}
	
	vendasValidas = carregaVendasValidas();
	if(vendasValidas < 0){
		catClientes = apagaCatClientes(catClientes);
		catProds = apagaCatProds(catProds);
		MSG_ERRO("Erro a ler o ficheiro das vendas\n");
		return vendasValidas;
	}
	/* só chegamos aqui se todas as leituras correram bem */
	printf("Nº de clientes lidos: %d\n"
		   "Nº de Produtos lidos: %d\n"
		   "Nº de vendas validas: %d\n", clientesLidos, produtosLidos, vendasValidas);
	fichCarregados = TRUE;
	ENTER_PARA_CONTINUAR();
	return CONTINUAR;
}

static int query2()
{
	int r = CONTINUAR;
	char letra;
	LStrings lProdsLetra;

	printf("Introduza a 1ª letra dos códigos de produto que pretende consultar: ");
	letra = leChar();
	
	if(isalpha(letra)){
		letra = toupper(letra);
		lProdsLetra = prodsPorLetra(catProds, letra);

		if(lProdsLetra){
			navega(lProdsLetra);
			lProdsLetra = apagaLStrings(lProdsLetra);
		}
		else
			r = ERRO_MEM;
	}
	else if(!isspace(letra)){
		fprintf(stderr, "O caratere '%c' é inválido\n", letra);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	return r;
}

static char obterModoRes()
{	
	char c;

	printf("Resultados globais [G] ou por filial [F]? ");
	c = leChar();
	return toupper(c);
}

static int query3()
{	
	char modo;
	char codigoProd[MAX_BUFFER_PROD];
	Produto p;
	FatProdMes fProdMes;
	int mes, r = CONTINUAR;

	printf("Código de produto: ");
	if(leLinha(codigoProd, MAX_BUFFER_PROD, stdin) == NULL)
		return INPUT_INVAL;

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
					fputs("Erro: Modo inválido\nModos válidos: G | F\n", stderr);
					r = INPUT_INVAL;
					break;
			}
			fProdMes = apagaFatProdMes(fProdMes);
		}
		else{ /* o mês introduzido é inválido */
			fputs("Mês inválido. O mês deve ser um valor do intervalo [1,12]\n", stderr);
			r = INPUT_INVAL;
		}
	}
	else{ /* o código de produto introduzido é inválido */
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigoProd);
		r = INPUT_INVAL;
	}
	p = apagaProduto(p); /* já não precisamos do produto */
	ENTER_PARA_CONTINUAR();
	return r;
}

/* calcula e apresenta os resultados globais da query3 */
static void resultadosGlobaisQuery3(FatProdMes fProdMes)
{
	int totalVendas[N_TIPOS_VENDA];
	double totalFaturado[N_TIPOS_VENDA];

	totalVendas[N] = totalUnidsProdMes(fProdMes, N);
	totalVendas[P] = totalUnidsProdMes(fProdMes, P);
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
	int* unidsFilial[N_TIPOS_VENDA];
	double* faturacaoFilial[N_TIPOS_VENDA];

	unidsFilial[N] = unidsPorFilialProdMes(fProdMes, N);
	unidsFilial[P] = unidsPorFilialProdMes(fProdMes, P);
	faturacaoFilial[N] = faturacaoPorFilialProdMes(fProdMes, N);
	faturacaoFilial[P] = faturacaoPorFilialProdMes(fProdMes, P);

	for(filial = 1; filial <= N_FILIAIS; ++filial){
		printf(" ----------\n| Filial %d |\n ----------\n", filial);
		printf("Vendas N = %d, Vendas P = %d\n"
				"Faturação N = %.2f, Faturação P = %.2f\n\n",
				unidsFilial[N][filial], unidsFilial[P][filial],
				faturacaoFilial[N][filial], faturacaoFilial[P][filial]);
	}
	free(unidsFilial[N]); free(unidsFilial[P]);
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
			naoCompradosG = apagaLStrings(naoCompradosG);
		}
		else
			r = ERRO_MEM;
	}
	else if(modo == 'F'){
		LStrings* naoCompradosF = naoCompradosPorFilial(faturacaoGlobal);
		if(naoCompradosF){
			int i;
			system(CLEAR);
			navegaVarias(naoCompradosF, N_FILIAIS);
			for(i = 1; i <= N_FILIAIS; ++i)
				naoCompradosF[i] = apagaLStrings(naoCompradosF[i]);
			free(naoCompradosF);
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

static int query5()
{
	char codigoCliente[MAX_BUFFER_CLIS];
	int r = CONTINUAR;
	Cliente cliente;

	printf("Insira um código de cliente: ");
	if(leLinha(codigoCliente, MAX_BUFFER_CLIS, stdin) == NULL)
		return INPUT_INVAL;

	if((cliente = criaCliente(codigoCliente)) == NULL) /* falha de alocação */
		return ERRO_MEM;
	
	if(existeCliente(catClientes, cliente)){
		int i, j, totalMes;
		int* comprasPorFilial[N_FILIAIS+1]; /* ao indice i (!= 0), correspondem as vendas na filial i */

		for(i = 1; i <= N_FILIAIS; i++)
			comprasPorFilial[i] = unidadesClientePorMes(filiais[i], cliente); 	
	
		IMPRIME_SEPARADOR();
		printf("|  Meses  |");
		for(i = 1; i <= N_FILIAIS; i++)
			printf("|  Filial %d  |", i);	
		printf("|  Total  |\n");
		IMPRIME_SEPARADOR();

		for(j = 1; j < 13; j++){
			totalMes = 0;
			printf("|%8d |", j);
			for(i = 1 ; i <= N_FILIAIS; i++){
				printf("|%11d |", comprasPorFilial[i][j]);
				totalMes += comprasPorFilial[i][j];
			}
			printf("|%8d |\n", totalMes);	
			IMPRIME_SEPARADOR();
		}
		for(i = 1; i <= N_FILIAIS; ++i)
			free(comprasPorFilial[i]);
	}
	else{ /* o código de cliente é inválido */
		fprintf(stderr, "O código de cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
	}
	cliente = apagaCliente(cliente);
	ENTER_PARA_CONTINUAR();
	return r;
}

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
		printf("Total de vendas = %d\n"
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
	int r = CONTINUAR;
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
			clientes[i] = apagaCliente(clientes[i]);
		return ERRO_MEM;
	}

	for(i = 0, j = 0; i < nClientes; i++){
		if(comprouTodasFiliais(clientes[i])){
			codigosClientes[j] = obterCodigoCliente(clientes[i]);
			if(codigosClientes[j] == NULL){ /* falha de alocação em obterCodigoCliente() */
				for(; j >= 0; j--)
					free(codigosClientes[j]);
				free(codigosClientes);
				return ERRO_MEM;
			}	
			j++;
		}
		clientes[i] = apagaCliente(clientes[i]);	
	}
	free(clientes);

	lista = criaLStrings(j, codigosClientes);
	apagaArray((void **) codigosClientes, j, free); /* os códigos de clientes já estão na LStrings 'lista' */
	if(lista == NULL)
		return ERRO_MEM;

	navega(lista); 
	lista = apagaLStrings(lista);
	return r;
}

int query8(){
	int i, indexP, indexN, filial, nClientes, r = CONTINUAR;
	char c;
	char codigoProd[MAX_BUFFER_PROD];
	char** quemComprouN, **quemComprouP;
	bool comprouN, comprouP;
	time_t inicio, fim;
	Cliente* clientes;
	Produto produto;
	LStrings compraramN, compraramP;

	printf("Insira o código de produto: "); /* falta validar o produto */
	if(leLinha(codigoProd, MAX_BUFFER_PROD, stdin) == NULL)
		return INPUT_INVAL;

	produto = criaProduto(codigoProd);
	if(produto == NULL) return ERRO_MEM;

	if(existeProduto(catProds, produto)){
		printf("Insira a filial que pretende consultar: ");
		filial = leInt();
		inicio = time(NULL); /* começa a contagem do tempo */
		indexP = indexN = 0;	
		if(FILIAL_VALIDA(filial)){
			nClientes = totalClientes(catClientes);
			clientes = todosClientes(catClientes);
			if(clientes == NULL){
				produto = apagaProduto(produto);
				return ERRO_MEM;
			}

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
				clientes[i] = apagaCliente(clientes[i]);
			}
			compraramN = criaLStrings(indexN, quemComprouN);
			compraramP = criaLStrings(indexP, quemComprouP);
			apagaArray((void **) quemComprouN, indexN, free);
			apagaArray((void **) quemComprouP, indexP, free);
			free(clientes);

			if(compraramP == NULL || compraramN == NULL){
				compraramP = apagaLStrings(compraramP);
				compraramN = apagaLStrings(compraramN);
				return ERRO_MEM;
			}
			fim = time(NULL);
			IMPRIME_TEMPOS_ENTER("Query 8", difftime(fim, inicio));	
			do {
				printf("> %d cliente(s) compraram o produto em modo Normal;\n", indexN); 
				printf("> %d cliente(s) compraram o produto em Promoção;\n", indexP); 

				printf("\n-> Insira o modo que pretende consultar (N ou P) ou S para sair\n"
					   ">>> ");
				c = toupper(leChar());
				if(c == 'P')
					navega(compraramP);
				else if(c == 'N')
					navega(compraramN);
			} while(c != 'S');						

			compraramP = apagaLStrings(compraramP);
			compraramN = apagaLStrings(compraramN);
		}
		else{ /* a filial introduzida é inválida */
			MSG_ERRO("Filial inválida. \n");
			r = INPUT_INVAL;
		}
	}
	else{
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigoProd);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	produto = apagaProduto(produto);
	return r;
}

static int query9()
{	
	int mes, r = CONTINUAR;
	char codigoCliente[MAX_BUFFER_CLIS];
	Cliente c;
	time_t inicio, fim;

	printf("Introduza o código do cliente: ");
	if(leLinha(codigoCliente, MAX_BUFFER_CLIS, stdin) == NULL)
		return INPUT_INVAL;

	c = criaCliente(codigoCliente);
	if(c == NULL) /* falha de alocação */
		return ERRO_MEM;

	if(existeCliente(catClientes, c)){
		printf("Introduza o mês: "); mes = leInt();
	
		inicio = time(NULL);	
		if(MES_VALIDO(mes)){
			LStrings lStr = produtosClienteMaisComprou(filialGlobal, c, mes);
			fim = time(NULL);
			IMPRIME_TEMPOS_ENTER("Query 9", difftime(fim, inicio));
			navega(lStr);
			lStr = apagaLStrings(lStr);
		}
		else{
			fputs("Mês inválido. O mês deve ser um valor do intervalo [1,12]\n", stderr);
			r = INPUT_INVAL;
			ENTER_PARA_CONTINUAR();
		}
	}
	else{
		fprintf(stderr, "O cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	c = apagaCliente(c);
	return r;
}


static int query10()
{
	int N, filial;
	LStrings resultados[N_FILIAIS+1] = {NULL};
	time_t inicio, fim;

	printf("Quantos produtos pretende consultar? ");
	N = leInt();

	do{
		printf("Existem resultados para %d filiais. "
			   "Insira o numero da filial ou %d para sair\n>>> ", N_FILIAIS, N_FILIAIS+1);
		filial = leInt();
		if(FILIAL_VALIDA(filial)){
			inicio = time(NULL);	
			if(resultados[filial] == NULL){ /* ainda não calculamos a LString com os resultados da filial pedida */
				resultados[filial] = criaResultadosQuery10(N, filial);
				
				if(resultados[filial] == NULL){ /* falha de alocação a criar uma LStrings */
					for(filial = 1; filial <= N_FILIAIS; ++filial)
						resultados[filial] = apagaLStrings(resultados[filial]);
					return ERRO_MEM;
				}
			}
			fim = time(NULL);
			IMPRIME_TEMPOS_ENTER("Query 10", difftime(fim, inicio));
			navega(resultados[filial]);
		}
		else if(filial != N_FILIAIS + 1)
			MSG_ERRO("Opção inválida\n");
	} while (filial != N_FILIAIS + 1);
	
	for(filial = 1; filial <= N_FILIAIS; ++filial) /* liberta memória alocada para a(s) LStrings */
		resultados[filial] = apagaLStrings(resultados[filial]);
	
	return CONTINUAR;
}

static LStrings criaResultadosQuery10(int N, int filial)
{	
	int i, nClientes, nUnidades;
	char** produtos;
	char** imprimir = malloc(sizeof(char *) * N);
	LStrings lStr;

	if(imprimir == NULL)
		return NULL;

	produtos = NmaisVendidosFilial(faturacaoGlobal, N, filial);
	if(produtos == NULL){ /* falha de alocação */
		free(imprimir);	
		return NULL;		
	}
				
	for(i = 0; i < N; i++){ /* cria as linhas a introduzir na LStrings */
		char* linha = malloc(sizeof(char *) * (MAX_BUFFER_VENDAS));	
		if(linha == NULL){
			apagaArray((void **) imprimir, --i, free);
			apagaArray((void **) produtos, N, free);	
			return NULL;
		}
		nClientes = numeroClientesCompraramProduto(filiais[filial], produtos[i], &nUnidades);
		sprintf(linha, "%3d) %s | NºClientes: %5d | Qtd: %8d", i+1, produtos[i], nClientes, nUnidades);
		imprimir[i] = linha;
	}
	lStr = criaLStrings(N, imprimir);
	apagaArray((void **) imprimir, N, free);
	apagaArray((void **) produtos, N, free);
	return lStr;
}

static int query11()
{	
	int r = CONTINUAR;
	Cliente c;
	char codigoCliente[MAX_BUFFER_CLIS];
	time_t inicio, fim;

	printf("Introduza o código do cliente: ");
	if(leLinha(codigoCliente, MAX_BUFFER_CLIS, stdin) == NULL)
		return INPUT_INVAL;

	inicio = time(NULL);
	c = criaCliente(codigoCliente);
	if(c == NULL) /* falha de alocação */
		return ERRO_MEM;

	if(existeCliente(catClientes, c)){
		char** top3 = tresProdsEmQueMaisGastou(filialGlobal, c);
		if(top3){
			int i;

			printf("> Códigos dos 3 produtos em que o cliente '%s' gastou mais dinheiro\n", codigoCliente);
			for(i = 0; i < 3 && top3[i]; ++i)
				printf("%dº: %s\n", i+1, top3[i]);
			apagaArray((void **) top3, 3, free);
			fim = time(NULL);
			IMPRIME_TEMPOS("Query 11", difftime(fim, inicio));
			ENTER_PARA_CONTINUAR();
		}
		else
			r = ERRO_MEM;
	}
	else{
		fprintf(stderr, "O código de cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	c = apagaCliente(c);
	return r;
}

static int query12()
{
	int nClientesNaoCompr = 0;
	int nProdsNaoVendidos;
	time_t inicio, fim;

	inicio = time(NULL);
	nClientesNaoCompr = totalClientes(catClientes) - quantosClientesCompraram(filialGlobal);
	nProdsNaoVendidos = quantosNaoComprados(faturacaoGlobal);
	if(nProdsNaoVendidos == -1) /* falha de alocação em quantosNaoComprados() */
		return ERRO_MEM;
	
	printf("Número de clientes que não compraram: %d\n", nClientesNaoCompr);
	printf("Número de produtos não vendidos: %d\n", nProdsNaoVendidos);
	fim = time(NULL);
	IMPRIME_TEMPOS("Query 12", difftime(fim, inicio));
	ENTER_PARA_CONTINUAR();
	return CONTINUAR;
}

/* Liberta toda a memória alocada e devolve o valor SAIR para o interpretador */
static int sair()
{	
	if(fichCarregados == TRUE){
		apagaEstruturas();
		fichCarregados = FALSE;
	}
	return SAIR;
}
