/* Módulo principal do projeto GereVendas 
 * 
 * Tanto a leitura, como a interpretação de comandos e invocação
 * das queries interativas são feitas neste módulo.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "produto.h"
#include "catalogoProds.h"
#include "catalogoClientes.h"
#include "faturacaoGlobal.h"
#include "filial.h"
#define N_FILIAIS 3
#define BUF_SIZE 1024

static CatClientes catClientes = NULL;
static CatProds catProds = NULL;
static Filial filiais[N_FILIAIS+1] = {}; /* a cada elemento de indice i do vetor faz corresponder a filial i. */
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

#define FCLIENTES "data/Clientes.txt" /* caminho do ficheiro de clientes */
#define FPRODUTOS "data/Produtos.txt" /* caminho do ficheiro de produtos */
#define FVENDAS "data/Vendas_1M.txt"     /* caminho do ficheiro de vendas */

int interpretador();
int interpreta(char linha[]);

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

Query queries[] = {NULL, query1, query2, query3, query4, query5, query6, query7, query8,
		     query9, query10, query11, query12, sair};


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
		"Listar clientes que não compraram e produtos não vendidos",
		"Sair"
};

int main()
{
	int r = interpretador();
	return r;
}

int interpretador()
{
	int r = CONTINUAR;
	char linha[MAXLINHA];

	do{
		imprimeOpcoes(opcoes);
		r = (fgets(linha, MAXLINHA, stdin) == NULL) ? SAIR : interpreta(linha); /* se falhar deve-se sair? */
	}while(r == CONTINUAR || r == CMD_INVAL); /* enquanto não houver erro ou ordem para sair */
	return r;
}

/* NOTA: Falta completar esta função */
int interpreta(char linha[])
{
	char *tmp;
	/*long i = strtol(linha, &tmp, 10); */
	int i = atoi(linha);
	int r;

	if(/* *tmp == '\0' && */ i > 0 && i <= N_OPCOES){ /* o utilizador introduziu um comando válido */
		queries[i]();			
		r = i == N_OPCOES? SAIR : CONTINUAR; /* se for inserida a ultima opção, o programa deve sair */
	}else{
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
}

/* Mensagem de opção inválida */
static void opcaoInvalida(char opcao[])
{
	fprintf(stderr, "A opção '%s' é inválida\n\n", opcao);
}

static FILE * perguntaAbreFicheiro(char * ficheiroPadrao, char buf[], int tamanhoBuffer, char * tipoDeElems){
	int i;
	FILE * fp;
	char * caminho;
	printf("Insira o caminho do ficheiro de %s (Enter para abrir ficheiro padrao):", tipoDeElems);	
	fgets(buf, tamanhoBuffer, stdin);
	for(i = 0; buf[i] && isblank(buf[i]); i++)
		;
	if(buf[i] != '\0' && buf[i] != '\n') 
		caminho = buf;
	else caminho = ficheiroPadrao; 	
	fp = fopen(caminho, "r");
	if(fp == NULL) fprintf(stderr, "Nao foi possivel abrir o ficheiro %s\n", caminho);
	return fp;	
}

#define LIDO_SUCESSO 0
int leCatalogoProdutos(){
	Produto p;
	FILE * fp;	
	char buf[BUF_SIZE];

	fp = perguntaAbreFicheiro(FPRODUTOS, buf, MAX_CODIGO_PROD, "produtos");
	if(fp == NULL) return ERRO;
	while(fgets(buf, BUF_SIZE, fp)){
		p = criaProduto(buf);
		if(p == NULL)return ERRO; 
		insereProduto(catProds, p); /*inserir tratamento de erros */
		/*registaProduto(faturacaoGlobal, p);*/
		removeProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
	}
	fclose(fp);
	return LIDO_SUCESSO;
}

int leCatalogoClientes(){
	FILE * fp;	
	char buf[MAX_CODIGO_CLIENTE];
	Cliente c;

	fp = perguntaAbreFicheiro(FCLIENTES, buf, MAX_CODIGO_CLIENTE, "clientes");
	if(fp == NULL) return ERRO;

	while(fgets(buf, BUF_SIZE, fp)){
		c = criaCliente(buf);
		if(c == NULL) return ERRO;
		insereCliente(catClientes, c); /*mudar nome para ficar evidente que insere num catalogo */
		/*registaNovoCliente(FILIAL_GLOBAL, c);*/
		removeCliente(c);
	}
	fclose(fp);

	return LIDO_SUCESSO;
}


#define GET (strtok(NULL," "))

int carregaVendasValidas(){
	char buf[BUF_SIZE];
	Cliente c;
	Produto p;
	FILE * fp; 

	/* parseVenda */
	char * codigoProduto, * codigoCliente;
	double preco;
	TipoVenda tipoVenda;
	int mes, nfilial, nUnidades;
	

	fp = perguntaAbreFicheiro(FVENDAS, buf, BUF_SIZE, "vendas");
	if(fp == NULL) return ERRO;
	
	while(fgets(buf, BUF_SIZE, fp)){
		/* ver codigo de tratamento de erros */
		codigoProduto = strtok(buf, " ");
		preco = atof(GET);
		nUnidades = atoi(GET);
		tipoVenda = GET[0] == 'P' ? P : N;
		codigoCliente = GET;
		mes = atoi(GET);
		nfilial = atoi(GET);
		
	}
	fclose(fp);
	return LIDO_SUCESSO;
}

/* alterar para inserir os caminhos dos ficheiros */
static int query1()
{
	int resL1, resL2, resL3, i;
	catProds = criaCatProds();
	catClientes = criaCatClientes();	
	/*faturacaoGlobal = criaFaturacaoGlobal();*/


	for(i = 1; i <= N_FILIAIS; i++)    /* o elemento 0 das filiais contem informação total relatvia Às compras */
		filiais[i] = criaFilial(); /* de todos os clientes nas filiais, útil para otimizar queries */
		
	resL1 = leCatalogoProdutos();		
	resL2 = leCatalogoClientes();
	/* Le o catalogoClientes  - passar para função separada*/	
		/* introduzir melhorias para filial total -> muito mais rapido */
	
	return 0;
}

static int query2( /* faltam os args */)
{
	return 0;
}

static int query3( /* faltam os args */)
{
	return 0;
}

static int query4( /* faltam os args */)
{
	return 0;
}

static int query5( /* faltam os args */)
{
	return 0;
}

static int query6( /* faltam os args */)
{
	return 0;
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
