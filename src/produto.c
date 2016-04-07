#include "produto.h"
#include <string.h>

#define MAX_CODIGO_PROD 8

struct produto{
	char codigoProduto[MAX_CODIGO_PROD];
}

Produto criaProduto(char * codigoProduto){
	Produto novo = (Produto) malloc(sizeof(struct produto));
	if(!novo)
		/* ver codigo de erros */ ;
	strcpy(novo -> codigoProduto, codigoProduto);
	return novo;
}

char * codigoProduto(Produto prod){
	return strdup(prod->codigoProduto);
}
