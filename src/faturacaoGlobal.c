#include "faturacaoGlobal.h"
#include "avl.h"
#include <stdlib.h>
#include <string.h> /* para utilizar a função memcpy() */

/* !! Decidir onde vamos colocar estas definições */
#define N_FILIAIS 3
#define N_MESES 12

/* !! Decidir se vale a pena validar meses */
#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)

/****************************************************************************** 
 *	Estruturas de dados do módulo:											  *
 *		        														      *
 *	 _FaturacaoGlobal_______________	   __FatAnualProd________________	  *
 *	|								|	  |								 |	  *
 *	|  AVL todosProdutos;			| ==> |	Produto prod;				 |	  *
 *	|  FatMes fatMensal[N_MESES+1]; | 	  |	int totalVendas[N_FILIAIS+1];|	  *
 *	|_______________________________|	  |______________________________|	  *
 *				 															  *
 *				 ||															  *
 *				 \/															  *
 *	 _FatMes_______________________ 										  *
 *	|                              |										  *
 *	|  int totalVendas;            |										  *
 *	|  int totalFaturado;          |										  *
 *	|  AVL fatProds;               |										  *
 *	|______________________________|										  *
 *																			  *
 *				 ||															  *
 *				 \/															  *
 *																			  *
 *	 _FatProdMes____________________________________						  *
 *	|                              					|						  *
 *	|  Produto prod;               					|						  *
 *	|  int vendas[N_TIPOS_VENDA][N_FILIAIS+1]       |						  *
 *	|  double faturacao[N_TIPOS_VENDA][N_FILIAIS+1] |						  *
 *	|_______________________________________________|						  *
 *																			  *
 ******************************************************************************/

/* FatAnualProd e FatMes são tipos auxiliares (i.e.: não exportados) */
typedef struct fatAnualProd* FatAnualProd;
typedef struct fatMes* FatMes;

/* Faturação global */
struct fatGlobal{
	/* referencia todos os produtos (mesmo os não vendidos). Permite obter informações
	 * sobre as vendas anuais de cada produto, tanto globalmente como por filial */
	AVL todosProdutos;
	/* guarda, para cada mês, informação quantitiva sobre as vendas e produtos
	 * vendidos nesse mês (produtos não vendidos não são referenciados) */
	FatMes fatMensal[N_MESES+1]; 
};

/* Armazena, para um dado produto, informação sobre o seu
 * total de vendas anuais em cada uma das filiais */
struct fatAnualProd{
	Produto prod;
	int totalVendas[N_FILIAIS+1];
};

struct fatMes{
	int totalVendas;
	double totalFaturado;
	AVL fatProds;
};

/* Guarda informação sobre a faturação de um produto (num dado mês) */
struct fatProdMes{
	Produto prod;
	int vendas[N_TIPOS_VENDA][N_FILIAIS+1];
	double faturacao[N_TIPOS_VENDA][N_FILIAIS+1];
};

/* Funções privadas ao módulo */

/* Funções privadas usadas na criação de estruturas de dados */
static FatAnualProd criaFatAnualProd(Produto p);
static FatProdMes criaFatProdMes(Produto p, int quantidade, double totalFaturado,
								 TipoVenda tipo, int filial);

static void** alocaMatriz(int nlinhas, int ncolunas, size_t nbytesElem);

/* Funções utilizadas na criação de AVLs */

/* Comparação de nodos */
static int comparaFatProdMes(const void* , const void* );
static int comparaFatAnualProd(const void* , const void* );
/* Atualização de nodos */
static void atualizaFatProdMes(void *, void* );
static void atualizaFatAnualProd(void* , void* );
/* Duplicação do valor de um nodo */
static ValorNodo duplicaFatAnualProd(void* p);
static ValorNodo duplicaFatProdMes(void* p);

/* Funções que permitem obter informações
 * sobre as vendas anuais de um produto */
static int obterTotalVendasAnuaisProd(const FatAnualProd);
static bool naoComprado(const FatAnualProd);

/* Compara o nº total de vendas anuais de dois produtos */
static int comparaVendasAnuais(const void* v1, const void* v2);

/* Funções de libertação dos tipos de dados não exportados */
static void apagaFatMensalInterv(FatMes fMes[], int inicio, int fim);
static void apagaFatMes(FatMes fMes);
static void apagaNodoFatProdMes(void* p);
static void apagaFatAnualProd(void* p);

FaturacaoGlobal criaFaturacaoGlobal()
{
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatGlobal));
	
	if(fg == NULL) /* falha a alocar a struct fatGlobal */
		return NULL;

	fg->todosProdutos = criaAVLgenerica(atualizaFatAnualProd, comparaFatAnualProd, 
										duplicaFatAnualProd, apagaFatAnualProd);
	fg->fatMensal[0] = NULL;
	for(i = 1; i <= N_MESES; i++){ /* cria estruturas da faturação mensal */
		fg->fatMensal[i] = malloc(sizeof(struct fatMes));

		if(fg->fatMensal[i] == NULL){ /* falha a alocar a struct fatMes */
			/* liberta a memória que de fg->fatMensal desde a posição 1 até i-1 */
			apagaFatMensalInterv(fg->fatMensal, 1, i-1);
			return NULL;
		}
		fg->fatMensal[i]->totalVendas = 0;
		fg->fatMensal[i]->totalFaturado = 0;
		fg->fatMensal[i]->fatProds = criaAVLgenerica(atualizaFatProdMes,comparaFatProdMes,
													 duplicaFatProdMes, apagaNodoFatProdMes);
	}
	return fg;
}

/* Apaga um array de struct fatMes, desde 'inicio' até 'fim' (inclusivé) */
static void apagaFatMensalInterv(FatMes* fatMes, int inicio, int fim)
{
	int i;
	/* assume que 'inicio' e 'fim' são valores válidos */
	for(i = inicio; i <= fim; ++i)
		apagaFatMes(fatMes[i]);
}

/* Liberta a memória alocada para uma struct fatMes */
static void apagaFatMes(FatMes fMes)
{	
	apagaAVL(fMes->fatProds);
	free(fMes);
}

/* Regista um produto na faturação global, guardando-o na AVL de total de vendas
 * anuais com o total de vendas igual a 0, para cada uma das filiais */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p)
{	
	FatAnualProd fAnualProd = criaFatAnualProd(p);

	if(fAnualProd == NULL) /* falha a criar a struct fatAnualProd */
		return NULL;

	fg->todosProdutos = insere(fg->todosProdutos, fAnualProd);
	apagaFatAnualProd(fAnualProd); /* já temos uma cópia de fAnualProd na AVL */
	return fg;
}

/* Cria a estrutura de dados que guarda o total de
 * vendas anuais de um produto, para cada filial. */
static FatAnualProd criaFatAnualProd(Produto p)
{	
	Produto copia = duplicaProduto(p);
	FatAnualProd fAnualProd;

	if(copia == NULL) /* falha a criar a cópia do produto 'p' */
		return NULL;

	fAnualProd = calloc(1, sizeof(struct fatAnualProd));
	if(fAnualProd == NULL){ /* falha da função calloc() */
		free(copia);
		return NULL;
	}
	/* só chegamos aqui se tivemos sucesso em todas as alocações */
	fAnualProd->prod = copia;
	return fAnualProd;
}

/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(FaturacaoGlobal fg, Produto p, double precoUnit,
							 int quantidade, TipoVenda tipo, int filial, int mes)
{	
	double totalFaturado = quantidade * precoUnit;
	FatProdMes fProdMes = criaFatProdMes(p, quantidade, totalFaturado, tipo, filial);
	FatMes fMes;

	if(fProdMes == NULL)
		return NULL;

	/* guarda referência da struct fatMes do mês da venda */
	fMes = fg->fatMensal[mes];
	/* insere a venda e atualiza o total vendido e total faturado do mês */		
	fMes->fatProds = insere(fMes->fatProds, (ValorNodo) fProdMes);
	fMes->totalVendas += quantidade;
	fMes->totalFaturado += totalFaturado;
	/* já temos uma cópia de fatMes na AVL */
	apagaFatProdMes((ValorNodo) fProdMes);
	return fg;
}

/* Cria estrutura de dados com informação 
 * sobre a faturação de um produto num dado mês. */
static FatProdMes criaFatProdMes(Produto p, int quantidade, double totalFaturado, TipoVenda tipo, int filial)
{	
	int i, j;
	FatProdMes fProdMes;
	Produto copia = duplicaProduto(p);

	if(copia == NULL)
		return NULL;

	fProdMes = malloc(sizeof(struct fatProdMes));
		
	if(fProdMes == NULL){ /* falha a alocar a struct fatProdMes */
		apagaProduto(copia);
		return NULL;
	}
		
	fProdMes->prod = copia;
	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 1; j <= N_FILIAIS; ++j){
			fProdMes->vendas[i][j] = 0;
			fProdMes->faturacao[i][j] = 0;
		}
	}
	fProdMes->vendas[tipo][filial] = quantidade;
	fProdMes->faturacao[tipo][filial] = totalFaturado;
	return fProdMes;
}

/* Devolve informação sobre a faturação de um produto num dado mês */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes)
{
	return (FatProdMes) procuraAVL(fg->fatMensal[mes]->fatProds, p);
}

/* Funções usadas na query6 */

/* Devolve o total de vendas registadas num dado mês */
int totalVendasMes(const FaturacaoGlobal fg, int mes)
{
	return fg->fatMensal[mes]->totalVendas;
}

/* Devolve o total faturado num dado mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes)
{	
	return fg->fatMensal[mes]->totalFaturado;
}

/* Devolve o total de vendas registadas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes, total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalVendasMes(fg, mes);
	return total;
}

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes;
	double total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalFaturadoMes(fg, mes);
	return total;
}

/* Fim das funções usadas na query6 */

/* Funções usadas na query3 */

int vendasTotaisProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{
	int total = 0;

	if(fProdMes){
		int i;
		for(i = 1; i <= N_FILIAIS; ++i)
			total += fProdMes->vendas[tipo][i];
	}
	return total;
}

int* vendasPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{	
	int* copiaVendas;

	if(fProdMes == NULL) /* o produto não foi vendido */
		copiaVendas = calloc(N_FILIAIS+1, sizeof(int));
	else{
		copiaVendas = malloc((N_FILIAIS+1) * sizeof(int));
		if(copiaVendas != NULL)
			copiaVendas = memcpy(copiaVendas, fProdMes->vendas[tipo], (N_FILIAIS+1) * sizeof(int));
	}
	return copiaVendas;
}

double faturacaoTotalProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{
	double total = 0;

	if(fProdMes){
		int i;
		for(i = 1; i <= N_FILIAIS; ++i)
			total += fProdMes->faturacao[tipo][i];
	}
	return total;
}

double* faturacaoPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{	
	double* copiaFat;

	if(fProdMes == NULL) /* o produto não foi vendido */
		copiaFat = calloc(N_FILIAIS+1, sizeof(int));
	else{
		copiaFat = malloc((N_FILIAIS+1) * sizeof(double));
		if(copiaFat != NULL)
			copiaFat = memcpy(copiaFat, fProdMes->faturacao[tipo], (N_FILIAIS+1) * sizeof(double));
	}	
	return copiaFat;
}

/* Fim das funções utilizadas na query3 */

/* Início das funções usadas na query4 */

/* Devolve o número total de vendas anuais de um produto */
static int obterTotalVendasAnuaisProd(const FatAnualProd fAnualProd)
{
	int total = 0;

	if(fAnualProd){
		int i;
		for(i = 1; i <= N_FILIAIS; ++i)
			total += fAnualProd->totalVendas[i];
	}
	return total;
}

/* Indica se um produto não teve vendas anuais */
static bool naoComprado(const FatAnualProd fAnualProd)
{	
	return (obterTotalVendasAnuaisProd(fAnualProd) == 0);
}

/* Devolve conjunto ordenado com os códigos de produtos que
 * não forma comprados em nenhuma das filiais. */
ConjuntoProds naoCompradosGlobal(const FaturacaoGlobal fg)
{	/* !! Aqui podiamos fazer um filter com o predicado 'semVendas' */
	int i, j;
	int total = tamanho(fg->todosProdutos); /* número total de produtos */
	FatAnualProd* arrTodosProdutos;
	Produto *naoComprados, *reallocRes; /* reallocRes - guarda valor de retorno do realloc() */
	ConjuntoProds cProds;
 
	naoComprados = malloc(total * sizeof(Produto)); /*  1.37MB */
	if(naoComprados == NULL)
		return NULL;
		
	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	for(i = 0, j = 0; i < total; ++i){
		if(naoComprado(arrTodosProdutos[i])){
			naoComprados[j++] = duplicaProduto(arrTodosProdutos[i]->prod);
		}
	}
	reallocRes = realloc(naoComprados, j * sizeof(Produto));
	if(reallocRes == NULL){
		/* função de libertação de naoComprados */
		return NULL;
	}
	else{
		naoComprados = reallocRes;
		cProds = criaConjuntoProds(j, naoComprados);
	}
	return cProds;
}

/* !! Esta função precisa de limpeza de código e de simplificações */
ConjuntoProds* naoCompradosPorFilial(const FaturacaoGlobal fg)
{	
	Produto** naoComprados;
	FatAnualProd* arrTodosProdutos;
	int total = tamanho(fg->todosProdutos);
	/* Temos um conjunto de produtos não comprados para cada filial */
	ConjuntoProds* cProdsFiliais = malloc((N_FILIAIS+1) * sizeof(ConjuntoProds));

	if(cProdsFiliais == NULL) /* falha de alocação */
		return NULL;
	naoComprados = (Produto**) alocaMatriz(N_FILIAIS+1, total, sizeof(Produto));

	if(naoComprados == NULL)
		return NULL;
	else{
		int i, j;
		int inds[N_FILIAIS+1] = {0}; /* array que na posição j tem o índice da filial j */
		
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		for(i = 0; i < total; ++i){ /* itera ao longo das vendas anuais */
			FatAnualProd atual = arrTodosProdutos[i];
			for(j = 1; j <= N_FILIAIS; ++j){
				if(atual->totalVendas[j] == 0)
					naoComprados[j][inds[j]++] = duplicaProduto(atual->prod);
			}
		}
		/* não está a ser verificado o valor de retorno do realloc() */
		for(j = 1; j <= N_FILIAIS; ++j){
			naoComprados[j] = realloc(naoComprados[j], inds[j] * sizeof(Produto));
			cProdsFiliais[j] = criaConjuntoProds(inds[j], naoComprados[j]);
		}
	}
	return cProdsFiliais;
}

/* Recebe o #linhas e #colunas de uma matriz e o número de bytes
 * de cada elemento. Aloca uma matriz com as dimensões especificadas. */
static void** alocaMatriz(int nlinhas, int ncolunas, size_t nBytesElem)
{	
	int i;
	void** mat = malloc(nlinhas * sizeof(void *));

	if(mat == NULL)
		return NULL;

	for(i = 0; i < ncolunas; ++i){
		mat[i] = malloc(ncolunas * nBytesElem);
		
		if(mat[i] == NULL){ /* falha a alocar a linha i */
			/* liberta linhas [0, i-1] e devolve NULL */
			for(i = i-1; i >= 0; --i)
				free(mat[i]);
			return NULL;
		}
	}
	return mat;
}

/* Fim das funções utilizadas na query4 */

/* Funções utilizadas na query10 */

ConjuntoProds obterNmaisVendidos(int N, FaturacaoGlobal fg)
{	
	int i, total;
	ConjuntoProds cProds;
	FatAnualProd* arrTodosProdutos;
	Produto* NmaisVendidos = malloc(N * sizeof(Produto));
	
	if(NmaisVendidos == NULL)
		return NULL;

	total = tamanho(fg->todosProdutos);
	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	/* ordena array por ordem decrescente do nº total de vendas anuais */
	qsort(arrTodosProdutos, total, sizeof(FatAnualProd), comparaVendasAnuais);
	
	for(i = 0; i < N; ++i){
		NmaisVendidos[i] = duplicaProduto(arrTodosProdutos[i]->prod);
		/* a travessia inorder da AVL devolve duplicados */
		apagaFatAnualProd(arrTodosProdutos[i]);
	}
	cProds = criaConjuntoProds(N, NmaisVendidos);
	return cProds;
}

/* Função de comparação passada para qsort() para se obter o array
 * ordenado decrescentemente pelo total de vendas anuais dos produtos */
static int comparaVendasAnuais(const void* v1, const void* v2)
{	
	int totalVendas1 = obterTotalVendasAnuaisProd((FatAnualProd) v1);
	int totalVendas2 = obterTotalVendasAnuaisProd((FatAnualProd) v2);
	/* trocar a posição dos termos da subtração permite que qsort ordene
	 * os produtos por ordem decrescente do total de vendas anuais */
	return totalVendas2 - totalVendas1;
}

/* Funções de comparaçõa usadas nas AVLs */
static int comparaFatProdMes(const void* p1, const void* p2)
{	
	return comparaCodigosProduto( ((FatProdMes) p1)->prod, 
		                          ((FatProdMes) p2)->prod );
}

static int comparaFatAnualProd(const void* p1, const void* p2)
{
	return comparaCodigosProduto( ((FatAnualProd) p1)->prod, 
		                          ((FatAnualProd) p2)->prod );
}
/* Fim das funções de comparação usadas nas AVLs */

/* Funções de atualização dos nodos das AVLs utilizadas neste módulo */
static void atualizaFatProdMes(void* p1, void* p2)
{	
	int i, j;
	FatProdMes atual = p1;
	FatProdMes adicional = p2;

	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 1; j <= N_FILIAIS; ++j){
			atual->vendas[i][j] += adicional->vendas[i][j];
			atual->faturacao[i][j] += adicional->faturacao[i][j];
		}
	}
	apagaFatProdMes(adicional);
}

static void atualizaFatAnualProd(void* p1, void* p2)
{	
	int i;
	FatAnualProd atual = p1;
	FatAnualProd adicional = p2;

	for(i = 1; i <= N_FILIAIS; ++i)
		atual->totalVendas[i] += adicional->totalVendas[i];
	apagaFatAnualProd(adicional);
}
/* Fim das funções de atualização dos nodos das AVLs */

/* Funções de duplicação dos nodos das AVLs utilizadas neste módulo */
static ValorNodo duplicaFatAnualProd(void* p)
{	
	int i;
	FatAnualProd original = p;
	FatAnualProd novo = malloc(sizeof(struct fatAnualProd));

	if(novo == NULL) /* falha a alocar a struct fatAnualProd */
		return NULL;

	novo->prod = duplicaProduto(original->prod);
	if(novo->prod == NULL){ /* falha a duplicar o produto */
		free(novo);
		return NULL;
	}
	for(i = 1; i <= N_FILIAIS; ++i)
		novo->totalVendas[i] = original->totalVendas[i];
	return (ValorNodo) novo;
}

static ValorNodo duplicaFatProdMes(void* p){
	int i, j;
	FatProdMes original = p;
	FatProdMes novo = malloc(sizeof(struct fatProdMes));

	if(novo == NULL) /* falha a alocar a struct fatProdMes */
		return NULL;

	novo->prod = duplicaProduto(original->prod);
	if(novo->prod == NULL){ /* falha a duplicar o produto */
		free(novo);
		return NULL;
	}

	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 1; j <= N_FILIAIS; ++j){
			novo->vendas[i][j] = original->vendas[i][j];
			novo->faturacao[i][j] = original->faturacao[i][j];
		}
	}
	return (ValorNodo) novo;
}
/* Fim das funções de duplicação de nodos das AVLs */

/* Funções que libertam a memória alocada */

void apagaFaturacaoGlobal(FaturacaoGlobal fg)
{
	if(fg != NULL){
		int i;

		for(i = 0; i <= N_MESES; ++i)
			apagaFatMes(fg->fatMensal[i]);
		apagaAVL(fg->todosProdutos);
	}
}

/* Função da API que permite apagar a estrutura do tipo
 * FatProdMes devolvida pela função obterFatProdMes()  */
void apagaFatProdMes(FatProdMes fProdMes)
{	
	if(fProdMes != NULL){
		apagaProduto(fProdMes->prod);
		free(fProdMes);
	}
}

/* Apaga uma estrutura do tipo FatProdMes armazenada na AVL de fatProds */
static void apagaNodoFatProdMes(void* p)
{
	if(p != NULL){
		FatProdMes fProdMes = p;

		apagaProduto(fProdMes->prod);
		free(fProdMes);
	}
}

static void apagaFatAnualProd(void* p)
{	
	if(p != NULL){
		FatAnualProd fAnualProd = p;

		apagaProduto(fAnualProd->prod);
		free(fAnualProd);
	}
}
