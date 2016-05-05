#include "filial.h"
#include "avl.h"
#include "memUtils.h"
#include <stdlib.h>
#include <string.h>

typedef AVL AVL_ComprasDoProduto;
typedef AVL AVL_ComprasPorCliente;

struct filial{
	AVL_ComprasPorCliente clientesOrdenados[26];	
};

typedef struct comprasPorCliente {
	char * cliente;
	/* O indice 0 dá-nos as compras do ano todo.
	 * O indice i corresponde a compra no mes i*/
	AVL_ComprasDoProduto comprasPorMes[13];
}* ComprasPorCliente;

typedef struct comprasDoProduto {
	char* produto;
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

		for(i = 0; i < 13; i++)
			cpc->comprasPorMes[i] = apagaAVL(cpc->comprasPorMes[i]);
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

static void * duplicaComprasDoProduto(void* p)
{
	ComprasDoProduto original = p;
	ComprasDoProduto copia = NULL;
	char * copiaProduto;
	
	if(original){
		copia = malloc(sizeof(struct comprasDoProduto));
		if(copia == NULL)
			return NULL;
	
		copiaProduto = malloc(sizeof(char) * (strlen(original->produto) + 1));
		if(copiaProduto == NULL){
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

Filial criaFilial()
{
	int i, j;
	Filial nova = malloc(sizeof(struct filial));		

	if(nova == NULL)
		return NULL;

	for(i = 0; i < 26; i++){		
		nova->clientesOrdenados[i] = criaAVL(NULL, comparaComprasPorCliente, NULL, apagaComprasPorCliente);
		if(nova->clientesOrdenados[i] == NULL){
			for (j = 0; j < i; j++)
				nova->clientesOrdenados[j] = apagaAVL(nova->clientesOrdenados[j]);
			nova = NULL; 
			break;
		}
	}
	return nova;
}

Filial apagaFilial(Filial filial)
{
	int i;

	if(filial){
		for(i = 0; i < 26; i++){
			filial->clientesOrdenados[i] = apagaAVL(filial->clientesOrdenados[i]);
		}
		free(filial);
	}
	return NULL;
}

/* aloca espaço e inicializa uma ComprasDoProduto */
static ComprasDoProduto criaComprasDoProduto(Produto produto, int unidades, double preco, TipoVenda tipoVenda)
{
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
	int posicao, i, j;
	ComprasPorCliente naFilial;
	ComprasPorCliente ccliente;
	ComprasDoProduto comprasAux;

	if(filial == NULL)
		return NULL;

	ccliente = malloc(sizeof(struct comprasPorCliente)); 
	if(ccliente == NULL) return NULL;

	comprasAux = criaComprasDoProduto(produto, unidades, preco, tipoVenda);
	if(comprasAux == NULL) {
		free(ccliente);
		return NULL;
	}

	/* procura se existe cliente */
	ccliente->cliente = obterCodigoCliente(cliente);
	if(ccliente->cliente == NULL){
		apagaComprasDoProduto(comprasAux);
		free(ccliente);
		return NULL;
	}

	posicao = ccliente->cliente[0] - 'A'; /* <=> 1a letra do cliente - 'A' */
	/* se o cliente já comprou na filial, devolve a estrutura que tem os valores originais para ser atualizada */
	naFilial = procuraAVL(filial->clientesOrdenados[posicao], ccliente);

	if(naFilial == NULL){ /* é a primeira vez que o cliente compra na filial */
		/* inicializar os campos */
		for(i = 0; i < 13; i++){
			ccliente->comprasPorMes[i] = criaAVL(atualizaComprasDoProduto, comparaComprasDoProduto, duplicaComprasDoProduto, apagaComprasDoProduto);
			/* falha de alocacao de espaco */
			if(ccliente->comprasPorMes[i] == NULL){
				apagaComprasDoProduto(comprasAux);
				free(ccliente->cliente);
				free(ccliente);	
				for(j = 0; j < i; j++)
					ccliente->comprasPorMes[j] = apagaAVL(ccliente->comprasPorMes[j]);
				return NULL;
			}

		}
		ccliente->comprasPorMes[mes] = insereAVL(ccliente->comprasPorMes[mes], comprasAux);
		ccliente->comprasPorMes[0] = insereAVL(ccliente->comprasPorMes[0], comprasAux); /* insere na informação anual do cliente */
		filial->clientesOrdenados[posicao] = insereAVL(filial->clientesOrdenados[posicao], ccliente);
	}
	else{ /* já havia compras do cliente registadas na filial */
		free(ccliente->cliente);
		free(ccliente);
		insereAVL(naFilial->comprasPorMes[mes], comprasAux); 
		insereAVL(naFilial->comprasPorMes[0], comprasAux);
	}	
	apagaComprasDoProduto(comprasAux);
	return filial;		
}

/* devolve o endereço da estrutura interna que contem os dados relativos às compras de um dado cliente */
static ComprasPorCliente procuraClienteNasVendas(Cliente cliente, Filial filial)
{
	int posicao;
	ComprasPorCliente nasVendas, ccliente;
	char* codigoCliente;

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

bool clienteComprouNaFilial(Filial filial, Cliente cliente)
{
	return procuraClienteNasVendas(cliente, filial)? TRUE : FALSE;
}

int* unidadesClientePorMes(Filial filial, Cliente cliente)
{
	int i;
	int* unidades; 
	ComprasPorCliente comprasDoCliente;
	
	unidades = calloc(13, sizeof(int));
	comprasDoCliente = procuraClienteNasVendas(cliente, filial);
	if(comprasDoCliente)
		for (i = 1; i < 13; i++)
			unidades[i] = somaUnidadesMes(comprasDoCliente->comprasPorMes[i]);
	return unidades;
}

/* Dada a AVL de ComprasPorCliente de um determinado mes (para um determinado produto),
 * indica quantas unidades foram compradas nesse mes */
static int somaUnidadesMes(AVL_ComprasDoProduto arv)
{
	int i, tamanhoArv, soma = 0;
	ComprasDoProduto* produtosComprados = (ComprasDoProduto *) inorderAVL(arv);

	tamanhoArv = (produtosComprados != NULL)? tamanhoAVL(arv) : 0;
	for(i = 0; i < tamanhoArv; i++) {
		soma += produtosComprados[i]->unidades;
		apagaComprasDoProduto(produtosComprados[i]);
	}
	free(produtosComprados);	
	return soma;
}

/* Devolve o número de clientes que compraram na filial */
int quantosClientesCompraram(Filial filial)
{
	int quantos, i;
	quantos = 0;
	for(i = 0; i < 26; i++){
		quantos += tamanhoAVL(filial->clientesOrdenados[i]);
	}
	return quantos;
}

/*Dado um cliente, uma filial e um produto, verifica se o cliente comprou em modo Normal e 
  se comprou em modo de promoção. os resultados sao guardados nas variaveis passadas cmo referencia */
void comprou(Filial filial, Cliente cliente, Produto produto, bool* comprouN, bool* comprouP)
{
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

LStrings produtosClienteMaisComprou(Filial filial, Cliente c, int mes)
{
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
	/* guarda na posição i o total gasto no produto cujo código está em codigosProds[i] */
	double totalGasto[3] = {0};
	ComprasDoProduto* arrComprasDoAno;

	cpc = procuraClienteNasVendas(c, filial);
	if(cpc == NULL)
		return NULL;

	codigosProds = calloc(3, sizeof(char *));
	if(codigosProds == NULL)
		return NULL;
	
	total = tamanhoAVL(cpc->comprasPorMes[0]);
	arrComprasDoAno = (ComprasDoProduto *) inorderAVL(cpc->comprasPorMes[0]);
	if(arrComprasDoAno == NULL){ /* falha de alocação */
		free(codigosProds);
		return NULL;
	}

	for(i = 0; i < total; ++i){ /* percorre o array de compras do ano */
		imin = indiceDoMenor(totalGasto);
		if(arrComprasDoAno[i]->faturacao > totalGasto[imin]){
			int len = strlen(arrComprasDoAno[i]->produto);
			char* copia = malloc((len + 1) * sizeof(char));

			if(copia == NULL){ /* falha de alocação */
				apagaArray((void **) codigosProds, 3, free);
				apagaArray((void **) arrComprasDoAno, total, apagaComprasDoProduto);
				return NULL;
			}
			free(codigosProds[imin]);
			strcpy(copia, arrComprasDoAno[i]->produto);
			codigosProds[imin] = copia;
			totalGasto[imin] = arrComprasDoAno[i]->faturacao;
		}
	}
	apagaArray((void **) arrComprasDoAno, total, apagaComprasDoProduto);
	ordenaTop3(codigosProds, totalGasto);
	return codigosProds;
}

/* Recebe um array (codigosProds[3]) com os códigos dos 3 produtos em que
 * um dado cliente gastou mais dinheiro e outro array (totalGasto[3]). 
 * totalGasto[i] indica a quantida que um dado cliente gastou no produto
 * cujo código está em codigosProds[i]. O array totalGasto[3] é então
 * ordenado decrescentemente e codigosProds[3] é permutado de forma a 
 * que a correspondência entre produtos e total gasto seja mantida. */
static void ordenaTop3(char* codigosProds[3], double totalGasto[3])
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

int numeroClientesCompraramProduto(Filial filial, char* produto, int* unidadesCompradas)
{
	int i, j, elems, unidades = 0, conta = 0;
	ComprasDoProduto encontrou;
	ComprasPorCliente* cpc;
	ComprasDoProduto paraComparar = malloc(sizeof(struct comprasDoProduto));
	
	if(paraComparar == NULL)
	    return -1;
	
	paraComparar->produto = produto;
	for(i = 0; i < 26; i++){
		cpc = (ComprasPorCliente*) inorderAVL(filial->clientesOrdenados[i]);
		if (cpc == NULL){
			free(paraComparar);
			return -1;	
		} 
		elems = tamanhoAVL(filial->clientesOrdenados[i]);
		for(j = 0; j < elems; j++){
		    encontrou = procuraAVL(cpc[j]->comprasPorMes[0], paraComparar);
			if(encontrou){
				unidades += encontrou->unidades;
				conta++;
			}
			apagaComprasDoProduto(encontrou);
		}
		free(cpc); /* não se apagam os elementos da filial porque sao os que estao na arvore */
	}
	*unidadesCompradas = unidades;
	free(paraComparar); /* não se usa funçoes da api para nao apagar o produto que esta na estrutura*/
	return conta;
}
