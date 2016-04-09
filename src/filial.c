#include "avl.h"
#include "cliente.h"
#include "produto.h"

typedef AVL AVL_VendaProd;
typedef AVL AVL_ComprasCliente;

struct filial{
	AVL_ComprasCliente comprasClientes[26];	
}

struct comprasCliente {
	Cliente cliente;
	AVL_VendaProd comprasPorMes[13]; 
}

struct vendaProduto {
	Produto produto;
	int vendas;
	int faturacao;
	bool modoP, modoN;
}

Filial criaFilial(){
	int i;
	Filial nova = (Filial) malloc(sizof(struct filial));		

	if(nova != NULL)
		for(i = 0; i < 26; i++)		
			nova->comprasClientes[i] = NULL/* criaAVL(...)*/ ;
	else	
		; /* ver tratamento de erros */
	return nova;
}
/* ver encapsulamento das funçoes */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco)
{
	int posicao = inicioCodigoCliente(cliente) - 'A';
	if(filial->compras[posicao] == NULL)
		filial->compras[posicao] = criaAVL(...);
	if(!existeAVL(filial->compras[posicao], cliente)){
		
	}
		
}
