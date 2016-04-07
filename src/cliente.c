#include "cliente.h"
#include <string.h>

#define MAX_CODIGO_CLIENTE 8

struct cliente {
	char codigoCliente[MAX_CODIGO_CLIENTE];
}

Cliente criaCliente(char * codigoCliente){
	Cliente novo = (Cliente) malloc(sizeof(struct cliente));
	if(!novo)
		/* ver codigo de erros */ ;
	strcpy(novo -> codigoCliente, codigoCliente);
	return novo;
}
