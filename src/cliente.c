#include "cliente.h"
#include <stdlib.h> /* para usar a função malloc() */
#include <string.h>

#define MAX_CODIGO_CLIENTE 8

struct cliente {
	char codigoCliente[MAX_CODIGO_CLIENTE];
};

Cliente criaCliente(char * codigoCliente){
	Cliente novo = (Cliente) malloc(sizeof(struct cliente));
	
	if(!novo){
		/* ver codigo de erros */ ;
	}
	strcpy(novo -> codigoCliente, codigoCliente);
	return novo;
}

char* obterCodigoCliente(Cliente c){
	char* copiaCodigo = (char*) malloc(MAX_CODIGO_CLIENTE * sizeof(char));
	/* strdup não faz parte do ANSI C. É uma função POSIX */
	if(copiaCodigo)
		strcpy(copiaCodigo, c->codigoCliente);
	return copiaCodigo;
}