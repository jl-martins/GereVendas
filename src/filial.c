#include <stdlib.h>
#include "filial.h"
#include "avl.h"

typedef AVL AVL_VendaProd;
typedef AVL AVL_ComprasCliente;

/*temporario*/
typedef enum tipoVenda {N = 0, P} TipoVenda;

struct filial{
	AVL_ComprasCliente comprasClientes[26];	
};

typedef struct comprasCliente {
	Cliente cliente;
	AVL_VendaProd comprasPorMes[13]; 
} * ComprasCliente;

typedef struct vendaProduto {
	Produto produto;
	int vendas;
	double faturacao;
	bool modoP, modoN;
} *VendaProduto;

static int comparaComprasCliente(const void * cc1, const void * cc2){
	return comparaCodigosCliente(((ComprasCliente) cc1)->cliente, ((ComprasCliente) cc2)->cliente);		
}

static int comparaVendaProduto(const void * vp1, const void * vp2){
	return comparaCodigosProduto(((VendaProduto) vp1)->produto, ((VendaProduto) vp2)->produto);
}

static void atualizaVendaProduto(void * vp1, void * vp2){
	struct vendaProduto * vendaProd1 = vp1,
		            * vendaProd2 = vp2;
	vendaProd1->vendas += vendaProd2->vendas;
	vendaProd1->faturacao += vendaProd2->faturacao;
	vendaProd1->modoP = vendaProd2->modoP ? vendaProd2->modoP : vendaProd1->modoP;	
	vendaProd1->modoN = vendaProd2->modoN ? vendaProd2->modoN : vendaProd1->modoN;
	free(vendaProd2);
}

Filial criaFilial(){
	int i;
	Filial nova = malloc(sizeof(struct filial));		

	if(nova != NULL)
		for(i = 0; i < 26; i++)		
			nova->comprasClientes[i] = criaAVL(comparaComprasCliente);
	else	
		; /* ver tratamento de erros */
	return nova;
}
/* ver encapsulamento das funçoes */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco)
{
	int posicao = inicioCodigoCliente(cliente) - 'A';
	int i;
	struct vendaProduto * vendaProdAux = malloc(sizeof(struct vendaProduto));
	vendaProdAux->produto = produto;
	vendaProdAux->vendas = unidades;
	vendaProdAux->faturacao = preco * unidades;
	vendaProdAux->modoP = tipoVenda == P;
	vendaProdAux->modoN = tipoVenda == N;

	/* procura se existe cliente */	
	struct comprasCliente * ccliente = calloc(1, sizeof(struct comprasCliente));
	ccliente->cliente = cliente;
	struct comprasCliente * naAVL = procuraAVL(filial->comprasClientes[posicao], ccliente);
	
	if(naAVL == NULL){
		/* inicializar os campos */
		for(i = 1; i < 13; i++)
			ccliente->comprasPorMes[i] = criaAVLgenerica(comparaVendaProduto, atualizaVendaProduto);
		ccliente->comprasPorMes[mes] = insere(ccliente->comprasPorMes[mes], vendaProdAux);
		filial->comprasClientes[posicao] = insere(filial->comprasClientes[posicao], ccliente);
	}
	else{
		free(ccliente);
		insere(naAVL->comprasPorMes[mes], vendaProdAux); /*nota: a funçao de atualização deve fazer o free no caso de atualizar */
	}	
	return filial;		
}


