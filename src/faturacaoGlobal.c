/* As 3 macros que se seguem podem (possivelmente) ser colocadas noutro .h */
#include "faturacaoGlobal.h"
#include "avl.h"
#include <stdlib.h>
#include <string.h> /* para utilizar a função memcpy() */

/* !! Decidir onde vamos colocar estas defnições */
#define N_FILIAIS 3
#define N_MESES 12

/* !! Decidir se vale a pena validar meses */
#define MES_VALIDO(mes) ((mes) >= 1 && (mes) <= 12)

/* Estruturas de dados do módulo:
		        		
	 _FaturacaoGlobal______________		  _foiVendido_______________
	|  							   |	 |							|
	|  AVL foramVendidos;          | ==> |	Produto prod;			|
	|  FatMes fatMensal[13];       | 	 |	bool vendido[N_FILIAIS] |
	|______________________________|	 |__________________________|
				 
				 ||
				 \/	
	 _FatMes_______________________
	|                              |
	|  int totalVendas;            |
	|  int totalFaturado;          |
	|  AVL fatProds;               |
	|______________________________|

				 ||
				 \/

 	 _FaturacaoProd________________
	|                              |
	|  Produto prod;               |
	|  int vendas[2][3]            |
	|  double faturacao[2][3]      |
	|______________________________|
*/

/* Guarda informação sobre a faturação de um produto (num dado mês) */

struct fatProd{
	Produto prod;
	int vendas[N_TIPOS_VENDA][N_FILIAIS];
	double faturacao[N_TIPOS_VENDA][N_FILIAIS];
};

/* Guarda informação sobre a faturação de um dado mês */

typedef struct fatMes{
	int totalVendas;
	double totalFaturado;
	AVL fatProds;
}* FatMes;

/* Faturação global */
struct fatG{
	/* permite saber quais os códigos produto que ninguém comprou (por filial ou no total) */
	AVL foramVendidos;
	/* guarda, para cada mês, informação quantitativa sobre os produtos vendidos*/
	FatMes fatMensal[N_MESES+1]; 
};

/* Indica se um produto foi vendido e, se sim, em que filiais */

typedef struct foiVendido{
	Produto prod;
	bool vendido[N_FILIAIS];
}* FoiVendido;

/* Funções privadas ao módulo */

/* !! ver como partir a assinatura de funções em várias linhas para ficar mais legível */
static FaturacaoProd criaFaturacaoProd(Produto p, int quantidade, double total_faturado,
	    							   TipoVenda tipo, int filial);

/* Funções utilizadas na criação de AVLs */
static int comparaFaturacaoProd(const void* p1, const void* p2);
static int comparaFoiVendido(const void* p1, const void* p2);
static void atualizaFaturacaoProd(void *, void* );
static void atualizaFoiVendido(void* , void* );

FaturacaoGlobal criaFaturacaoGlobal(){
	int i;
	FaturacaoGlobal fg = malloc(sizeof(struct fatG));
	
	if(fg != NULL){
		/* ver com que parâmetros devemos criar as AVLs */
		fg->foramVendidos = criaAVLgenerica(comparaFoiVendido, atualizaFoiVendido);
		for(i = 1; i <= N_MESES; i++){
			fg->fatMensal[i]->totalVendas = 0;
			fg->fatMensal[i]->totalFaturado = 0;
			fg->fatMensal[i]->fatProds = criaAVLgenerica(comparaFaturacaoProd,
				                                         atualizaFaturacaoProd);
		}
	}
	return fg; /* se o malloc() falhou, é retornado NULL. Na main decidimos o que fazer */
}

/* Ver como sinalizar erros */
FaturacaoGlobal registaVenda(FaturacaoGlobal fg, Produto p, double preco_unit,
			 				 int quantidade, TipoVenda tipo, int filial, int mes)
{
	double total_faturado = quantidade * preco_unit;
	FaturacaoProd fatP = criaFaturacaoProd(p, quantidade, total_faturado, tipo, filial);

	if(fatP != NULL){
		FatMes fatMes = fg->fatMensal[mes];
		fatMes->fatProds = insere(fatMes->fatProds, (ValorNodo) fatP);

		/* Atualiza valores totais do mês */
		fatMes->totalVendas += quantidade;
		fatMes->totalFaturado += total_faturado;
	}
	else
		fg = NULL;
	return fg;
}

static FaturacaoProd criaFaturacaoProd(Produto p, int quantidade, double total_faturado,
									   TipoVenda tipo, int filial)
{
	FaturacaoProd fProd = malloc(sizeof(struct fatProd));

	if(fProd != NULL){
		int i, j;

		fProd->prod = p;
		for(i = 0; i < N_TIPOS_VENDA; ++i){
			for(j = 0; j < N_FILIAIS; ++j){
				fProd->vendas[i][j] = 0;
				fProd->faturacao[i][j] = 0;
			}
		}
		fProd->vendas[tipo][filial] = quantidade;
		fProd->faturacao[tipo][filial] = total_faturado;
	}
	return fProd;
}

/* Devolve informação sobre a faturação de um produto num dado mês */
FaturacaoProd obterFaturacaoProduto(const FaturacaoGlobal fg, const Produto p, int mes){
	/* ver se podemos assumir que o mês é válido */
	return (FaturacaoProd) procuraAVL(fg->fatMensal[mes]->fatProds, p); /* retornamos NULL se o produto não foi vendido */
}

/* Funções usadas na query6 */

/* Devolve o total de vendas de um dado mês */
/* (Não há nenhuma query que peça o total de vendas de um mês, mas pode ser útil ter esta função na API) */
int totalVendasMes(const FaturacaoGlobal fg, int mes){
	return fg->fatMensal[mes]->totalVendas;
}

/* Devolve o total faturado num dado mês */
/* (Não há nenhuma query que peça o total faturado num mês, mas pode ser útil ter esta função na API) */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes){
	return fg->fatMensal[mes]->totalFaturado;
}

/* Devolve o total de vendas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim){
	int i, total = 0;

	for(i = inicio; i <= fim; ++i)
		total += totalVendasMes(fg, i);
	return total;
}

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim){
	int i;
	double total = 0;

	for(i = inicio; i <= fim; ++i)
		total += totalFaturadoMes(fg, i);
	return total;
}

/* Fim das funções usadas na query6 */

/* Funções usadas na query3 */

int* vendasPorFilial(FaturacaoProd fProd, TipoVenda tipo)
{	
	int* copiaVendas;

	if(fProd != NULL){ /* o produto foi vendido */
		copiaVendas = malloc(N_FILIAIS * sizeof(int));
		if(copiaVendas != NULL)
			copiaVendas = memcpy(copiaVendas, fProd->vendas[tipo], N_FILIAIS * sizeof(int));
	}
	else /* o produto não foi vendido, logo devolvemos um array incializado a 0 */
		copiaVendas = calloc(N_FILIAIS, sizeof(int));

	return copiaVendas;
}

int vendasTotais(FaturacaoProd fProd, TipoVenda tipo)
{
	int total = 0;

	if(fProd != NULL){
		int i;

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProd->vendas[tipo][i];
	}
	return total;
}

double* faturacaoPorFilial(FaturacaoProd fProd, TipoVenda tipo)
{	
	double* copiaFat;

	if(fProd != NULL){ /* o produto foi vendido */
		copiaFat = malloc(N_FILIAIS * sizeof(double));
		if(copiaFat != NULL)
			copiaFat = memcpy(copiaFat, fProd->faturacao[tipo], N_FILIAIS * sizeof(double));
	}
	else /* o produto não foi vendido, logo devolvemos um array incializado a 0 */
		copiaFat = calloc(N_FILIAIS, sizeof(int));
		
	return copiaFat;
}

double faturacaoTotal(FaturacaoProd fProd, TipoVenda tipo)
{
	double total = 0;

	if(fProd != NULL){
		int i;

		for(i = 0; i < N_FILIAIS; ++i)
			total += fProd->faturacao[tipo][i];
	}
	return total;
}

/* Fim das funções utilizadas na query3 */

/* Funções utilizadas na criação da AVLs */

static int comparaFaturacaoProd(const void* p1, const void* p2)
{	
	return comparaCodigosProduto( ((FaturacaoProd) p1)->prod, 
		                          ((FaturacaoProd) p2)->prod );
}

static int comparaFoiVendido(const void* p1, const void* p2)
{
	return comparaCodigosProduto( ((FoiVendido) p1)->prod, 
		                          ((FoiVendido) p2)->prod );
}

static void atualizaFaturacaoProd(void* p1, void* p2)
{
	FaturacaoProd atual = p1;
	FaturacaoProd adicional = p2;
	int i, j;

	for(i = 0; i < N_TIPOS_VENDA; ++i){
		for(j = 0; j < N_FILIAIS; ++j){
			atual->vendas[i][j] += adicional->vendas[i][j];
			atual->faturacao[i][j] += adicional->faturacao[i][j];
		}
	}
	free(adicional);
}

static void atualizaFoiVendido(void* p1, void* p2)
{
	FoiVendido atual = p1;
	FoiVendido adicional = p2;
	int i;

	for(i = 0; i < N_FILIAIS; ++i)
		atual->vendido[i] = atual->vendido[i] || adicional->vendido[i];
}
