#include <stdlib.h>
#include "filial.h"
#include "avl.h"

typedef AVL AVL_VendaProd;
typedef AVL AVL_ComprasCliente;

struct filial{
	AVL_ComprasCliente comprasClientes[26];	
};

struct comprasCliente {
	Cliente cliente;
	AVL_VendaProd comprasPorMes[13]; 
};

struct vendaProduto {
	Produto produto;
	int vendas;
	double faturacao;
	bool modoP, modoN;
};

int comparaComprasCliente(const void * cc1, const void * cc2){
	struct comprasCliente * cc_1 = (struct comprasCliente *) cc1,
			      * cc_2 = (struct comprasCliente *) cc2;
	return comparaCodigosCliente(cc_1->cliente, cc_2->cliente);		
}

int comparaVendaProduto(const void * vp1, const void * vp2){
	struct vendaProduto * vendaProd1 = vp1,
		            * vendaProd2 = vp2;
	return comparaCodigosProduto(vendaProd1->produto,vendaProd2->produto);
}

void atualizaVendaProduto(void * vp1, void * vp2){
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
	Filial nova = malloc(sizof(struct filial));		

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
	struct vendaProduto * vendaProdAux = malloc(sizeof(vendaProduto));
	vendaProdAux->produto = produto;
	vendaProdAux->vendas = unidades;
	vendaProdAux->faturacao = preco * unidades;
	vendas->modoP = tipoVenda == P;
	vendas->modoN = tipoVenda == N;

	/* procura se existe cliente */	
	struct comprasCliente * ccliente = calloc(sizeof(struct comprasCliente));
	ccliente->cliente = cliente;
	struct comprasCliente * naAVL = procuraAVL(filial->compras[posicao], ccliente);
	
	if(naAVL == NULL){
		/* inicializar os campos */
		ccliente->comprasPorMes[mes] = criaAVLgenerica(comparaVendaProduto, atualizaVendaProduto);
		ccliente->comprasPorMes[mes] = insere(ccliente->comprasPorMes[mes], vendaProdAux);
		filial->compras[posicao] = insere(filial->compras[posicao], ccliente);
	}
	else{
		free(ccliente);
		insere(naAVL, vendaProdAux); /*nota: a funçao de atualização deve fazer o free no caso de atualizar */
	}	
	return filial;		
}


