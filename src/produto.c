#include "produto.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CODIGO_PROD 8

struct produto {
	char codigoProduto[MAX_CODIGO_PROD];
};

Produto criaProduto(char* codigoProduto) {
	Produto novo = malloc(sizeof(struct produto));
	
	if(novo != NULL)
		strcpy(novo -> codigoProduto, codigoProduto);
	return novo;
}

void removeProduto(Produto p) {
	free(p);
}

char* obterCodigoProduto(Produto prod) {
	char* copiaCodigo = malloc(MAX_CODIGO_PROD * sizeof(char));
	
	if(copiaCodigo != NULL)
		strcpy(copiaCodigo, prod->codigoProduto);
	return copiaCodigo;
}

/* Possibilidade: criar algo do género 'charAt()'. Assim ficarmos
   com uma API mais genérica. Para obter a primeira letra do código
   do produto poderiamos fazer 'caraterPos(p, 0)' */
char inicioCodigoProduto(Produto prod) {
	return prod->codigoProduto[0];
}