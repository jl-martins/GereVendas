#include "faturacaoGlobal.h"
#include "avl.h"
#include "memUtils.h"
#include <stdlib.h>
#include <string.h> /* para utilizar a função memcpy() e strcmp() */

#define N_FILIAIS 3
#define N_MESES 12

#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)

/****************************************************************************** 
 *	Estruturas de dados do módulo:											  *
 *		        														      *
 *	 _FaturacaoGlobal_______________	   __FatAnualProd________________	  *
 *	|								|	  |								 |	  *
 *	|  AVL todosProdutos;			| ==> |	char* prod;				 	 |	  *
 *	|  FatMes fatMensal[N_MESES+1]; | 	  |	int totalUnids[N_FILIAIS+1]; |	  *
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
 *	|  char* prod;               					|						  *
 *	|  int vendas[N_TIPOS_VENDA][N_FILIAIS+1]       |						  *
 *	|  double faturacao[N_TIPOS_VENDA][N_FILIAIS+1] |						  *
 *	|_______________________________________________|						  *
 *																			  *
 ******************************************************************************/

/* Tipos auxiliares (i.e.: não exportados) */
typedef struct fatMes* FatMes;
typedef struct fatAnualProd* FatAnualProd;

/* Faturação global */
struct fatGlobal{
	AVL todosProdutos; /* referencia todos os produtos (mesmo os não vendidos). */
	FatMes fatMensal[N_MESES+1]; /* para cada mês, guarda informação apenas sobre as vendas desse mês */
};

/* Armazena, para um dado produto, informação sobre o seu
 * total de unidades vendidas durante o ano, em cada uma das filiais */
struct fatAnualProd{
	char* prod;
	int totalUnids[N_FILIAIS+1];
};

struct fatMes{
	int totalVendas;
	double totalFaturado;
	AVL fatProds;
};

/* Guarda informação sobre a faturação de um produto (num dado mês) */
struct fatProdMes{
	char* prod;
	int unidsVend[N_TIPOS_VENDA][N_FILIAIS+1];
	double faturacao[N_TIPOS_VENDA][N_FILIAIS+1];
};

/* Funções privadas ao módulo */

/* Funções privadas usadas na criação de estruturas de dados */
static FatAnualProd criaFatAnualProd(char* codProd, int totalUnids[N_FILIAIS+1]);
static FatProdMes criaFatProdMes(char* codProd, int quantidade, double totalFaturado, TipoVenda tipo, int filial);

/* Funções que auxiliam na obtenção dos produtos não comprados */
static LStrings listaNaoCompradosGlobal(FatAnualProd arrTodosProdutos[], int total);
static LStrings listaNaoCompradosFilial(FatAnualProd arrTodosProdutos[], int total, int filial);

/* Funções que devolvem informações sobre o número de unidades vendidas para um dado produto */
static int obterTotalUnidsAnuaisProd(const FatAnualProd fAnualProd);
static bool naoComprado(const FatAnualProd fAnualProd);

/* Funções de libertação dos tipos de dados não exportados */
static void apagaFatMensalInterv(FatMes fMes[], int inicio, int fim);
static void apagaFatMes(FatMes fMes);

/* Funções de comparaçõa usadas nas AVLs */
static int comparaFatProdMes(const void* p1, const void* p2)
{	
	return strcmp( ((FatProdMes) p1)->prod , ((FatProdMes) p2)->prod );
}

static int comparaFatAnualProd(const void* p1, const void* p2)
{
	return strcmp( ((FatAnualProd) p1)->prod , ((FatAnualProd) p2)->prod );
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
			atual->unidsVend[i][j] += adicional->unidsVend[i][j];
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
		atual->totalUnids[i] += adicional->totalUnids[i];
}
/* Fim das funções de atualização dos nodos das AVLs */

/* Funções de duplicação dos nodos das AVLs utilizadas neste módulo */
static void* duplicaFatAnualProd(void* p)
{	
	int i;
	FatAnualProd original = p;
	FatAnualProd copia = malloc(sizeof(struct fatAnualProd));

	if(copia == NULL) /* falha a alocar a struct fatAnualProd */
		return NULL;

	copia->prod = malloc((strlen(original->prod) + 1) * sizeof(char));
	if(copia->prod == NULL){ /* falha de alocação */
		free(copia);
		return NULL;
	}
	strcpy(copia->prod, original->prod);
	for(i = 1; i <= N_FILIAIS; ++i)
		copia->totalUnids[i] = original->totalUnids[i];
	
	return (void *) copia;
}

static void* duplicaFatProdMes(void* p)
{
	int i, j;
	FatProdMes original = p;
	FatProdMes copia = malloc(sizeof(struct fatProdMes));

	if(copia == NULL) /* falha a alocar a struct fatProdMes */
		return NULL;

	copia->prod = malloc((strlen(original->prod) + 1) * sizeof(char));
	if(copia->prod == NULL){ /* falha a duplicar o produto */
		free(copia);
		return NULL;
	}
	strcpy(copia->prod, original->prod);
	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 1; j <= N_FILIAIS; ++j){
			copia->unidsVend[i][j] = original->unidsVend[i][j];
			copia->faturacao[i][j] = original->faturacao[i][j];
		}
	}
	return (void *) copia;
}

/* Funções de libertação de nodos das AVLs */
static void apagaNodoFatProdMes(void* p)
{	
	FatProdMes fProdMes = p;
	
	if(fProdMes != NULL){
		free(fProdMes->prod);
		free(fProdMes);
	}
}

/* Função passada na criação da AVL da faturação anual de todos os produtos */
static void apagaNodoFatAnualProd(void* p)
{	
	FatAnualProd fAnualProd = p;
	
	if(fAnualProd != NULL){
		free(fAnualProd->prod);
		free(fAnualProd);
	}
}

FaturacaoGlobal criaFaturacaoGlobal()
{
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatGlobal));
	
	if(fg == NULL) /* falha a alocar a struct fatGlobal */
		return NULL;

	fg->todosProdutos = criaAVL(atualizaFatAnualProd, comparaFatAnualProd, 
								duplicaFatAnualProd, apagaNodoFatAnualProd);
	if(fg->todosProdutos == NULL){ /* falha de alocação a criar a AVL de 'todosProdutos' */
		free(fg);
		return NULL;
	}

	fg->fatMensal[0] = NULL;
	/* cria estruturas da faturação mensal */
	for(i = 1; i <= N_MESES; ++i){
		fg->fatMensal[i] = malloc(sizeof(struct fatMes));

		if(fg->fatMensal[i] == NULL){ /* falha a alocar a struct fatMes */
			apagaFatMensalInterv(fg->fatMensal, 1, i-1);
			return NULL;
		}
		fg->fatMensal[i]->totalVendas = 0;
		fg->fatMensal[i]->totalFaturado = 0;
		fg->fatMensal[i]->fatProds = criaAVL(atualizaFatProdMes,comparaFatProdMes,
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
		fMes->fatProds = apagaAVL(fMes->fatProds);
		free(fMes);
	}
}

FaturacaoGlobal apagaFaturacaoGlobal(FaturacaoGlobal fg)
{
	if(fg != NULL){
		int i;

		for(i = 0; i <= N_MESES; ++i)
			apagaFatMes(fg->fatMensal[i]);
		fg->todosProdutos = apagaAVL(fg->todosProdutos);
		free(fg);
	}
	return NULL;
}

/* Regista um produto na faturação global, guardando-o na AVL de total de unidades
 * vendidas, com um total de vendas nulo para cada uma das filiais. */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p)
{	
	int arrTotalUnids[N_FILIAIS+1] = {0};
	char* codProd = obterCodigoProduto(p);
	FatAnualProd fAnualProd;
	AVL nova;

	fAnualProd = criaFatAnualProd(codProd, arrTotalUnids);
	if(fAnualProd == NULL)/* falha a criar a struct fatAnualProd */
		return NULL;

	nova = insereAVL(fg->todosProdutos, fAnualProd); 
	if(nova == NULL)
		fg = NULL; /* indica falha a inserir na AVL à função chamadora */
	else
		fg->todosProdutos = nova;

	apagaNodoFatAnualProd((void*) fAnualProd); /* já temos uma cópia de fAnualProd na AVL */
	return fg;
}

/* Cria a estrutura de dados que guarda o total de unidades vendidas
 * durante o ano para um dado produto e uma determinada filial. */
static FatAnualProd criaFatAnualProd(char* codProd, int arrTotalUnids[N_FILIAIS+1])
{	
	FatAnualProd fAnualProd = malloc(sizeof(struct fatAnualProd));

	if(fAnualProd != NULL){
		fAnualProd->prod = codProd;
		memcpy(fAnualProd->totalUnids, arrTotalUnids, (N_FILIAIS + 1) * sizeof(int));
	}
	else
		free(codProd);
	return fAnualProd;
}

/* Regista uma venda na faturação global, atualizando a faturação do mês
 * da venda registada e a faturação anual (i.e.: a que referencia todos os produtos) */
FaturacaoGlobal registaVenda(FaturacaoGlobal fg, Produto p, double precoUnit,
							 int quantidade, TipoVenda tipo, int filial, int mes)
{
	FatProdMes fProdMes;
	FatAnualProd fAnualProd;
	int arrTotalUnids[N_FILIAIS+1] = {0};
	AVL retInsere; /* guarda valor de retorno da função insereAVL() */

	fProdMes = criaFatProdMes(obterCodigoProduto(p), quantidade, quantidade * precoUnit, tipo, filial);
	if(fProdMes == NULL) /* falha a alocar memória na função criaFatProdMes() */
		return NULL;
	
	arrTotalUnids[filial] = quantidade; 
	fAnualProd = criaFatAnualProd(obterCodigoProduto(p), arrTotalUnids);

	if(fAnualProd == NULL){ /* falha a alocar memória na função criaFatAnualProd()*/
		apagaFatProdMes((void*) fProdMes);
		return NULL;
	}

	retInsere = insereAVL(fg->fatMensal[mes]->fatProds, fProdMes);
	if(retInsere != NULL){
		++(fg->fatMensal[mes]->totalVendas);
		fg->fatMensal[mes]->totalFaturado += (quantidade * precoUnit);
		fg->fatMensal[mes]->fatProds = retInsere;
		
		retInsere = insereAVL(fg->todosProdutos, fAnualProd);
		if(retInsere != NULL)
			fg->todosProdutos = retInsere;
	}
	/* chegando a este ponto, já temos cópias de fProdMes e fAnualProd 
	 * na AVL da faturação do mês e na de todos os produtos, respetivamente */
	apagaFatProdMes((void *) fProdMes);
	apagaNodoFatAnualProd((void *) fAnualProd);
	return fg;
}

/* Cria estrutura de dados com informação 
 * sobre a faturação de um produto num dado mês. */
static FatProdMes criaFatProdMes(char* codProd, int quantidade, double totalFaturado, TipoVenda tipo, int filial)
{	
	int i, j;
	FatProdMes fProdMes;

	fProdMes = malloc(sizeof(struct fatProdMes));
	if(fProdMes == NULL){
		free(codProd);
		return NULL;
	}
	/* Preenchimento da estrutura de dados */
	fProdMes->prod = codProd;
	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 1; j <= N_FILIAIS; ++j){
			fProdMes->unidsVend[i][j] = 0;
			fProdMes->faturacao[i][j] = 0;
		}
	}
	fProdMes->unidsVend[tipo][filial] = quantidade;
	fProdMes->faturacao[tipo][filial] = totalFaturado;
	return fProdMes;
}

/* Query 6 (os meses passados como argumentos devem ser validados pela função chamadora) */

int totalVendasMes(const FaturacaoGlobal fg, int mes)
{	
	return fg->fatMensal[mes]->totalVendas;
}

double totalFaturadoMes(const FaturacaoGlobal fg, int mes)
{
	return fg->fatMensal[mes]->totalFaturado;
}

int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes, total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalVendasMes(fg, mes);
	return total;
}

double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim)
{
	int mes;
	double total = 0;

	for(mes = inicio; mes <= fim; ++mes)
		total += totalFaturadoMes(fg, mes);
	return total;
}

/* Query 3 */

/* Devolve informação sobre a faturação de um produto num dado mês */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes)
{	
	FatProdMes fProdMes = NULL;
	
	if(MES_VALIDO(mes)){
		FatProdMes tmp = malloc(sizeof(struct fatProdMes));

		if(tmp == NULL)
			return NULL;

		tmp->prod = obterCodigoProduto(p);
		if(tmp->prod == NULL){ /* falha a criar o produto */
			free(tmp);
			return NULL;
		}
		fProdMes = (FatProdMes) procuraAVL(fg->fatMensal[mes]->fatProds, tmp);
		free(tmp->prod); free(tmp);
	}
	return fProdMes;
}

/* Função da API que permite apagar a estrutura do tipo
 * FatProdMes devolvida pela função obterFatProdMes()  */
FatProdMes apagaFatProdMes(FatProdMes fProdMes)
{	
	if(fProdMes != NULL){
		free(fProdMes->prod);
		free(fProdMes);
	}
	return NULL;
}

int totalUnidsProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{
	int total = 0;

	if(fProdMes){
		int i;
		for(i = 1; i <= N_FILIAIS; ++i)
			total += fProdMes->unidsVend[tipo][i];
	}
	return total;
}

int* unidsPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo)
{	
	int* copiaUnidsVend;

	if(fProdMes == NULL) /* o produto não foi vendido */
		copiaUnidsVend = calloc(N_FILIAIS + 1, sizeof(int));
	else{
		copiaUnidsVend = malloc((N_FILIAIS + 1) * sizeof(int));
		if(copiaUnidsVend != NULL)
			copiaUnidsVend = memcpy(copiaUnidsVend, fProdMes->unidsVend[tipo], (N_FILIAIS + 1) * sizeof(int));
	}
	return copiaUnidsVend;
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
		copiaFat = calloc(N_FILIAIS + 1, sizeof(double));
	else{
		copiaFat = malloc((N_FILIAIS + 1) * sizeof(double));
		if(copiaFat != NULL)
			copiaFat = memcpy(copiaFat, fProdMes->faturacao[tipo], (N_FILIAIS+1) * sizeof(double));
	}	
	return copiaFat;
}

/* Query 4 */

/* Recebe a faturação anual de um produto. 
 * Devolve o total de unidades vendidas desse produto, durante o ano. */
static int obterTotalUnidsAnuaisProd(const FatAnualProd fAnualProd)
{
	int total = 0;

	if(fAnualProd){
		int i;
		for(i = 1; i <= N_FILIAIS; ++i)
			total += fAnualProd->totalUnids[i];
	}
	return total;
}

static bool naoComprado(const FatAnualProd fAnualProd)
{	
	return (obterTotalUnidsAnuaisProd(fAnualProd) == 0);
}

/* Query12 */
int quantosNaoComprados(const FaturacaoGlobal fg)
{
	FatAnualProd* arrTodosProdutos;
	int total = tamanhoAVL(fg->todosProdutos);
	int quantos = 0;

	arrTodosProdutos = (FatAnualProd *) inorderAVL(fg->todosProdutos);
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

LStrings naoCompradosGlobal(const FaturacaoGlobal fg)
{
	FatAnualProd* arrTodosProdutos;
	int total = tamanhoAVL(fg->todosProdutos);
	LStrings res = NULL;

	arrTodosProdutos = (FatAnualProd *) inorderAVL(fg->todosProdutos);
	if(arrTodosProdutos != NULL){
		res = listaNaoCompradosGlobal(arrTodosProdutos, total);
		/* os códigos dos produtos não comprados já estão em 'res' */
		apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
	}
	return res;
}

/* Recebe um array com informação sobre a faturação anual de cada produto.
 * Devolve uma LStrings ordenada por código de produto, com os códigos dos produtos
 * não comprados globalmente */
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
			int len = strlen(arrTodosProdutos[i]->prod);
			char* codigoProd = malloc((len + 1) * sizeof(char));
			
			if(codigoProd == NULL){ /* falha de alocação de codigoProd */
				apagaArray((void **) naoComprados, numNaoComp, free);
				return NULL;
			}
			strcpy(codigoProd, arrTodosProdutos[i]->prod);
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
	LStrings* arrLStr = calloc(N_FILIAIS + 1, sizeof(LStrings));

	if(arrLStr == NULL) /* falha de alocação. Não é possível gerar o resultado */
		return NULL;
	
	arrTodosProdutos = (FatAnualProd *) inorderAVL(fg->todosProdutos);
	if(arrTodosProdutos == NULL){ /* falha de alocação na função inorder() */
		free(arrLStr);
		return NULL;
	}
	total = tamanhoAVL(fg->todosProdutos);

	for(filial = 1; filial <= N_FILIAIS; ++filial){
		LStrings lStr = listaNaoCompradosFilial(arrTodosProdutos, total, filial);

		if(lStr == NULL){ /* falha de alocação a criar lStr */
			for( ; filial >= 1; --filial)
				arrLStr[filial] = apagaLStrings(arrLStr[filial]);
			apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
			free(arrLStr);
			return NULL;
		}
		arrLStr[filial] = lStr;
	}
	apagaArray((void**) arrTodosProdutos, total, apagaNodoFatAnualProd);
	return arrLStr;
}

/* Recebe um array com informação sobre a faturação anual de todos os produtos, o tamanho
 * desse array e uma filial. Devolve uma LStrings ordenada por código de produto, com os
 * códigos de todos os produtos não comprados na filial especificada */
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
		if(atual->totalUnids[filial] == 0){
			int len = strlen(atual->prod);
			char* codigoProd = malloc((len + 1) * sizeof(char));

			if(codigoProd == NULL){ /* falha a duplicar código do produto */
				apagaArray((void**) naoComprados, numNaoComp, free);
				return NULL;
			}
			strcpy(codigoProd, atual->prod);
			naoComprados[numNaoComp++] = codigoProd;
		}
	}
	lStr = criaLStrings(numNaoComp, naoComprados);
	apagaArray((void**) naoComprados, numNaoComp, free);
	return lStr;
}

/* Query 10 */
#define compUnidsAnuais(filial) \
	static int compUnidsAnuaisFilial##filial(const void* v1, const void* v2)  \
	{																		  \
		int totalUnids1 = (*(FatAnualProd *) v1)->totalUnids[filial];		  \
		int totalUnids2 = (*(FatAnualProd *) v2)->totalUnids[filial];		  \
																			  \
		return totalUnids2 - totalUnids1;									  \
	}																		  \

compUnidsAnuais(1)
compUnidsAnuais(2)
compUnidsAnuais(3)

static int (*arrFunCompara[N_FILIAIS+1]) (const void*, const void*) = {
	NULL,
	compUnidsAnuaisFilial1,
	compUnidsAnuaisFilial2,
	compUnidsAnuaisFilial3
};

char** NmaisVendidosFilial(const FaturacaoGlobal fg, int N, int filial)
{
	int i, total;
	FatAnualProd* arrTodosProdutos;
	FatAnualProd* fatNmaisVend;
	char** maisVendFilial = malloc(N * sizeof(char *));

	if(maisVendFilial == NULL)
		return NULL;

	total = tamanhoAVL(fg->todosProdutos);
	arrTodosProdutos = (FatAnualProd *) inorderAVL(fg->todosProdutos);
	if(arrTodosProdutos == NULL){ /* a inorderAVL() não conseguiu criar cópias dos elementos da AVL */
		free(maisVendFilial);
		return NULL;
	}
	/* ordena array por ordem decrescente do nº total de vendas anuais na filial especificada */
	qsort(arrTodosProdutos, total, sizeof(FatAnualProd), arrFunCompara[filial]);
	
	fatNmaisVend = realloc(arrTodosProdutos, N * sizeof(FatAnualProd));
	for(i = 0; i < N; ++i){
		maisVendFilial[i] = fatNmaisVend[i]->prod;
		free(fatNmaisVend[i]);
	}
	return maisVendFilial;
}
