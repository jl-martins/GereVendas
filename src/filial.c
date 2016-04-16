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

typedef AVL AVL_ComprasDoProduto;
typedef AVL AVL_ComprasPorCliente;

struct filial{
	AVL_ComprasPorCliente clientesOrdenados[26];	
};

typedef struct comprasPorCliente {
	Cliente cliente;
	/* usar o indice 0 como info de todo o ano */
	AVL_ComprasDoProduto comprasPorMes[13]; 
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
	novo->modoP = tipoVenda == P; /* usar valores do bool.h ou definir macro toBool(x) (!!x) */
	novo->modoN = tipoVenda == N;
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
		char* codigo = obterCodigoCliente(ccliente->cliente);
		/* inicializar os campos */
		for(i = 1; i < 13; i++){
			ccliente->comprasPorMes[i] = criaAVLgenerica(atualizaComprasDoProduto, comparaComprasDoProduto, duplicaComprasDoProduto, apagaNodoComprasDoProduto);
			printf("Cliente: %s | Mês: %d | End: %p\n", codigo, i, (void *) ccliente->comprasPorMes[i]);
		}
		ccliente->comprasPorMes[mes] = insere(ccliente->comprasPorMes[mes], comprasAux);
		printf("ccliente->comprasPorMes[mes] = %p\n", (void *) ccliente->comprasPorMes[mes]);
		filial->clientesOrdenados[posicao] = insere(filial->clientesOrdenados[posicao], ccliente);
		free(codigo);
	}
	else{
		printf("Cliente: %p\n", (void *) naFilial->cliente);	
		perror("erro: ");
		sleep(5);
		free(ccliente); /* falta liberta o código de cliente duplicado */
		insere(naFilial->comprasPorMes[mes], comprasAux);  /*nota: a funçao de atualização deve fazer o free no caso de atualizar */
	}	
	return filial;		
}

/* codigo replicado porque em cima é necessario  o valor de ccliente para o resto da função */
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
	return nasVendas;
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

