#define DEBUG_MODE 1

#if DEBUG_MODE
	#include <stdio.h>
	#define DEBUG(X) printf(X); 
#else
	#define DEBUG(X)
#endif

#include <stdlib.h>
#include "filial.h"

#include "avl.h"
#include "memUtils.h"

typedef AVL AVL_ComprasDoProduto;
typedef AVL AVL_ComprasPorCliente;

struct filial{
	AVL_ComprasPorCliente clientesOrdenados[26];	
};

typedef struct comprasPorCliente {
	Cliente cliente;
	/* usar o indice 0 como info de todo o ano */
	AVL_ComprasDoProduto comprasPorMes[13]; /* ao indice 0 corresponde as compras durante todo o ano, ao indice i corresponde a compra no mes i*/
}* ComprasPorCliente;

typedef struct comprasDoProduto {
	Produto produto;
	int unidades;
	double faturacao;
	bool modoP, modoN;
}* ComprasDoProduto;

static int somaUnidadesMes(AVL_ComprasPorCliente arv);
static int comparaComprasPorCliente(const void* cc1, const void* cc2);
static int comparaComprasDoProduto(const void* vp1, const void* vp2);
static void atualizaComprasDoProduto(void* vp1, void* vp2);
static void apagaComprasDoProduto(ComprasDoProduto cdp);
static int comparaTotalCompras(const void* cdp1, const void* cdp2);

static void atualizaComprasDoProduto(void* c1, void* c2)
{
	ComprasDoProduto compraProd1 = c1, 
					 compraProd2 = c2;

	compraProd1->unidades += compraProd2->unidades;
	compraProd1->faturacao += compraProd2->faturacao;
	/* Esta atualização poder ser feita com ||, uma vez que FALSE
	 * está definido como 0 e TRUE está definido como 1 e || só
	 * pode devolver 0 ou 1 */
	compraProd1->modoP = compraProd1->modoP || compraProd2->modoP;	
	compraProd1->modoN = compraProd1->modoN || compraProd2->modoN;
	/*apagaComprasDoProduto(compraProd2);*/
}

static int comparaComprasPorCliente(const void* cc1, const void* cc2)
{
	return comparaCodigosCliente(((ComprasPorCliente) cc1)->cliente, ((ComprasPorCliente) cc2)->cliente);		
}

static int comparaComprasDoProduto(const void* vp1, const void* vp2)
{
	return comparaCodigosProduto(((ComprasDoProduto) vp1)->produto, ((ComprasDoProduto) vp2)->produto);
}

/* Função usada para libertar um nodo da AVL_ComprasPorCliente */
static void apagaNodoComprasPorCliente(void* p)
{
	int i;
	ComprasPorCliente cpc = p;

	apagaCliente(cpc->cliente);
	for(i = 1; i < 13; i++)
		apagaAVL(cpc->comprasPorMes[i]);
	free(cpc);
}

static void apagaComprasPorCliente(ComprasPorCliente cpc)
{
	apagaNodoComprasPorCliente((void*) cpc);
}

/* Função usada para libertar um nodo da AVL_ComprasDoProduto */
static void apagaNodoComprasDoProduto(void* p)
{
	if(p != NULL){
		ComprasDoProduto cdp = p;
		apagaProduto(cdp->produto);
		free(cdp);
	}
}

static void apagaComprasDoProduto(ComprasDoProduto cdp)
{
	apagaNodoComprasDoProduto((void *) cdp);
}

static ValorNodo duplicaComprasDoProduto(void* p){
	ComprasDoProduto original = p;
	ComprasDoProduto copia = NULL;
	
	if(original != NULL){
		copia = malloc(sizeof(struct comprasDoProduto));

		if(copia == NULL)
			return NULL;

		if((copia->produto = duplicaProduto(original->produto)) == NULL){
			free(copia);
			return NULL;
		}
		copia->unidades = original->unidades;
		copia->faturacao = original->faturacao;
		copia->modoP = original->modoP;
		copia->modoN = original->modoN;
	}
	return (ValorNodo) copia;
}

/* verificada */
Filial criaFilial(){
	int i;
	Filial nova = malloc(sizeof(struct filial));		

	if(nova == NULL) /* falha de alocação */
		return NULL;

	for(i = 0; i < 26; i++){		
		/* definir função de comparação */
		nova->clientesOrdenados[i] = criaAVLgenerica(NULL, comparaComprasPorCliente, NULL, apagaNodoComprasPorCliente);/*nao devia passar função de atualização??*/
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

	if((novo->produto = duplicaProduto(produto)) == NULL){
		free(novo);
		return NULL;
	}
	novo->unidades = unidades;
	novo->faturacao = preco * unidades;
	novo->modoP = (tipoVenda == P); /* usar valores do bool.h ou definir macro toBool(x) (!!x) */
	novo->modoN = (tipoVenda == N);
	return novo;
}

/* ver encapsulamento das funçoes */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
				TipoVenda tipoVenda, int unidades, double preco)
{
	int posicao;
	ComprasPorCliente naFilial;
	ComprasPorCliente ccliente;
	ComprasDoProduto comprasAux;

	if(filial == NULL)
		return NULL;
	posicao = inicioCodigoCliente(cliente) - 'A';
	comprasAux = criaComprasDoProduto(produto, unidades, preco, tipoVenda);
	
	ccliente = malloc(sizeof(struct comprasPorCliente));
	if(ccliente == NULL){
		apagaComprasDoProduto((void*) comprasAux);
		return NULL;
	}
	/* procura se existe cliente */
	ccliente->cliente = duplicaCliente(cliente);
	naFilial = procuraAVL(filial->clientesOrdenados[posicao], ccliente);

	if(naFilial == NULL){ /* é a primeira vez que o cliente compra na filial */
		int i;
		/*char* codigo = obterCodigoCliente(ccliente->cliente);*/
		/* inicializar os campos */
		for(i = 0; i < 13; i++)
			ccliente->comprasPorMes[i] = criaAVLgenerica(atualizaComprasDoProduto, comparaComprasDoProduto, duplicaComprasDoProduto, apagaNodoComprasDoProduto);

		ccliente->comprasPorMes[mes] = insere(ccliente->comprasPorMes[mes], comprasAux);
		ccliente->comprasPorMes[0] = insere(ccliente->comprasPorMes[0], comprasAux); /* insere na informação anual do cliente */
		/*printf("ccliente->comprasPorMes[%d] = %p\n",  mes, (void *) ccliente->comprasPorMes[mes]);*/
		filial->clientesOrdenados[posicao] = insere(filial->clientesOrdenados[posicao], ccliente);
		/*naFilial = procuraAVL(filial->clientesOrdenados[posicao], ccliente);*/
		/*printf("naFilial: %p | ccliente: %p | ccliente->comprasPorMes[%d]: %p\n", (void *) naFilial, (void *) ccliente, mes, (void *) naFilial->comprasPorMes[mes]);
		free(codigo);*/
	}
	else{
		free(ccliente); /* falta liberta o código de cliente duplicado */
		insere(naFilial->comprasPorMes[mes], comprasAux);  /*nota: a funçao de atualização deve fazer o free no caso de atualizar */
		insere(naFilial->comprasPorMes[0], comprasAux);
	}	
	/* limpar comprasAux */
	return filial;		
}

/* codigo replicado porque em cima é necessario  o valor de ccliente para o resto da função */
/* alterar API para apresentar primeiro a filial?*/
static ComprasPorCliente procuraClienteNasVendas(Cliente cliente, Filial filial){
	int posicao;
	ComprasPorCliente nasVendas; /* mudei o nome de 'naAVL' para 'nasVendas' porque a meu ver fica mais claro */
	ComprasPorCliente ccliente;
	
	posicao = inicioCodigoCliente(cliente) - 'A';
	ccliente = malloc(sizeof(struct comprasPorCliente));
	if(ccliente == NULL)
		return NULL;
	ccliente->cliente = cliente;
	nasVendas = procuraAVL(filial->clientesOrdenados[posicao], ccliente);
	free(ccliente); /*nao estou a usar a API para limpar */
	return nasVendas;
}

bool clienteComprouNaFilial(Filial filial, Cliente cliente){
	return procuraClienteNasVendas(cliente, filial)? TRUE : FALSE;
}

/* fazer o existe à custa do procura */

/* funçoes para queries */
/*query 5*/
/* devolve um array com 13 entradas(12 validas) que à entrada i faz corresponder o numero de unidades do mes i */
int * unidadesClientePorMes(Filial filial, Cliente cliente){
	int i;
	int * unidades; 
	ComprasPorCliente comprasDoCliente;
	unidades = calloc(13, sizeof(int));
	comprasDoCliente = procuraClienteNasVendas(cliente, filial);
	if(comprasDoCliente){ 
		for (i = 1; i < 13; i++){
			unidades[i] = somaUnidadesMes(comprasDoCliente->comprasPorMes[i]);
		}
	}
	return unidades;
}

int quantosClientesCompraram(Filial filial){
	int quantos, i;
	quantos = 0;
	for(i = 0; i < 26; i++){
		quantos += tamanho(filial->clientesOrdenados[i]);
	}
	return quantos;
}

static int somaUnidadesMes(AVL_ComprasPorCliente arv){
	int i, tamanhoArv, soma = 0;
	/* mudar o nome de vendaProduto para CompraProduto*/
	ComprasDoProduto * produtosComprados = (ComprasDoProduto *) inorder(arv);
	tamanhoArv = (produtosComprados != NULL)? tamanho(arv) : 0;
	for(i = 0; i < tamanhoArv; i++) soma += produtosComprados[i]->unidades;
	free(produtosComprados);	
	return soma;
}

/* fazer funçoes que libertam avls alocadas (mas nao Produtos e Vendas) e Produtos * e Vendas * (talvez ecapsular Produto *) */



/* funçoes uteis para querie 8*/
void comprou(Filial filial, Cliente cliente, Produto produto, int * comprouN, int * comprouP){
	ComprasDoProduto paraComparar, resultadoProcura; 
	ComprasPorCliente cpc;

	cpc = procuraClienteNasVendas(cliente, filial);		
	*comprouN = FALSE;
	*comprouP = FALSE;

	if(cpc){
		/*procurar na AVL*/ 	
		paraComparar = criaComprasDoProduto(produto, 0, 0, N);
		if(paraComparar){
			resultadoProcura = procuraAVL(cpc->comprasPorMes[0], paraComparar);
			if(resultadoProcura){
				*comprouN = resultadoProcura->modoN;
				*comprouP = resultadoProcura->modoP;
				apagaComprasDoProduto(resultadoProcura);
				/*printf("%d %d\n", *comprouN, * comprouP);*/
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

	if(cpc != NULL){ /* o cliente foi encontrado */
		int i, total = tamanho(cpc->comprasPorMes[mes]); /* adicionar macro COMPRAS_DO_ANO(x) x->comprasPorMes[0] */
		ComprasDoProduto* arrComprasDoMes = malloc(total * sizeof(ComprasDoProduto));
		char** codigosProds;

		if(arrComprasDoMes == NULL) /* falha de alocação */
			return NULL;
		
		codigosProds = malloc(total * sizeof(char *));
		if(codigosProds == NULL){
			apagaArray((void **) arrComprasDoMes, total, free);
			return NULL;
		}
		/* Falta verificar o resultado da inorder() */
		arrComprasDoMes = (ComprasDoProduto *) inorder(cpc->comprasPorMes[0]);
		qsort(arrComprasDoMes, total, sizeof(ComprasDoProduto), comparaTotalCompras);

		for(i = 0; i < total; ++i){
			char* codigoProd = obterCodigoProduto(arrComprasDoMes[i]->produto);
			if(codigoProd == NULL){ /* falha de alocação */
				apagaArray((void **) arrComprasDoMes, total, apagaNodoComprasDoProduto);
				apagaArray((void **) codigosProds, i, free);
				return NULL;
			}
			codigosProds[i] = codigoProd;
		}
		res = criaLStrings(total, codigosProds);
		apagaArray((void **) arrComprasDoMes, total, apagaNodoComprasDoProduto);
		apagaArray((void **) codigosProds, total, free);
	}
	return res;
}

/* Função de compração passada para qsort(), na função produtosClienteMaisComprou() */
static int comparaTotalCompras(const void* cdp1, const void* cdp2)
{
	return ((ComprasDoProduto) cdp2)->unidades - ((ComprasDoProduto) cdp1)->unidades;
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
	int i, imin;
	ComprasPorCliente cpc = procuraClienteNasVendas(c, filial);
	int total = tamanho(cpc->comprasPorMes[0]);
	char** codigosProds = calloc(3, sizeof(char *));
	/* guarda na posição i o total gasto no produto cujo código está em codigosProds[i] */
	double totalGasto[3] = {0}; 
	ComprasDoProduto* arrComprasDoAno;

	if(codigosProds == NULL || cpc == NULL) /* não faz sentido prosseguir */
		return NULL;
	
	arrComprasDoAno = malloc(total * sizeof(ComprasDoProduto));
	if(arrComprasDoAno == NULL) /* falha de alocação */
		return NULL;
		
	/* Falta verificar o resultado da inorder() */
	arrComprasDoAno = (ComprasDoProduto *) inorder(cpc->comprasPorMes[0]);
	for(i = 0; i < total; ++i){ /* percorre o array de compras do ano */
		imin = indiceDoMenor(totalGasto);
		if(arrComprasDoAno[i]->faturacao > totalGasto[imin]){
			char* codigo = obterCodigoProduto(arrComprasDoAno[i]->produto);

			if(codigo == NULL){ /* falha de alocação em obterCodigoProduto() */
				apagaArray((void **) arrComprasDoAno, total, apagaNodoComprasDoProduto);
				apagaArray((void **) codigosProds, 3, free);
				return NULL;
			}
			free(codigosProds[imin]);
			codigosProds[imin] = codigo;
			totalGasto[imin] = arrComprasDoAno[i]->faturacao;
		}
	}
	apagaArray((void **) arrComprasDoAno, total, apagaNodoComprasDoProduto);
	return codigosProds;
}
