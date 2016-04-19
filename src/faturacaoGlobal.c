#include "faturacaoGlobal.h"
#include "avl.h"
#include "memUtils.h"
#include <stdlib.h>
#include <string.h> /* para utilizar a função memcpy() */

/* !! Decidir onde vamos colocar estas definições */
#define N_FILIAIS 3
#define N_MESES 12

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

/* FatMes é um tipo auxiliar (i.e.: não exportado) */
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
static FatAnualProd criaFatAnualProd(Produto p, int totalVendas[N_FILIAIS+1]);
static FatProdMes criaFatProdMes(Produto p, int quantidade, double totalFaturado,
								 TipoVenda tipo, int filial);

/* Funções que auxiliam na obtenção dos produtos não comprados */
static LStrings listaNaoCompradosGlobal(FatAnualProd arrTodosProdutos[], int total);
static LStrings listaNaoCompradosFilial(FatAnualProd arrTodosProdutos[], int total, int filial);

/* Funções que devolvem informações sobre as vendas anuais de um produto */
static int obterTotalVendasAnuaisProd(const FatAnualProd);
static bool naoComprado(const FatAnualProd);

/* Compara o nº total de vendas anuais de dois produtos */
static int comparaVendasAnuais(const void* v1, const void* v2);

/* Funções de libertação dos tipos de dados não exportados */
static void apagaFatMensalInterv(FatMes fMes[], int inicio, int fim);
static void apagaFatMes(FatMes fMes);

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
}

static void atualizaFatAnualProd(void* p1, void* p2)
{	
	int i;
	FatAnualProd atual = p1;
	FatAnualProd adicional = p2;

	for(i = 1; i <= N_FILIAIS; ++i)
		atual->totalVendas[i] += adicional->totalVendas[i];
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

/* Funções de libertação de nodos das AVLs */
static void apagaNodoFatProdMes(void* p)
{
	if(p != NULL){
		FatProdMes fProdMes = p;

		apagaProduto(fProdMes->prod);
		free(fProdMes);
	}
}

/* Função passada na criação da AVL da faturação anual de todos os produtos */
static void apagaNodoFatAnualProd(void* p)
{	
	if(p != NULL){
		FatAnualProd fAnualProd = p;

		apagaProduto(fAnualProd->prod);
		free(fAnualProd);
	}
}

void apagaFatAnualProd(FatAnualProd fAnualProd)
{
	apagaNodoFatAnualProd((void *) fAnualProd);
}

FaturacaoGlobal criaFaturacaoGlobal()
{
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatGlobal));
	
	if(fg == NULL) /* falha a alocar a struct fatGlobal */
		return NULL;

	fg->todosProdutos = criaAVLgenerica(atualizaFatAnualProd, comparaFatAnualProd, 
										duplicaFatAnualProd, apagaNodoFatAnualProd);
	fg->fatMensal[0] = NULL;
	for(i = 1; i <= N_MESES; ++i){ /* cria estruturas da faturação mensal */
		fg->fatMensal[i] = malloc(sizeof(struct fatMes));

		if(fg->fatMensal[i] == NULL){ /* falha a alocar a struct fatMes */
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
static void apagaFatMensalInterv(FatMes fatMensal[], int inicio, int fim)
{
	if(fatMensal != NULL && inicio >= 0){
		int i;

		for(i = inicio; i <= fim; ++i)
			apagaFatMes(fatMensal[i]);
	}
}

/* Liberta a memória alocada para uma struct fatMes */
static void apagaFatMes(FatMes fMes)
{	
	if(fMes != NULL){
		apagaAVL(fMes->fatProds);
		free(fMes);
	}
}

void apagaFaturacaoGlobal(FaturacaoGlobal fg)
{
	if(fg != NULL){
		int i;

		for(i = 0; i <= N_MESES; ++i)
			apagaFatMes(fg->fatMensal[i]);
		apagaAVL(fg->todosProdutos);
	}
}

/* Regista um produto na faturação global, guardando-o na AVL de total de vendas
 * anuais com o total de vendas igual a 0, para cada uma das filiais. */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p)
{	
	int totalVendas[N_FILIAIS+1] = {0};
	FatAnualProd fAnualProd;
	AVL nova;

	fAnualProd = criaFatAnualProd(p, totalVendas);
	if(fAnualProd == NULL)/* falha a criar a struct fatAnualProd */
		return NULL;

	/* insere cópia de fAnualProd na AVL de todos os produtos */
	nova = insere(fg->todosProdutos, fAnualProd); 
	if(nova == NULL)
		fg = NULL; /* indica falha a inserir na AVL à função chamadora */
	else
		fg->todosProdutos = nova;

	apagaFatAnualProd(fAnualProd);
	return fg;
}

/* Cria a estrutura de dados que guarda o total de
 * vendas anuais de um produto, para cada filial. */
static FatAnualProd criaFatAnualProd(Produto p, int totalVendas[N_FILIAIS+1])
{	
	FatAnualProd fAnualProd = malloc(sizeof(struct fatAnualProd));

	if(fAnualProd != NULL){
		Produto copia = duplicaProduto(p);
		if(copia != NULL){
			fAnualProd->prod = copia;
			memcpy(fAnualProd->totalVendas, totalVendas, (N_FILIAIS + 1) * sizeof(int));
		}
	}
	return fAnualProd;
}

/* Regista uma venda na faturação global, atualizando a faturação do mês
 * da venda registada e a faturação anual (i.e.: a que referencia todos os produtos) */
FaturacaoGlobal registaVenda(FaturacaoGlobal fg, Produto p, double precoUnit,
							 int quantidade, TipoVenda tipo, int filial, int mes)
{
	FatProdMes fProdMes;
	FatAnualProd fAnualProd;
	int totalVendas[N_FILIAIS+1] = {0};
	AVL retInsere; /* guarda valor de retorno da função insere() */

	fProdMes = criaFatProdMes(p, quantidade, quantidade * precoUnit, tipo, filial);
	if(fProdMes == NULL) /* falha a alocar memória na função criaFatProdMes() */
		return NULL;
	
	totalVendas[filial] = quantidade; 
	fAnualProd = criaFatAnualProd(p, totalVendas);

	if(fAnualProd == NULL){ /* falha a alocar memória na função criaFatAnualProd()*/
		apagaFatProdMes((void*) fProdMes);
		return NULL;
	}

	retInsere = insere(fg->fatMensal[mes]->fatProds, fProdMes);
	if(retInsere != NULL){
		fg->fatMensal[mes]->totalVendas += quantidade;
		fg->fatMensal[mes]->totalFaturado += (quantidade * precoUnit); /* guardar faturaçao em variavel local ??? */
		fg->fatMensal[mes]->fatProds = retInsere;
		
		retInsere = insere(fg->todosProdutos, fAnualProd);
		if(retInsere != NULL)
			fg->todosProdutos = retInsere;

	}
	/* chegando a este ponto, já temos cópias de fProdMes e fAnualProd 
	 * na AVL da faturação do mês e na de todos os produtos, respetivamente */
	apagaFatProdMes((void *) fProdMes);
	apagaFatAnualProd(fAnualProd);
	return fg;
}

/* Cria estrutura de dados com informação 
 * sobre a faturação de um produto num dado mês. */
static FatProdMes criaFatProdMes(Produto p, int quantidade, double totalFaturado, TipoVenda tipo, int filial)
{	
	int i, j;
	FatProdMes fProdMes;
	Produto copia = duplicaProduto(p);

	if(copia == NULL) /* falha a duplicar o produto */
		return NULL;

	fProdMes = malloc(sizeof(struct fatProdMes));
	if(fProdMes == NULL){ /* falha a alocar a struct fatProdMes */
		apagaProduto(copia);
		return NULL;
	}
	/* Preenchimento da estrutura de dados */
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
	FatProdMes fProdMes = NULL;
	
	if(MES_VALIDO(mes)){
		FatProdMes tmp = malloc(sizeof(struct fatProdMes));

		tmp->prod = p;
		fProdMes = (FatProdMes) procuraAVL(fg->fatMensal[mes]->fatProds, tmp);
		free(tmp);
	}
	return fProdMes;
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

/* Funções usadas na query6 */

/* Devolve o total de vendas registadas num dado mês. */
int totalVendasMes(const FaturacaoGlobal fg, int mes)
{	
	return fg->fatMensal[mes]->totalVendas;
}

/* Devolve o total faturado num dado mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes)
{	/* o mês deve ser validado pela função chamadora */
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
		copiaVendas = calloc(N_FILIAIS + 1, sizeof(int));
	else{
		copiaVendas = malloc((N_FILIAIS + 1) * sizeof(int));
		if(copiaVendas != NULL)
			copiaVendas = memcpy(copiaVendas, fProdMes->vendas[tipo], (N_FILIAIS + 1) * sizeof(int));
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
		copiaFat = calloc(N_FILIAIS + 1, sizeof(int));
	else{
		copiaFat = malloc((N_FILIAIS + 1) * sizeof(double));
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

int quantosNaoComprados(const FaturacaoGlobal fg)
{
	FatAnualProd* arrTodosProdutos;
	int total = tamanho(fg->todosProdutos);
	int quantos = 0;

	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	if(arrTodosProdutos == NULL)
		quantos = -1; /* sinaliza falha de alocação na inorder da AVL com todos os produtos */
	else{
		int i;
		for(i = 0; i < total; ++i)
			if(naoComprado(arrTodosProdutos[i]))
				++quantos;
		apagaArray((void **) arrTodosProdutos, total, apagaNodoFatAnualProd);
	}
	return quantos;
}

/* Devolve conjunto ordenado com os códigos de produtos que
 * não forma comprados em nenhuma das filiais. */
LStrings naoCompradosGlobal(const FaturacaoGlobal fg)
{
	FatAnualProd* arrTodosProdutos;
	int total = tamanho(fg->todosProdutos);
	LStrings res = NULL;

	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	if(arrTodosProdutos != NULL){
		res = listaNaoCompradosGlobal(arrTodosProdutos, total);
		/* o array com o código dos produtos não comprados já está em 'res' */
		apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
	}
	return res;
}

/* Recebe o array com a faturação anual de todos os produtos, resultante da
 * travessia inorder da AVL 'todosProdutos' */
static LStrings listaNaoCompradosGlobal(FatAnualProd* arrTodosProdutos, int total)
{	
	int i, numNaoComp;
	char** naoComprados;
	LStrings lStr;

	naoComprados = malloc(total * sizeof(char *));
	if(naoComprados == NULL) /* falha de alocação */
		return NULL;
	
	numNaoComp = 0;
	for(i = 0; i < total; ++i){
		if(naoComprado(arrTodosProdutos[i])){
			char* codigoProd = obterCodigoProduto(arrTodosProdutos[i]->prod);
			
			if(codigoProd == NULL){ /* falha de alocação de codigoProd */
				apagaArray((void**) naoComprados, numNaoComp, free);
				return NULL;
			}
			naoComprados[numNaoComp++] = codigoProd;
		}
	}
	/* só chegamos aqui se não houve falhas de alocação */
	lStr = criaLStrings(numNaoComp, naoComprados);
	apagaArray((void**) naoComprados, numNaoComp, free);
	return lStr;
}

LStrings* naoCompradosPorFilial(const FaturacaoGlobal fg)
{	
	int filial, total;
	FatAnualProd* arrTodosProdutos;
	LStrings* arrLStr = malloc((N_FILIAIS + 1) * sizeof(LStrings));

	if(arrLStr == NULL) /* falha de alocação. Não é possível gerar o resultado */
		return NULL;
	
	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	if(arrTodosProdutos == NULL){ /* falha de alocação na função inorder() */
		free(arrLStr);
		return NULL;
	}
	total = tamanho(fg->todosProdutos);

	for(filial = 1; filial <= N_FILIAIS; ++filial){
		LStrings lStr = listaNaoCompradosFilial(arrTodosProdutos, total, filial);

		if(lStr == NULL){ /* falha de alocação a criar lStr */
			/* liberta a memória alocada */
			--filial;
			while(filial >= 1)
				apagaLStrings(arrLStr[filial--]);
			apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
			return NULL;
		}
		arrLStr[filial] = lStr;
	}
	apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
	return arrLStr;
}

static LStrings listaNaoCompradosFilial(FatAnualProd arrTodosProdutos[], int total, int filial)
{
	int i, numNaoComp;
	char** naoComprados;
	LStrings lStr;

	naoComprados = malloc(total * sizeof(char *));
	if(naoComprados == NULL)
		return NULL;
	
	numNaoComp = 0;
	for(i = 0; i < total; ++i){
		FatAnualProd atual = arrTodosProdutos[i];
		if(atual->totalVendas[filial] == 0){
			char* codigoProd = obterCodigoProduto(atual->prod);

			if(codigoProd == NULL){ /* falha a duplicar código do produto */
				apagaArray((void**) naoComprados, numNaoComp, free);
				return NULL;
			}
			naoComprados[numNaoComp++] = codigoProd;
		}
	}
	lStr = criaLStrings(numNaoComp, naoComprados);
	apagaArray((void**) naoComprados, numNaoComp, free);
	return lStr;
}

/* Fim das funções utilizadas na query4 */

/* Funções utilizadas na query10 */

FatAnualProd* fatNmaisVendidos(const FaturacaoGlobal fg, int N)
{
	int total;
	FatAnualProd* arrTodosProdutos;
	FatAnualProd* fatNmaisVend;

	total = tamanho(fg->todosProdutos);
	arrTodosProdutos = (FatAnualProd *) inorder(fg->todosProdutos);
	if(arrTodosProdutos == NULL) /* a inorder() não conseguiu criar cópias dos elementos da AVL */
		return NULL;
	
	/* ordena array por ordem decrescente pelo nº total de vendas anuais */
	qsort(arrTodosProdutos, total, sizeof(FatAnualProd), comparaVendasAnuais);
	fatNmaisVend = realloc(arrTodosProdutos, N * sizeof(FatAnualProd));
	return fatNmaisVend;
}

Produto* obterArrNmaisVendidos(const FatAnualProd fatNmaisVend[], int N)
{	
	int i;
	Produto* nMaisVend = malloc(N * sizeof(Produto));

	if(nMaisVend == NULL)
		return NULL;

	for(i = 0; i < N; ++i){
		nMaisVend[i] = duplicaProduto(fatNmaisVend[i]->prod);
		if(nMaisVend[i] == NULL){ /* falha a duplicar o produto */
			--i;
			while(i >= 0)
				apagaProduto(nMaisVend[i--]);
			return NULL;
		}
	}
	return nMaisVend;
}

int** vendasPorFilialNmaisVend(const FatAnualProd fatNmaisVend[], int N)
{
	int i, j;
	int** vendasPorFilial = (int **) alocaMatriz(N, N_FILIAIS+1, sizeof(int));

	if(vendasPorFilial == NULL)
		return NULL;

	for(i = 0; i < N; ++i)
		for(j = 1; j <= N_FILIAIS; ++j)
			vendasPorFilial[i][j] = fatNmaisVend[i]->totalVendas[j];
	return vendasPorFilial;
}

/* Função de comparação passada para qsort() para se obter o array
 * ordenado decrescentemente pelo total de vendas anuais dos produtos */
static int comparaVendasAnuais(const void* v1, const void* v2)
{	
	int totalVendas1 = obterTotalVendasAnuaisProd(*(FatAnualProd *) v1);
	int totalVendas2 = obterTotalVendasAnuaisProd(*(FatAnualProd *) v2);
	/* trocar a posição dos termos da subtração permite que qsort ordene
	 * os produtos por ordem decrescente do total de vendas anuais */
	return totalVendas2 - totalVendas1;
}