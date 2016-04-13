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
 *	 _FaturacaoGlobal_______________	   __FatAnualProd_______________	  *
 *	|								|	  |								|	  *
 *	|  AVL todosProdutos;			| ==> |	Produto prod;				|	  *
 *	|  FatMes fatMensal[N_MESES+1]; | 	  |	int totalVendas[N_FILIAIS];	|	  *
 *	|_______________________________|	  |_____________________________|	  *
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
 *	|  int vendas[N_TIPOS_VENDA][N_FILIAIS]         |						  *
 *	|  double faturacao[N_TIPOS_VENDA][N_FILIAIS]   |						  *
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
	int totalVendas[N_FILIAIS];
};

struct fatMes{
	int totalVendas;
	double totalFaturado;
	AVL fatProds;
};

/* Guarda informação sobre a faturação de um produto (num dado mês) */
struct fatProdMes{
	Produto prod;
	int vendas[N_TIPOS_VENDA][N_FILIAIS];
	double faturacao[N_TIPOS_VENDA][N_FILIAIS];
};

/* Funções privadas ao módulo */

/* Funções privadas usadas na criação de estruturas de dados */
static FatAnualProd criaFatAnualProd(Produto p);
static FatProdMes criaFatProdMes
(
	Produto p, 
	int quantidade, 
	double totalFaturado,
	TipoVenda tipo, 
	int filial
);
static Produto** alocaArrayNaoComprados(int tamanho);

/* Funções utilizadas na criação de AVLs */
static int comparaFatProdMes(const void* , const void* );
static int comparaFatAnualProd(const void* , const void* );
static void atualizaFatProdMes(void *, void* );
static void atualizaFatAnualProd(void* , void* );

/* Funções que permitem obter informações sobre as vendas
 * anuais de um produto */
static int obterTotalVendasAnuaisProd(const FatAnualProd);
static bool naoComprado(const FatAnualProd);

/* Compara o nº total de vendas anuais de dois produtos */
static int comparaVendasAnuais(const void* v1, const void* v2);

/* Funções de libertação dos tipos de dados não exportados */
static void removeFatMes(FatMes fMes);
static void removeFatAnualProd(FatAnualProd fAnualProd);

FaturacaoGlobal criaFaturacaoGlobal()
{
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatGlobal));
	
	if(fg){
		fg->todosProdutos = criaAVLgenerica(comparaFatAnualProd, atualizaFatAnualProd);
		for(i = 1; i <= N_MESES; i++){
			fg->fatMensal[i]->totalVendas = 0;
			fg->fatMensal[i]->totalFaturado = 0;
			fg->fatMensal[i]->fatProds = criaAVLgenerica(comparaFatProdMes, atualizaFatProdMes);
		}
	}
	return fg; /* se o malloc() falhou, é retornado NULL. Na main decidimos o que fazer */
}

/* Regista um produto na faturação global, guardando-o na AVL de total de vendas
 * anuais com o total de vendas igual a 0 para cada uma das filiais */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p)
{	
	FatAnualProd fAnualProd;
	Produto copia = duplicaProduto(p);

	if(copia && (fAnualProd = criaFatAnualProd(copia)))
		fg->todosProdutos = insere(fg->todosProdutos, fAnualProd);
	else /* falha de alocação a criar FatAnualProd */
		fg = NULL;
	return fg;
}

/* Cria a estrutura de dados que guarda o total de
 * vendas anuais de um produto, para cada filial. */
static FatAnualProd criaFatAnualProd(Produto p)
{
	FatAnualProd fAnualProd = calloc(1, sizeof(struct fatAnualProd));

	if(fAnualProd)
		fAnualProd->prod = p; /* o produto já foi duplicado na função chamadora */
	return fAnualProd;
}


/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(
	FaturacaoGlobal fg,
	Produto p,
	double precoUnit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes)
{
	double totalFaturado = quantidade * precoUnit;
	Produto copia = duplicaProduto(p);
	FatProdMes fatP;

	if(copia && (fatP = criaFatProdMes(copia, quantidade, totalFaturado, tipo, filial))){
		FatMes fatMes = fg->fatMensal[mes]; 
			
		fatMes->fatProds = insere(fatMes->fatProds, (ValorNodo) fatP);
		fatMes->totalVendas += quantidade;
		fatMes->totalFaturado += totalFaturado;
	}
	else /* houve uma falha de alocação */
		fg = NULL;
	return fg;
}


/* Cria estrutura de dados com informação sobre a faturação
 * de um produto num dado mês. */
static FatProdMes criaFatProdMes(
	Produto p,
	int quantidade,
	double totalFaturado,
	TipoVenda tipo,
	int filial)
{
	FatProdMes fProdMes = malloc(sizeof(struct fatProdMes));

	if(fProdMes){
		int i, j;

		fProdMes->prod = p; /* p já é uma cópia do produto passado para registaVenda() */
		for(i = 0; i < N_TIPOS_VENDA; ++i){
			for(j = 0; j < N_FILIAIS; ++j){
				fProdMes->vendas[i][j] = 0;
				fProdMes->faturacao[i][j] = 0;
			}
		}
		fProdMes->vendas[tipo][filial] = quantidade;
		fProdMes->faturacao[tipo][filial] = totalFaturado;
	}
	return fProdMes;
}

/* Devolve informação sobre a faturação de um produto num dado mês */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes)
{	/* !! assumindo que o mês é válido */
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

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProdMes->vendas[tipo][i];
	}
	return total;
}

int* vendasPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{	
	int* copiaVendas;

	if(!fProdMes) /* o produto não foi vendido */
		copiaVendas = calloc(N_FILIAIS, sizeof(int));
	else{ /* o produto foi vendido */
		copiaVendas = malloc(N_FILIAIS * sizeof(int));
		if(copiaVendas)
			copiaVendas = memcpy(copiaVendas, fProdMes->vendas[tipo], N_FILIAIS * sizeof(int));
	}
	return copiaVendas;
}

double faturacaoTotalProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{
	double total = 0;

	if(fProdMes){
		int i;

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProdMes->faturacao[tipo][i];
	}
	return total;
}

double* faturacaoPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{	
	double* copiaFat;

	if(!fProdMes) /* o produto não foi vendido */
		copiaFat = calloc(N_FILIAIS, sizeof(int));
	else{
		copiaFat = malloc(N_FILIAIS * sizeof(double));
		if(copiaFat) /* o produto foi vendido */
			copiaFat = memcpy(copiaFat, fProdMes->faturacao[tipo], N_FILIAIS * sizeof(double));
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

		for(i = 0; i < N_FILIAIS; ++i)
			total += fAnualProd->totalVendas[i];
	}
	return total;
}

/* Indica se um produto não teve vendas anuais */
static bool naoComprado(const FatAnualProd fAnualProd)
{	
	return obterTotalVendasAnuaisProd(fAnualProd) == 0;
}

/* Devolve conjunto ordenado com os códigos de produtos que
 * não forma comprados em nenhuma das filiais. */
ConjuntoProds naoCompradosGlobal(const FaturacaoGlobal fg)
{	
	/* Aqui podiamos fazer um filter com o predicado 'semVendas' */
	int total;
	Produto* naoComprados;
	FatAnualProd* arrTodosProdutos;
	ConjuntoProds cProds = NULL;

	total = tamanho(fg->todosProdutos);
	naoComprados = malloc(total * sizeof(Produto)); /* 1.37MB */
	
	if(naoComprados){
		int i, j;
		Produto* novoNaoComprados; /* guarda retorno de realloc() para verificação de erros */
		
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		for(i = 0, j = 0; i < total; ++i)
			if(naoComprado(arrTodosProdutos[i]))
				naoComprados[j++] = duplicaProduto(arrTodosProdutos[i]->prod);
		
		novoNaoComprados = realloc(naoComprados, j * sizeof(Produto));
		if(!novoNaoComprados){ /* falha de realocação */
			free(naoComprados);
			cProds = NULL;
		}
		else{
			naoComprados = novoNaoComprados;
			cProds = criaConjuntoProds(j, naoComprados);
		}
	}
	return cProds;
}

/* !! Esta função precisa de limpeza de código e de simplificações */
ConjuntoProds* naoCompradosPorFilial(const FaturacaoGlobal fg)
{
	int total;
	Produto** naoComprados;
	FatAnualProd* arrTodosProdutos;
	ConjuntoProds* cProdsFiliais = malloc(N_FILIAIS * sizeof(ConjuntoProds));

	total = tamanho(fg->todosProdutos);
	naoComprados = alocaArrayNaoComprados(total);

	if(naoComprados && cProdsFiliais){
		int i, filial;
		int inds[N_FILIAIS] = {0}; /* array que na posição j tem o índice da filial j+1 */
		
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		for(i = 0; i < total; ++i){ /* itera ao longo das vendas anuais */
			FatAnualProd fAnualProd = arrTodosProdutos[i];
			for(filial = 0; filial < N_FILIAIS; ++filial){
				if(fAnualProd->totalVendas[filial] == 0)
					naoComprados[filial][inds[filial]++] = duplicaProduto(fAnualProd->prod);
			}
		}
		/* não está a ser verificado o valor de retorno do realloc() */
		for(filial = 0; filial < N_FILIAIS; ++filial){
			naoComprados[filial] = realloc(naoComprados[filial], inds[filial] * sizeof(Produto));
			cProdsFiliais[filial] = criaConjuntoProds(inds[filial], naoComprados[filial]);
		}
	}
	return cProdsFiliais;
}

/* !Discutir possibilidade de fazer uma função de alocação de arrays de arrays */
static Produto** alocaArrayNaoComprados(int tamanho)
{
	Produto** naoComprados = malloc(N_FILIAIS * sizeof(Produto *));

	if(naoComprados != NULL){
		int i = 0;

		for(i = 0; i < N_FILIAIS; ++i)
			naoComprados[i] = malloc(tamanho * sizeof(Produto));
			if(naoComprados[i] == NULL){ 
				/* !!! falha da alocação */
			}
	}
	return naoComprados;
}

/* Fim das funções utilizadas na query4 */

/* Funções utilizadas na query10 */

ConjuntoProds obterNmaisVendidos(int N, FaturacaoGlobal fg)
{	
	Produto* NmaisVendidos = malloc(N * sizeof(Produto));
	ConjuntoProds cProds = NULL;

	if(NmaisVendidos){
		int i, total;
		FatAnualProd* arrTodosProdutos;

		total = tamanho(fg->todosProdutos);
		arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
		/* ordena array por ordem decrescente do nº total de vendas anuais */
		qsort(arrTodosProdutos, total, sizeof(FatAnualProd), comparaVendasAnuais);
		for(i = 0; i < N; ++i){
			NmaisVendidos[i] = duplicaProduto(arrTodosProdutos[i]->prod);
			/* assume que inorder devolveu duplicados das faturações anuais dos produtos */
			removeFatAnualProd(arrTodosProdutos[i]);
		}
		cProds = criaConjuntoProds(N, NmaisVendidos);
	}
	return cProds;
}

/* Função de comparação passada para qsort() para se obter o array
   ordenado decrescentemente pelo total de vendas anuais dos produtos */
static int comparaVendasAnuais(const void* v1, const void* v2)
{	
	int totalVendas1 = obterTotalVendasAnuaisProd((FatAnualProd) v1);
	int totalVendas2 = obterTotalVendasAnuaisProd((FatAnualProd) v2);
	/* trocar a posição dos termos da subtração permite que qsort ordene
	 * os produtos por ordem decrescente do total de vendas anuais */
	return totalVendas2 - totalVendas1;
}

/* Funções utilizadas na criação da AVLs */

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

static void atualizaFatProdMes(void* p1, void* p2)
{	
	int i, j;
	FatProdMes atual = p1;
	FatProdMes adicional = p2;

	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 0; j < N_FILIAIS; ++j){
			atual->vendas[i][j] += adicional->vendas[i][j];
			atual->faturacao[i][j] += adicional->faturacao[i][j];
		}
	}
	free(adicional);
}

static void atualizaFatAnualProd(void* p1, void* p2)
{	
	int i;
	FatAnualProd atual = p1;
	FatAnualProd adicional = p2;

	for(i = 0; i < N_FILIAIS; ++i)
		atual->totalVendas[i] += adicional->totalVendas[i];
}

/* Fim das funções utilizadas na criação de AVLs */

/* Funções de libertação da memória alocada */

/* Liberta a memória alocada para a faturação global */
void removeFaturacaoGlobal(FaturacaoGlobal fg)
{
	int i;

	for(i = 0; i < N_MESES + 1; ++i)
		removeFatMes(fg->fatMensal[i]);
	/*removeAVL(fg->todosProdutos);*/ /* discutir se vale a pena fazer a função removeTodosProdutos() */
}

/* Liberta a memória alocada para armazenar informação sobre a faturação de um mês */
static void removeFatMes(FatMes fMes)
{
	/*removeAVL(fMes->fatProds);*/
	free(fMes);
}

/* Liberta a memória alocada para guardar informação 
 * sobre a faturação mensal de um produto */
void removeFatProdMes(FatProdMes fProdMes)
{
	free(fProdMes);
}

/* Liberta o espaço alocado para guardar a faturação anual de um produto */
static void removeFatAnualProd(FatAnualProd fAnualProd)
{
	removeProduto(fAnualProd->prod);
	free(fAnualProd);
}
