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

int interpretador();
int interpreta(char linha[]);

void apresentaPag(ConjuntoProds);
void navega(ConjuntoProds);

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
	int i = atoi(linha);
	int r;

	if(i > 0 && i <= N_OPCOES){ /* o utilizador introduziu um comando válido */
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

static FILE * perguntaAbreFicheiro(char * ficheiroPadrao, char buf[BUF_SIZE], char * tipoDeElems){
	int i;
	FILE * fp;
	char * caminho;
	printf("Insira o caminho do ficheiro de %s (Enter para abrir ficheiro padrao):", tipoDeElems);	
	fgets(buf, BUF_SIZE, stdin);
	for(i = 0; buf[i] && isspace(buf[i]); i++)
		;
	if(buf[i] != '\0' && buf[i] != '\n') 
		caminho = buf;
	else caminho = ficheiroPadrao; 	
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
		insereProduto(catProds, p); /*inserir tratamento de erros */
		registaProduto(faturacaoGlobal, p);
		apagaProduto(p); /*sao inseridas copias pelo que o original deve ser apagado*/
	}
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
		/*registaNovoCliente(FILIAL_GLOBAL, c);*/
		apagaCliente(c);
	}
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
	   existeCliente(catClientes, cliente) &&
	   unidades > 0 && unidades <= MAX_UNIDADES &&
	   mes > 0 && mes < 13 &&
	   preco >= 0 && preco <= 999.99 
	   && nfilial > 0 && nfilial <= N_FILIAIS)
	{
			registaCompra(filiais[nfilial], cliente, produto, mes, tipoVenda, unidades, preco);
			faturacaoGlobal = registaVenda(faturacaoGlobal, produto, preco, unidades, tipoVenda, nfilial, mes);
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
	int valida, validadas = 0;

	fp = perguntaAbreFicheiro(FVENDAS, buf, "vendas");
	if(fp == NULL) return ERRO;
	
	while(fgets(buf, BUF_SIZE, fp)){
		validadas += insereSeValida(buf);
	}
	
	fclose(fp);
	printf("Vendas Validadas: %d\n", validadas);
	return 1; /* mudar valor */
}

/* alterar para inserir os caminhos dos ficheiros */
/* se necessaro, inserir FilialTotal ou guardar informação relativa a todos os meses na filial*/
static int query1()
{
	/* apaga os dados de uma execuçao anterior do programa */
	int resL1, resL2, resL3, i;
	/*catProds = apagaCatProds(catProds);*/
	/*catClientes = apagaCatClientes(catClientes);*/
	/*faturacaoGlobal = apagaFaturacaoGlobal(faturacaoGlobal);
	
	for(i = 1; i <= N_FILIAIS; i++)    
		filiais[i] = apagaFilial(filiais[i]); */
	/*ver verificacao de erros */
	
	catProds = criaCatProds();
	catClientes = criaCatClientes();	
	faturacaoGlobal = criaFaturacaoGlobal();

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
		ConjuntoProds conjP;

		printf("Introduza a 1ª letra dos códigos de produto que pretende consultar: ");
		letra = toupper(getchar());
		conjP = prodsPorLetra(catProds, letra);

		if(conjP){
			navega(conjP);
			apagaConjuntoProds(conjP);
		}
		else
			erro = 1;
	}

	return erro;
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

#define MAXBUF 8
#define NEXT 1
#define PREV 2
#define GOTO 3
#define FST 4
#define LST 5

void navega(ConjuntoProds conj)
{
	int exit = 0;
	int err = 0;
	char linha[MAXBUF]; int op;

	while(!exit)
	{
		/*Ver se está em windows ou linux*/
		if(system("clear") == -1)
			system("cls");
		printf("Column1    Column2    Column3    Column4\n");
		apresentaPag(conj);
		if(err){ printf("Erro: Página inexistente!\n"); err = 0; }
		printf("(%d/%d): NEXT(1) | PREV(2) | GOTO(3) | FIRST(4) | LAST(5) | SAIR(0)\n", obterPag(conj), obterMaxPag(conj));
		
		fgets(linha, MAXBUF, stdin);
		op = atoi(linha);

		switch(op)
		{
			case NEXT: nextPage(conj); break;
			case PREV: prevPage(conj); break;
			case GOTO: 
				fgets(linha, MAXBUF, stdin);
				op = atoi(linha);
				err = goToPage(conj, op);
				break;
			case FST: fstPage(conj); break;
			case LST: lastPage(conj); break;
			case SAIR: exit = 1;
		}
	}
}

void apresentaPag(ConjuntoProds conjP)
{
	char** conj = obterCodigosPPag(conjP);
	int i = 0;
	int f = obterIndiceFinal(conjP) - obterIndice(conjP);

	if(conj)
	{
		while(i < f)
		{
			if(i < f && conj[i]) printf("%7s", conj[i++]);	
			if(i < f && conj[i]) printf("%11s", conj[i++]);
			if(i < f && conj[i]) printf("%11s", conj[i++]);
			if(i < f && conj[i]) printf("%11s", conj[i++]);
			printf("\n");
		}
	}

	for(i = 0; i < f; i++)
		free(conj[i]);
	
	free(conj); /*?acho que é preciso?*/
}
