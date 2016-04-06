#define MESES 13
#define N_INDICES 26

#include "avl.h"

typedef struct faturacaoProduto{
	char nomeProd[MAX_PRODUTO];
	int vendasN[N_FILIAIS][MESES], vendasP[N_FILIAIS][MESES];
	double faturacaoN[N_FILIAIS][MESES], faturacaoP[N_FILIAIS][MESES];
} * FaturacaoProduto;

struct fatGlobal {
	AVL catalogo[N_INDICES];
/*	int vendasNTotais[N_FILIAIS][MESES], vendasPTotais[N_FILIAIS][MESES];
	double faturacaoNTotal[N_FILIAIS], faturacaoPTotal[N_FILIAIS];*/
}; /*passar definição do tipo para o .h*/

FaturacaoGlobal criaFaturacaoGlobal(){
	int i;
	FaturacaoGlobal fg = (FaturacaoGlobal) malloc(sizeof(fatGlobal));
	if(fg == NULL)
	/* ver tratamento de erros */
		;
	for(i = 0; i < N_INDICES; i++){
		catalogo[i] = criaAVL(compara, atualiza);
	}	
	return fg;
}

FaturacaoGlobal contabiliza(FaturacaoProduto fg, const Venda){
	
}
