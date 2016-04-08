#include "cliente.h"
#include <stdlib.h> /* para usar a função malloc() */
#include <string.h>

#define MAX_CODIGO_CLIENTE 8

struct cliente {
	char codigoCliente[MAX_CODIGO_CLIENTE];
};

Cliente criaCliente(char* codigoCliente) {
	Cliente novo = malloc(sizeof(struct cliente));
	
	if(novo != NULL)
		strcpy(novo -> codigoCliente, codigoCliente);
	return novo;
}

void removeCliente(Cliente c) {
	free(c);
}

char* obterCodigoCliente(Cliente c) {
	char* copiaCodigo = malloc(MAX_CODIGO_CLIENTE * sizeof(char));
	
	if(copiaCodigo != NULL)
		strcpy(copiaCodigo, c->codigoCliente);
	return copiaCodigo;
}

char inicioCodigoCliente(Cliente c) {
	return c->codigoCliente[0];
}
