#include <stdlib.h>
#include "filial.h"

#include "avl.h"

typedef AVL AVL_ComprasDoProduto;
typedef AVL AVL_ComprasPorCliente;

struct filial{
	AVL_ComprasPorCliente clientesOrdenados[26];	
};

typedef struct comprasPorCliente {
	Cliente cliente;
	AVL_ComprasDoProduto comprasPorMes[13]; 
} * ComprasPorCliente;

typedef struct comprasDoProduto {
	Produto produto;
	int unidades;
	double faturacao;
	bool modoP, modoN;
} * ComprasDoProduto;

static int somaUnidadesMes(AVL_ComprasPorCliente arv);
static int comparaComprasPorCliente(const void * cc1, const void * cc2);
static int comparaComprasDoProduto(const void * vp1, const void * vp2);
static void atualizaComprasDoProduto(void * vp1, void * vp2);
 
static int comparaComprasPorCliente(const void * cc1, const void * cc2){
	return comparaCodigosCliente(((ComprasPorCliente) cc1)->cliente, ((ComprasPorCliente) cc2)->cliente);		
}

static int comparaComprasDoProduto(const void * vp1, const void * vp2){
	return comparaCodigosProduto(((ComprasDoProduto) vp1)->produto, ((ComprasDoProduto) vp2)->produto);
}

static void atualizaComprasDoProduto(void * c1, void * c2){
	ComprasDoProduto compraProd1 = c1,
		             compraProd2 = c2;

	compraProd1->unidades += compraProd2->unidades;
	compraProd1->faturacao += compraProd2->faturacao;
	/* definir isto no bool.h*/
	compraProd1->modoP = compraProd2->modoP ? compraProd2->modoP : compraProd1->modoP;	
	compraProd1->modoN = compraProd2->modoN ? compraProd2->modoN : compraProd1->modoN;
	free(compraProd2);
}

static ComprasPorCliente apagaComprasPorCliente(ComprasPorCliente cpc){
	int i;
	apagaCliente(cpc->cliente);
	for(i=1; i < 13; i++)
		apagaAVL(cpc->comprasPorMes[i]);
	free(cpc);
	return NULL;
}

static ComprasDoProduto apagaComprasDoProduto(ComprasDoProduto cdp){
	free(cdp);
	return NULL;
}

static ComprasDoProduto duplicaComprasDoProduto(ComprasDoProduto cdp){
	ComprasDoProduto novo = malloc(sizeof(struct comprasDoProduto));
	novo->produto = cdp->produto;
	novo->unidades = cdp->unidades;
	novo->faturacao = cdp->faturacao;
	novo->modoP = cdp->modoP;
	novo->modoN = cdp->modoN;

}

Filial criaFilial(){
	int i;
	Filial nova = malloc(sizeof(struct filial));		

	if(nova != NULL)
		for(i = 0; i < 26; i++)		
			nova->clientesOrdenados[i] = criaAVLgenerica(NULL, comparaComprasPorCliente, NULL, (LibertarNodo) apagaComprasPorCliente);/*nao devia passar função de atualização??*/
	/*else	
		;  ver tratamento de erros */
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
	ComprasDoProduto novo;
	novo = malloc(sizeof(struct comprasDoProduto));
	novo->produto = duplicaProduto(produto);
	novo->unidades = unidades;
	novo->faturacao = preco * unidades;
	novo->modoP = tipoVenda == P; /* usar valores do bool.h ou definir macro toBool(x) (!!x) */
	novo->modoN = tipoVenda == N;
	return novo;
}


/* ver encapsulamento das funçoes */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco)
{
	if(filial == NULL) return NULL;
	ComprasPorCliente ccliente;
	int posicao, i;
	ComprasPorCliente naAVL;
	ComprasDoProduto comprasAux;

	posicao = inicioCodigoCliente(cliente) - 'A';
	comprasAux = criaComprasDoProduto(produto, unidades, preco, tipoVenda);

	/* procura se existe cliente */	
	ccliente = calloc(1, sizeof(struct comprasPorCliente));
	ccliente->cliente = duplicaCliente(cliente);
	naAVL = procuraAVL(filial->clientesOrdenados[posicao], ccliente);
	
	if(naAVL == NULL){
		/* inicializar os campos */
		for(i = 1; i < 13; i++)
			ccliente->comprasPorMes[i] = criaAVLgenerica(atualizaComprasDoProduto, comparaComprasDoProduto, duplicaComprasDoProduto, apagaComprasDoProduto);
		ccliente->comprasPorMes[mes] = insere(ccliente->comprasPorMes[mes], comprasAux);
		filial->clientesOrdenados[posicao] = insere(filial->clientesOrdenados[posicao], ccliente);
	}
	else{
		free(ccliente);
		insere(naAVL->comprasPorMes[mes], comprasAux); /*nota: a funçao de atualização deve fazer o free no caso de atualizar */
	}	
	return filial;		
}

/* codigo replicado porque em cima é necessario  o valor de ccliente para o resto da função */
static ComprasPorCliente procuraClienteNasVendas(Cliente cliente, Filial filial){
	int posicao;
	ComprasPorCliente naAVL;
	ComprasPorCliente ccliente;
	posicao = inicioCodigoCliente(cliente) - 'A';
	ccliente = malloc(sizeof(struct comprasPorCliente));
	ccliente->cliente = cliente;
	naAVL = procuraAVL(filial->clientesOrdenados[posicao], ccliente);
	return naAVL;
}

/* funçoes para queries */
/*querie 5*/
/* devolve um array com 13 entradas(12 validas) que à entrada i daz corresponder o numero de unidades do mes i */
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

static int somaUnidadesMes(AVL_ComprasPorCliente arv){
	int i, tamanhoArv, soma = 0;
	/* mudar o nome de vendaProduto para CompraProduto*/
	ComprasDoProduto * produtosComprados = (ComprasDoProduto *) inorder(arv);
	tamanhoArv = (produtosComprados != NULL)? tamanho(arv) : 0;
	for(i = 0; i < tamanhoArv; i++) soma += produtosComprados[i]->unidades;
	free(produtosComprados);	
	return soma;
}

/*querie 7 */
/* nota: para evitar duplicacoes, os elementos do tipo Cliente e Produto só vao ser libertados no modulo de catalogos*/
/* esta querie vai ser implementada com um merge das 3 listas */
Cliente * clientesCompraramNaFilial(Filial filial){
	int tamanhoArv = 0, quantasCompras;
	int i, j, lim, k;
	Cliente * clientes;
	ComprasPorCliente* compras;

	for(i = 0; i < 26; i++){
		tamanhoArv += tamanho(filial->clientesOrdenados[i]);
	}		
	clientes = malloc(sizeof(Cliente) * tamanhoArv);
	i = 0;
	for(j = 0; j < 26; j++){
		compras = (ComprasPorCliente *) inorder(filial->clientesOrdenados[i]);
		if(compras == NULL) return NULL;
		quantasCompras = tamanho(filial->clientesOrdenados[i]); 		
		lim = i + quantasCompras;		
		for(k=0; i < lim; i++, k++)
			clientes[i] = compras[k]->cliente; 
		free(compras);				
	}
	return clientes;
}

/* fazer funçoes que libertam avls alocadas (mas nao Produtos e Vendas) e Produtos * e Vendas * (talvez ecapsular Produto *) */

/*querie 8*/

