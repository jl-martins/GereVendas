#include "produto.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CODIGO_PROD 8

struct produto{
	char codigoProduto[MAX_CODIGO_PROD];
};

Produto criaProduto(char * codigoProduto){
	Produto novo = (Produto) malloc(sizeof(struct produto));
	
	if(!novo){
		/* ver codigo de erros */ ;
	}
	strcpy(novo -> codigoProduto, codigoProduto);
	return novo;
}

char* obterCodigoProduto(Produto prod){
	char* copiaCodigo = (char*) malloc(MAX_CODIGO_PROD * sizeof(char));
	/* strdup não faz parte do ANSI C. É uma função POSIX */
	if(copiaCodigo)
		strcpy(copiaCodigo, prod->codigoProduto);
	return copiaCodigo;
}
