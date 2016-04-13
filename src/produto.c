#include "produto.h"
#include <stdlib.h>
#include <string.h>

struct produto {
	char codigoProduto[MAX_CODIGO_PROD];
};

Produto criaProduto(char* codigoProduto) {
	Produto novo = malloc(sizeof(struct produto));
	
	if(novo)
		strcpy(novo->codigoProduto, codigoProduto);
	return novo;
}

Produto duplicaProduto(Produto p){
	Produto novo = malloc(sizeof(struct produto));
	if(novo)
		strcpy(novo->codigoProduto, p->codigoProduto);
	return novo;
}

void removeProduto(Produto p) {
	free(p);
}

char* obterCodigoProduto(Produto prod){
	char* copiaCodigo = malloc(MAX_CODIGO_PROD * sizeof(char));
	
	if(copiaCodigo)
		strcpy(copiaCodigo, prod->codigoProduto);
	return copiaCodigo;
}

/* Possibilidade: criar algo do género 'charAt()'. Assim ficarmos
   com uma API mais genérica. Para obter a primeira letra do código
   do produto poderiamos fazer 'caraterPos(p, 0)' */
char inicioCodigoProduto(Produto prod) {
	return prod->codigoProduto[0];
}

int comparaCodigosProduto(Produto p1, Produto p2){
	strcmp(p1->codigoProduto, p2->codigoProduto);
}
