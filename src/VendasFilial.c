#include "avl.h"
#define N_ELEMS 26

struct vendasCliente {
	Cliente cliente;
	AVL vendas[13]; /*avls de vendaProduto */
}

struct vendasFilial {
	AVL clientesPorLetra[N_ELEMS]; /*avls de vendasCliente */
}

struct vendaProduto {
	Produto produto;
	int vendas[N_FILIAIS];
	int faturacao[N_FILIAIS];
	bool modoP[N_FILIAIS], modoN[N_FILIAIS];
}

VendasFilial criaVendasFilial(){
	int i;
	VendasFilial nova = malloc(sizeof(struct vendasFilial));
	if(nova != NULL)
		for(i = 0; i < N_ELEMS; i++)		
			nova->clientesPorLetra[i] = criaAVL(/*  */)
	/* ver tratamento de erros */
	return nova;
}

/* definir tipo Filial e tipo de compra ?*/
VendasFilial insereVenda(Cliente cliente,Produto produto, int unidades, double preco, int filial, char tipoCompra){

}  
