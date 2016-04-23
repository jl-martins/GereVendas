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
static void resultadosGlobaisQuery3(FatProdMes);
static void resultadosFiliaisQuery3(FatProdMes);
/* Funções de leitura */
static char obterModoRes();

static void perguntaPag(LStrings lStr);
static void apresentaPag(Pagina pag);
static void imprimeInformacaoLStrings(LStrings);
/* Funções auxiliares da query1 */
static void apagaEstruturas();
static int criaEstruturas();
static int leFicheiros();
/* Função invocada imediatamente antes de sair */
static int sair();

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
	char buffer[MAX_BUFFER_INTERP];

	do{
		system(CLEAR);
		imprimeOpcoes(opcoes);
		leLinha(buffer, MAX_BUFFER_INTERP, stdin);
		r = (buffer == NULL) ? SAIR : interpreta(buffer);
		if(r == ERRO_MEM)
			MSG_ERRO("Não foi possível alocar memória\n");
	}while(r != SAIR); /* enquanto não houver erro ou ordem para sair */
	
	return r;
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
		r = SAIR;
	else{
		opcaoInvalida(linha);
		r = CMD_INVAL;
	}
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

/* Mensagem de opção inválida */
static void opcaoInvalida(const char opcao[])
{
	fprintf(stderr, "A opção '%s' é inválida\n\n", opcao);
	ENTER_PARA_CONTINUAR();
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
			apagaPag(pag);
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
		fprintf(stderr, "Nao foi possivel abrir o ficheiro %s\n", caminho);
	else
		printf("A ler o ficheiro: %s\n", caminho);
	return fp;	
}

int leCatalogoProdutos()
{
	time_t inicio, fim;
	FILE* fp;
	int quantos = 0;
	char nomeFich[MAX_NOME_FICH];
	char codigoProd[MAX_CODIGO_PROD];
	Produto p;
	CatProds novoCatP;
	FaturacaoGlobal novaFatG;

	fp = perguntaAbreFicheiro(FPRODUTOS, nomeFich, "produtos");
	if(fp == NULL)
		return ERRO_FICH;
	
	inicio = time(NULL); 
	while(leLinha(codigoProd, MAX_CODIGO_PROD, fp)){ /* leLinha() já limpa a linha lida */
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
		apagaProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
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
	char codigoCliente[MAX_CODIGO_CLIENTE];
	Cliente c;
	CatClientes novoCatC;

	fp = perguntaAbreFicheiro(FCLIENTES, nomeFich, "clientes");
	if(fp == NULL)
		return ERRO_FICH;
	
	inicio = time(NULL); 
	while(leLinha(codigoCliente, MAX_CODIGO_CLIENTE, fp)){ /* leLinha() já limpa o(s) caratere(s) de newline */
		c = criaCliente(codigoCliente);
		if(c == NULL) /* falha de alocação ao criar o cliente */
			return ERRO_MEM;
		novoCatC = insereCliente(catClientes, c);
		if(novoCatC == NULL){
			apagaCliente(c);
			return ERRO_MEM;
		}
		else
			catClientes = novoCatC;
		apagaCliente(c);
		++quantos;
	}
	fim = time(NULL);
	IMPRIME_TEMPOS("Leitura de Clientes", difftime(fim,inicio));
	fclose(fp);
	return quantos;
}

#define GET strtok(NULL," \r\n");
#define MAX_UNIDADES 200
#define MAX_PRECO 999.99

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
	if(produto == NULL) return ERRO_MEM;

	it = GET;
	preco = atof(it);

	it = GET;
	unidades = atoi(it);

	it = GET;
	tipoVenda = it[0] == 'P' ? P : N;

	it = GET;
	cliente = criaCliente(it);
	if(cliente == NULL) {
		apagaProduto(produto);
		return ERRO_MEM;
	}

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
		/* possivel causa de leak se falharem alocaçoes */
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

/* alterar para inserir os caminhos dos ficheiros */
/* se necessaro, inserir FilialTotal ou guardar informação relativa a todos os meses na filial*/
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
				apagaFilial(filiais[i]);
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
			apagaLStrings(lProdsLetra);
		}
		else
			r = ERRO_MEM;
	}
	else{
		fprintf(stderr, "O caratere '%c' é inválido\n", letra);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	return r;
}

static char obterModoRes()
{	
	char c;

	printf("Resultados globais[G] ou por filial[F]? ");
	c = leChar();
	return toupper(c);
}

static int query3()
{	
	char modo;
	char codigoProd[MAX_CODIGO_PROD];
	Produto p;
	FatProdMes fProdMes;
	int mes, r = CONTINUAR;

	printf("Código de produto: ");
	if(leLinha(codigoProd, MAX_CODIGO_PROD, stdin) == NULL)
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
			fprintf(stderr, "O mês '%d' é inválido\n", mes);
			r = INPUT_INVAL;
		}
	}
	else{ /* o código de produto introduzido é inválido */
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigoProd);
		r = INPUT_INVAL;
	}
	apagaProduto(p); /* já não precisamos do produto */
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

#define IMPRIME_SEPARADOR printf("----------------------------------------------------------------\n");
static int query5()
{
	char codigoCliente[MAX_CODIGO_CLIENTE];
	int r = CONTINUAR;
	Cliente cliente;

	printf("Insira um código de cliente: ");
	if(leLinha(codigoCliente, MAX_CODIGO_CLIENTE, stdin) == NULL)
		return INPUT_INVAL;

	if((cliente = criaCliente(codigoCliente)) == NULL) /* falha de alocação */
		return ERRO_MEM;
	
	if(existeCliente(catClientes, cliente)){
		int i, j, totalMes;
		int* comprasPorFilial[N_FILIAIS+1]; /* ao indice i (!= 0), correspondem as vendas na filial i */

		for(i = 1; i <= N_FILIAIS; i++)
			comprasPorFilial[i] = unidadesClientePorMes(filiais[i], cliente); 	
		/* ver o que acontece se for NULL */	
		IMPRIME_SEPARADOR;
		printf("|  Meses  |");
		for(i = 1; i <= N_FILIAIS; i++)
			printf("|  Filial %d  |", i);	
		printf("|  Total  |\n");
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
		for(i = 1; i <= N_FILIAIS; ++i)
			free(comprasPorFilial[i]);
	}
	else{ /* o código de cliente é inválido */
		fprintf(stderr, "O código de cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
	}
	apagaCliente(cliente);
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
			codigosClientes[j] = obterCodigoCliente(clientes[i]);
			if(codigosClientes[j] == NULL){ /* falha de alocação em obterCodigoCliente() */
				for(; j >= 0; j--)
					free(codigosClientes[j]);
				free(codigosClientes);
				return ERRO_MEM;
			}	
			j++;
		}
		apagaCliente(clientes[i]);	
	}
	free(clientes);

	lista = criaLStrings(j, codigosClientes);
	apagaArray((void **) codigosClientes, j, free); /* os códigos de clientes já estão na LStrings 'lista' */
	if(lista){
		navega(lista); 
		lista = apagaLStrings(lista);
		return CONTINUAR;
	}
	return ERRO_MEM;
}

int query8(){
	int i, indexP, indexN, filial, nClientes, r = CONTINUAR;
	char c;
	char codigoProd[MAX_CODIGO_PROD];
	char** quemComprouN, **quemComprouP;
	bool comprouN, comprouP;
	Cliente* clientes;
	Produto produto;
	LStrings compraramN, compraramP;

	printf("Insira o código de produto: "); /* falta validar o produto */
	if(leLinha(codigoProd, MAX_CODIGO_PROD, stdin) == NULL)
		return INPUT_INVAL;

	produto = criaProduto(codigoProd);
	if(produto == NULL) return ERRO_MEM;

	if(existeProduto(catProds, produto)){
		printf("Insira a filial que pretende consultar: ");
		filial = leInt();
		indexP = indexN = 0;	
		if(FILIAL_VALIDA(filial)){
			nClientes = totalClientes(catClientes);
			clientes = todosClientes(catClientes);
			if(clientes == NULL){
				apagaProduto(produto);
				return ERRO_MEM;
			}
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
			apagaArray((void **) quemComprouN, indexN, free);
			apagaArray((void **) quemComprouP, indexP, free);

			if(compraramP == NULL || compraramN == NULL){
				apagaLStrings(compraramP);
				apagaLStrings(compraramN);
				return ERRO_MEM;
			}
			
			do {
				printf("%d clientes compraram o produto em modo Normal\n", indexN); 
				printf("%d clientes compraram o produto em Promoção\n", indexP); 

				printf("Insira o modo que pretende consultar (N ou P) ou S para sair: ");
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
			MSG_ERRO("Filial inválida\n");
			r = INPUT_INVAL;
		}
	}
	else{
		fprintf(stderr, "O código de produto '%s' não consta no catálogo de produtos\n", codigoProd);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	apagaProduto(produto);
	return r;
}

static int query9()
{	
	int mes, r = CONTINUAR;
	char codigoCliente[MAX_CODIGO_CLIENTE];
	Cliente c;

	printf("Introduza o código do cliente: ");
	if(leLinha(codigoCliente, MAX_CODIGO_CLIENTE, stdin) == NULL)
		return INPUT_INVAL;

	c = criaCliente(codigoCliente);
	if(c == NULL) /* falha de alocação */
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
			ENTER_PARA_CONTINUAR();
		}
	}
	else{
		fprintf(stderr, "O cliente '%s' não consta no catálogo de clientes\n", codigoCliente);
		r = INPUT_INVAL;
		ENTER_PARA_CONTINUAR();
	}
	apagaCliente(c);
	return r;
}


static int query10()
{
	/* fazer tabela em vez de LString */
	int N, i, filial, nClientes, nUnidades ;
	LStrings resultados[N_FILIAIS+1] = {NULL};
	char ** produtos, **imprimir;
	char * linha;

	printf("Quantos produtos pretende consultar? ");
	N = leInt();

	do{
		printf("Existem resultados para %d filiais. "
			   "Insira o numero da filial ou %d para sair\n>>> ", N_FILIAIS, N_FILIAIS+1);
		filial = leInt();
		if(FILIAL_VALIDA(filial)){
			if(resultados[filial] == NULL){ /* ainda não calculamos a LString com os resultados da filial pedida */
				imprimir = malloc(sizeof(char *) * N);
				if(imprimir == NULL)
					return ERRO_MEM;

				produtos = NmaisVendidosFilial(faturacaoGlobal, N, filial);
				if(produtos == NULL){ /* falha de alocação */
					free(imprimir);	
					return ERRO_MEM;		
				}
				
				/* N = min(N, ...) */
				/* preparar para o caso do N ser maior que o numero de produtos vendidos na filial*/	
				for(i = 0; i < N; i++){ /* cria as linhas a introduzir na LStrings */
					linha = malloc(sizeof(char *) * (MAX_BUFFER_VENDAS));	
					if(linha == NULL){
						apagaArray((void **) imprimir, --i, free);
						apagaArray((void **) produtos, N, free);
						/* falta libertar produtos */		
						return ERRO_MEM;
					}
					nClientes = numeroClientesCompraramProduto(filiais[filial], produtos[i], &nUnidades);
					sprintf(linha, "%3s     #Clientes: %5d     #Unidades Vendidas: %8d", produtos[i], nClientes, nUnidades);
					imprimir[i] = linha;
				}
				resultados[filial] = criaLStrings(N, imprimir);
				apagaArray((void **) imprimir, N, free);
				apagaArray((void **) produtos, N, free);
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
	char codigoCliente[MAX_CODIGO_CLIENTE];

	printf("Introduza o código do cliente: ");
	if(leLinha(codigoCliente, MAX_CODIGO_CLIENTE, stdin) == NULL)
		return INPUT_INVAL;

	c = criaCliente(codigoCliente);
	if(c == NULL) /* falha de alocação */
		return ERRO_MEM;

	if(existeCliente(catClientes, c)){
		char** top3 = tresProdsEmQueMaisGastou(filialGlobal, c);
		if(top3){
			int i;
			printf("Códigos dos 3 produtos em que o cliente %s gastou mais dinheiro\n", codigoCliente);
			for(i = 0; top3[i] && i < 3; ++i)
				printf("%dº: %s\n", i+1, top3[i]);
			apagaArray((void **) top3, 3, free);
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
	apagaCliente(c);
	return r;
}

static int query12()
{
	int nClientesNaoCompr = 0;
	int nProdsNaoVendidos;

	nClientesNaoCompr = totalClientes(catClientes) - quantosClientesCompraram(filialGlobal);
	nProdsNaoVendidos = quantosNaoComprados(faturacaoGlobal);
	if(nProdsNaoVendidos == -1) /* falha de alocação em quantosNaoComprados() */
		return ERRO_MEM;
	
	printf("Número de clientes que não compraram: %d\n", nClientesNaoCompr);
	printf("Número de produtos não vendidos: %d\n", nProdsNaoVendidos);
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
