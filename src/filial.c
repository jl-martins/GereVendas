#define DEBUG_MODE 1

#if DEBUG_MODE
	#include <stdio.h>
	#define DEBUG(X) printf(X); 
#else
	#define DEBUG(X)
#endif

#include <stdlib.h>
#include <string.h>
#include "filial.h"
#include "avl.h"
#include "memUtils.h"

typedef AVL AVL_ComprasDoProduto;
typedef AVL AVL_ComprasPorCliente;

struct filial{
	AVL_ComprasPorCliente clientesOrdenados[26];	
};

typedef struct comprasPorCliente {
	char * cliente;
	AVL_ComprasDoProduto comprasPorMes[13]; /* ao indice 0 corresponde as compras durante todo o ano, ao indice i corresponde a compra no mes i*/
}* ComprasPorCliente;

typedef struct comprasDoProduto {
	char * produto;
	int unidades;
	double faturacao;
	bool modoP, modoN;
}* ComprasDoProduto;

static int somaUnidadesMes(AVL_ComprasPorCliente arv);
static int comparaComprasPorCliente(const void* cc1, const void* cc2);
static int comparaComprasDoProduto(const void* vp1, const void* vp2);
static void atualizaComprasDoProduto(void* vp1, void* vp2);
static void apagaComprasDoProduto(void* p);
static int comparaTotalCompras(const void* p1, const void* p2);
static void ordenaTop3(char* codigosProds[3], double totalGasto[3]);

static void atualizaComprasDoProduto(void* c1, void* c2)
{
	ComprasDoProduto compraProd1 = c1, compraProd2 = c2;

	compraProd1->unidades += compraProd2->unidades;
	compraProd1->faturacao += compraProd2->faturacao;
	/* Esta atualização poder ser feita com ||, uma vez que FALSE
	 * está definido como 0 e TRUE está definido como 1 e || só
	 * pode devolver 0 ou 1 */
	compraProd1->modoP = compraProd1->modoP || compraProd2->modoP;	
	compraProd1->modoN = compraProd1->modoN || compraProd2->modoN;
}

static int comparaComprasPorCliente(const void* cc1, const void* cc2)
{
	return strcmp(((ComprasPorCliente) cc1)->cliente, ((ComprasPorCliente) cc2)->cliente);		
}

static int comparaComprasDoProduto(const void* vp1, const void* vp2)
{
	return strcmp(((ComprasDoProduto) vp1)->produto, ((ComprasDoProduto) vp2)->produto);
}

/* Função usada para libertar um nodo da AVL_ComprasPorCliente */
static void apagaComprasPorCliente(void* p)
{
	int i;
	ComprasPorCliente cpc = p;

	if(cpc != NULL){
		free(cpc->cliente);

		for(i = 1; i < 13; i++)
			apagaAVL(cpc->comprasPorMes[i]);
		free(cpc);
	}
}

/* Função usada para libertar um nodo da AVL_ComprasDoProduto */
static void apagaComprasDoProduto(void* p)
{
	if(p != NULL){
		ComprasDoProduto cdp = p;
		free(cdp->produto);
		free(cdp);
	}
}

static void * duplicaComprasDoProduto(void* p){
	ComprasDoProduto original = p;
	ComprasDoProduto copia = NULL;
	char * copiaProduto;
	
	if(original){
		copia = malloc(sizeof(struct comprasDoProduto));
		copiaProduto = malloc(sizeof(char) * (strlen(original->produto) + 1));

		if(copia == NULL)
			/* ver codigo de erros*/
			return NULL;

		if(copiaProduto == NULL){
			/* ver codigo de erros */
			free(copia);
			return NULL;
		}

		strcpy(copiaProduto, original->produto);

		copia->produto = copiaProduto;
		copia->unidades = original->unidades;
		copia->faturacao = original->faturacao;
		copia->modoP = original->modoP;
		copia->modoN = original->modoN;
	}

	return (void *) copia;
}

/* verificada */
Filial criaFilial(){
	int i;
	Filial nova = malloc(sizeof(struct filial));		

	if(nova == NULL) /* falha de alocação */
		return NULL;

	for(i = 0; i < 26; i++){		
		/* definir função de comparação */
		nova->clientesOrdenados[i] = criaAVL(NULL, comparaComprasPorCliente, NULL, apagaComprasPorCliente);
	}
	return nova;
}

Filial apagaFilial(Filial filial){
	int i;

	if(filial){
		for(i = 0; i < 26; i++){
			apagaAVL(filial->clientesOrdenados[i]);
		}
		free(filial);
	}
	return NULL;
}

static ComprasDoProduto criaComprasDoProduto(Produto produto, int unidades, double preco, TipoVenda tipoVenda){
	ComprasDoProduto novo = malloc(sizeof(struct comprasDoProduto));

	if(novo == NULL) /* falha a alocar a struct comprasDoProduto */
		return NULL;

	if((novo->produto = obterCodigoProduto(produto)) == NULL){
		free(novo);
		return NULL;
	}

	novo->unidades = unidades;
	novo->faturacao = preco * unidades;
	novo->modoP = (tipoVenda == P)? TRUE : FALSE; 
	novo->modoN = (tipoVenda == N)? TRUE : FALSE;
	return novo;
}

Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
				TipoVenda tipoVenda, int unidades, double preco)
{
	int posicao;
	ComprasPorCliente naFilial;
	ComprasPorCliente ccliente;
	ComprasDoProduto comprasAux;

	if(filial == NULL)
		return NULL;

	ccliente = malloc(sizeof(struct comprasPorCliente));
	comprasAux = criaComprasDoProduto(produto, unidades, preco, tipoVenda);

	if(ccliente == NULL || comprasAux == NULL){
		apagaComprasDoProduto((void*) comprasAux);
		apagaComprasPorCliente(ccliente);
		return NULL;
	}

	/* procura se existe cliente */
	ccliente->cliente = obterCodigoCliente(cliente); /*verificar se dá null */
	posicao = ccliente->cliente[0] - 'A'; /* <=> 1a letra do cliente - 'A' */
	naFilial = procuraAVL(filial->clientesOrdenados[posicao], ccliente); /* devolve a estrutura que tem os valores originais para ser atualizada */

	if(naFilial == NULL){ /* é a primeira vez que o cliente compra na filial */
		int i;
		/* inicializar os campos */
		for(i = 0; i < 13; i++)
			ccliente->comprasPorMes[i] = criaAVL(atualizaComprasDoProduto, comparaComprasDoProduto, duplicaComprasDoProduto, apagaComprasDoProduto);
		
		ccliente->comprasPorMes[mes] = insereAVL(ccliente->comprasPorMes[mes], comprasAux);
		ccliente->comprasPorMes[0] = insereAVL(ccliente->comprasPorMes[0], comprasAux); /* insere na informação anual do cliente */
		filial->clientesOrdenados[posicao] = insereAVL(filial->clientesOrdenados[posicao], ccliente);
	}
	else{
		free(ccliente->cliente);
		free(ccliente);
		insereAVL(naFilial->comprasPorMes[mes], comprasAux); 
		insereAVL(naFilial->comprasPorMes[0], comprasAux);
	}	
	apagaComprasDoProduto(comprasAux);
	return filial;		
}

/* codigo replicado porque em cima é necessario  o valor de ccliente para o resto da função */
/* alterar API para apresentar primeiro a filial?*/
static ComprasPorCliente procuraClienteNasVendas(Cliente cliente, Filial filial){
	int posicao;
	ComprasPorCliente nasVendas, ccliente;
	char * codigoCliente;

	ccliente = malloc(sizeof(struct comprasPorCliente));
	if(ccliente == NULL)
		return NULL;
	
	codigoCliente = obterCodigoCliente(cliente);
	if(codigoCliente == NULL){
		free(ccliente);
		return NULL;
	}
	posicao = codigoCliente[0] - 'A';

	ccliente->cliente = codigoCliente;
	nasVendas = procuraAVL(filial->clientesOrdenados[posicao], ccliente);
	free(codigoCliente); free(ccliente);
	return nasVendas;
}

bool clienteComprouNaFilial(Filial filial, Cliente cliente){
	return procuraClienteNasVendas(cliente, filial)? TRUE : FALSE;
}

/* funçoes uteis para queries */

/* devolve um array com 13 entradas(12 validas) que à entrada i faz corresponder o numero de unidades compradas
 no mes i pelo cliente */
int * unidadesClientePorMes(Filial filial, Cliente cliente){
	int i;
	int * unidades; 
	ComprasPorCliente comprasDoCliente;
	unidades = calloc(13, sizeof(int));
	comprasDoCliente = procuraClienteNasVendas(cliente, filial);
	if(comprasDoCliente)
		for (i = 1; i < 13; i++)
			unidades[i] = somaUnidadesMes(comprasDoCliente->comprasPorMes[i]);
	return unidades;
}

/* dada a AVL de ComprasPorCliente de um determinado mes, indica quantas unidades se compraram nessa filial e nesse mes */
static int somaUnidadesMes(AVL_ComprasDoProduto arv){
	int i, tamanhoArv, soma = 0;

	ComprasDoProduto * produtosComprados = (ComprasDoProduto *) inorderAVL(arv);
	tamanhoArv = (produtosComprados != NULL)? tamanhoAVL(arv) : 0;
	for(i = 0; i < tamanhoArv; i++) soma += produtosComprados[i]->unidades;
	free(produtosComprados);	
	return soma;
}

/* devolve quantos clientes compraram numa filial */
int quantosClientesCompraram(Filial filial){
	int quantos, i;
	quantos = 0;
	for(i = 0; i < 26; i++){
		quantos += tamanhoAVL(filial->clientesOrdenados[i]);
	}
	return quantos;
}

/* funçoes uteis para querie 8*/
void comprou(Filial filial, Cliente cliente, Produto produto, int * comprouN, int * comprouP){
	ComprasDoProduto paraComparar, resultadoProcura; 
	ComprasPorCliente cpc;

	cpc = procuraClienteNasVendas(cliente, filial);		
	*comprouN = FALSE;
	*comprouP = FALSE;

	if(cpc){
		paraComparar = criaComprasDoProduto(produto, 0, 0, N);
		if(paraComparar){
			resultadoProcura = procuraAVL(cpc->comprasPorMes[0], paraComparar);
			if(resultadoProcura){
				*comprouN = resultadoProcura->modoN;
				*comprouP = resultadoProcura->modoP;
				apagaComprasDoProduto(resultadoProcura);
			}
			apagaComprasDoProduto(paraComparar);
		}
	}
}

/* Recebe o número de uma filial e um cliente. Devolve uma lista de strings, com códigos
 * dos produtos comprados pelo cliente, em ordem decrescente da quantidade comprada. */
LStrings produtosClienteMaisComprou(Filial filial, Cliente c, int mes){
	LStrings res = NULL;
	ComprasPorCliente cpc = procuraClienteNasVendas(c, filial);

	if(cpc){ /* o cliente foi encontrado */
		int i, total = tamanhoAVL(cpc->comprasPorMes[mes]);
		ComprasDoProduto* arrComprasDoMes;
		char** codigosProds;
		
		codigosProds = malloc(total * sizeof(char *));
		if(codigosProds == NULL)
			return NULL;

		arrComprasDoMes = (ComprasDoProduto *) inorderAVL(cpc->comprasPorMes[mes]);
		if(arrComprasDoMes == NULL){
			free(codigosProds);
			return NULL;
		}

		qsort(arrComprasDoMes, total, sizeof(ComprasDoProduto), comparaTotalCompras);

		for(i = 0; i < total; ++i)
			codigosProds[i] = arrComprasDoMes[i]->produto;
		
		res = criaLStrings(total, codigosProds);
		apagaArray((void **) arrComprasDoMes, total, apagaComprasDoProduto);
		free(codigosProds);
	}
	return res;
}

/* Função de compração passada para qsort(), na função produtosClienteMaisComprou() */
static int comparaTotalCompras(const void* p1, const void* p2)
{	
	const ComprasDoProduto cdp1 = *(ComprasDoProduto *) p1;
	const ComprasDoProduto cdp2 = *(ComprasDoProduto *) p2;
	return cdp2->unidades - cdp1->unidades;
}

/* Função auxiliar de tresProdsEmQueMaisGastou().
 * Dado um array com 3 doubles, devolve o índice do seu menor elemento. */
static int indiceDoMenor(double totalGasto[3])
{
	int i, imin = 0;

	for(i = 1; i < 3; ++i)
		if(totalGasto[i] < totalGasto[imin])
			imin = i;
	return imin;

}

/* Devolve o array com os códigos dos 3 produtos em que um
 * cliente gastou mais dinheiro durante o ano, para uma dada filial. */
char** tresProdsEmQueMaisGastou(Filial filial, Cliente c)
{	
	int i, imin, total;
	ComprasPorCliente cpc;
	char** codigosProds;
	double totalGasto[3] = {0};
	/* guarda na posição i o total gasto no produto cujo código está em codigosProds[i] */
	ComprasDoProduto* arrComprasDoAno;

	cpc = procuraClienteNasVendas(c, filial);
	if(cpc == NULL)
		return NULL;

	codigosProds = calloc(3, sizeof(char *));
	
	if(codigosProds == NULL){
		apagaComprasDoProduto(cpc);
		return NULL;
	}
	
	total = tamanhoAVL(cpc->comprasPorMes[0]);
	arrComprasDoAno = malloc(total * sizeof(ComprasDoProduto));
	if(arrComprasDoAno == NULL){ /* falha de alocação */
		apagaComprasDoProduto(cpc); free(codigosProds);
		return NULL;
	}
		
	/* Falta verificar o resultado da inorderAVL() */
	arrComprasDoAno = (ComprasDoProduto *) inorderAVL(cpc->comprasPorMes[0]);
	for(i = 0; i < total; ++i){ /* percorre o array de compras do ano */
		imin = indiceDoMenor(totalGasto);
		if(arrComprasDoAno[i]->faturacao > totalGasto[imin]){
			free(codigosProds[imin]);
			codigosProds[imin] = arrComprasDoAno[i]->produto;
			totalGasto[imin] = arrComprasDoAno[i]->faturacao;
		}
	}
	ordenaTop3(codigosProds, totalGasto);
	for(i = 0; i < total; ++i)
		free(arrComprasDoAno[i]);
	free(arrComprasDoAno);
	return codigosProds;
}

/* Recebe um array (codigosProds[3]) com os códigos dos 3 produtos em que
 * um dado cliente gastou mais dinheiro e outro array (totalGasto[3]). 
 * totalGasto[i] indica a quantida que um dado cliente gastou no produto
 * cujo código está em codigosProds[i]. O array totalGasto[3] é então
 * ordenado decrescentemente e codigosProds[3] é permutado de forma a 
 * que a correspondência entre produtos e total gasto seja mantida. */
void ordenaTop3(char* codigosProds[3], double totalGasto[3])
{
	int i, j;
	
	for(i = 1; i < 3; ++i){
		double totalGastoAtual = totalGasto[i];
		char* codigoAtual = codigosProds[i];
		for(j = i-1; j >= 0 && totalGasto[j] < totalGastoAtual; --j){
			totalGasto[j+1] = totalGasto[j];
			codigosProds[j+1] = codigosProds[j];
		}
		totalGasto[j+1] = totalGastoAtual;
		codigosProds[j+1] = codigoAtual;
	}
}

int numeroClientesCompraramProduto(Filial filial, char * produto, int * unidadesCompradas){
	int i, elems, unidades = 0, conta = 0, j;
	ComprasDoProduto encontrou;
	ComprasDoProduto paraComparar = malloc(sizeof(struct comprasDoProduto));
	if(paraComparar == NULL)
	    return -1;
	paraComparar->produto = produto;
	for(i = 0; i < 26; i++){
		ComprasPorCliente * cpc = (ComprasPorCliente*) inorderAVL(filial->clientesOrdenados[i]);
		if (cpc == NULL) return -1;
		elems = tamanhoAVL(filial->clientesOrdenados[i]);
		for(j = 0; j < elems; j++){
		    encontrou = procuraAVL(cpc[j]->comprasPorMes[0], paraComparar);
			if(encontrou){
				unidades += encontrou->unidades;
				conta++;
			}
			apagaComprasDoProduto(encontrou);
		}
		free(cpc); /*nao se apagam os elementos da filial porque sao os que estao na arvore */
	}
	*unidadesCompradas = unidades;
	free(paraComparar); /*nao se usa funçoes da api para nao apagar o produto que esta na estrutura*/
	return conta;
}
