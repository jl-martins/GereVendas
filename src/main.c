/* Módulo principal do projeto GereVendas 
 * 
 * Tanto a leitura, como a interpretação de comandos e invocação
 * das queries interativas são feitas neste módulo.
 */

#include <catalogoProds.h>
#include <catalogoClientes.h>
#include <faturacaoGlobal.h>
#include <vendasFilial.h>

/* Tamanho máximo da linha do interpretador de comandos */
#define MAXLINHA 32

#define N_QUERIES 12
#define N_ERROS 2

/* Valores de retorno de interpreta() e interpretador() */
#define ERRO -1
#define SAIR 0
#define CONTINUAR 1
#define CMD_INVAL 127

int interpretador();
int interpreta(char linha[]);

static void imprimeOpcoes(char *opcoes[N_OPCOES]);

/* Apresentação de mensagens de erro */
static void opcaoInvalida(char opcao[]);
static void erroNaoLeuFich();

/* Queries interativas */
static int query1( /* faltam os args */ );
static int query2( /* faltam os args */ );
static int query3( /* faltam os args */ );
static int query4( /* faltam os args */ );
static int query5( /* faltam os args */ );
static int query6( /* faltam os args */ );
static int query7( /* faltam os args */ );
static int query8( /* faltam os args */);
static int query9( /* faltam os args */);
static int query10( /* faltam os args */);
static int query11( /* faltam os args */);
static int query12( /* faltam os args */);

/* Função invocada imediatamente antes de sair */
static int sair( /* faltam os args */ );

/* Opções do interpretador de comandos */
static const char* opcoes[N_OPCOES] = {
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

/* Array de apontadores para as queries, para funções que apresentam
 * mensagens de erro e para a função sair() */
static int (*funcoes[N_QUERIES + N_ERROS + 1]) = {
	opcaoInvalida,
	query1,
	query2,
	query3,
	query4,
	query5,
	query6,
	query7,
	query8,
	query9,
	query10,
	query11,
	query12,
	sair,
	erroNaoLeuFich
};

int main()
{
	int r = interpretador();
	return r;
}

int interpretador()
{
	CatProds cp;
	CatClientes cc;
	FaturacaoGlobal fatG;
	VendasFilial vFiliais[3];
	
	int r = CONTINUAR;
	char linha[MAXLINHA];

	cp = cc = fatG = vFiliais = NULL;

	do{
		imprimeOpcoes(opcoes);
		r = (fgets(linha, MAXLINHA, stdin) == NULL) ? SAIR : interpreta(linha);
	}while(r > 0); /* enquanto não houver erro ou ordem para sair */

	return r;
}

/* NOTA: Falta completar esta função */
int interpreta(char linha[]){
	char *tmp;
	/* strtol() devolve um long. */ 
	long i = strtol(linha, &tmp, 10); /* !(Ver se vale a pena fazer cast pra int) */
	int r;

	if(tmp == linha){ /* o utilizador não introduziu um número */
		r = CMD_INVAL;
		i = 0; /* índice da função comandoInvalido() */
	}
	else
		r = CONTINUAR;
}

/* Imprime as opções do GereVendas */
static void imprimeOpcoes(char *opcoes[N_OPCOES])
{
	int i;

	puts("Opções:\n");
	for(i = 0; i < N_OPCOES; ++i)
		printf("%d. %s\n", i, opcoes[i]);
}

/* Mensagem de opção inválida */
static void opcaoInvalida(char opcao[]){
	fprintf(stderr, "A opção %s é inválida\n\n", opcao);
}

static int query1( /* faltam os args */){
	
}

static int query2( /* faltam os args */){
	
}

static int query3( /* faltam os args */){
	
}

static int query4( /* faltam os args */){
	
}

static int query5( /* faltam os args */){
	
}

static int query6( /* faltam os args */){
	
}

static int query7( /* faltam os args */){
	
}

static int query8( /* faltam os args */){
	
}

static int query9( /* faltam os args */){
	
}

static int query10( /* faltam os args */){
	
}

static int query11( /* faltam os args */){
	
}

static int query12( /* faltam os args */){
	
}

/* Liberta toda a memória alocada e devolve o valor SAIR para o interpretador */
static int sair( /* faltam os args */ ){
	return SAIR;
}
/* Função que apresenta uma mensagem de erro quando o utilizador 
 * tenta realizar uma query se ter lido os ficheiros de dados. */
static void erroNaoLeuFich(){
	fputs("Erro: Ainda não leu os ficheiros de dados\n"
		  "Introduza 1 e prima ENTER para o fazer\n", stderr);
}
