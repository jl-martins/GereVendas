#include <stdlib.h> /* para usar a função malloc() */
#include <string.h>
#include "cliente.h"

struct cliente {
	char codigoCliente[MAX_CODIGO_CLIENTE];
};

Cliente criaCliente(char* codigoCliente) {
	Cliente novo = malloc(sizeof(struct cliente));
	
	if(novo)
		strcpy(novo->codigoCliente, codigoCliente);
	return novo;
}

Cliente duplicaCliente(Cliente c){
	Cliente novo = malloc(sizeof(struct cliente));
	if(novo)
		strcpy(novo->codigoCliente, c->codigoCliente);
	return novo;
}

Cliente apagaCliente(Cliente c) {
	free(c);
	return NULL;
}

char* obterCodigoCliente(Cliente c) {
	char* copiaCodigo = malloc(MAX_CODIGO_CLIENTE * sizeof(char));
	
	if(copiaCodigo)
		strcpy(copiaCodigo, c->codigoCliente);
	return copiaCodigo;
}

char inicioCodigoCliente(Cliente c){
	return c->codigoCliente[0];
}

int comparaCodigosCliente(Cliente c1, Cliente c2){
	return strcmp(c1->codigoCliente, c2->codigoCliente);
}
