#include <stdlib.h> /* para usar a função malloc() */
#include <string.h>
#include "cliente.h"

struct cliente {
	char* codigoCliente;
};

Cliente criaCliente(char* codigoCliente)
{
	Cliente novo = malloc(sizeof(struct cliente));
	int len;
	
	if(novo == NULL)
		return NULL;

	len = strlen(codigoCliente);
	novo->codigoCliente = malloc((len + 1) * sizeof(char));

	if(novo->codigoCliente != NULL)
		strcpy(novo->codigoCliente, codigoCliente);
	else{ /* falha de alocação de novo->codigoCliente */
		free(novo);
		novo = NULL;
	}
	return novo;
}

Cliente duplicaCliente(Cliente c)
{
	Cliente copia = malloc(sizeof(struct cliente));
	int len;
	
	if(copia == NULL)
		return NULL;

	len = strlen(c->codigoCliente);
	copia->codigoCliente = malloc((len + 1) * sizeof(char));

	if(copia->codigoCliente != NULL)
		strcpy(copia->codigoCliente, c->codigoCliente);
	else{ /* falha de alocação de copia->codigoCliente */
		free(copia);
		copia = NULL;
	}
	return copia;
}

Cliente apagaCliente(Cliente c)
{
	if(c){
		free(c->codigoCliente);
		free(c);
	}
	return NULL;
}

char* obterCodigoCliente(Cliente c)
{	
	int len = strlen(c->codigoCliente);
	char* copiaCodigo = malloc((len + 1) * sizeof(char));
	
	if(copiaCodigo)
		strcpy(copiaCodigo, c->codigoCliente);
	return copiaCodigo;
}

char inicioCodigoCliente(Cliente c)
{
	return c->codigoCliente[0];
}

int comparaCodigosCliente(Cliente c1, Cliente c2)
{
	return strcmp(c1->codigoCliente, c2->codigoCliente);
}
