/* Funções auxiliares para ler os vários campos de uma venda válida */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../venda.h"

#define GET (strtok(NULL," "))

static char * codigoProduto;
static char * codigoCliente;
static double preco;
static int nUnidades;
static int mes;
static int filial;
static char tipoCompra;

/* falta fazer a verificação de erros */
void parseVenda(char * linha){
	codigoProduto = strtok(linha, " ");
	preco = atof(GET);
	nUnidades = atoi(GET);
	tipoCompra = GET[0];
	codigoCliente = GET;
	mes = atoi(GET);
	filial = atoi(GET);
}

int getFilial(void){
	return filial;
}

int getMes(void){
	return mes;
}

int getNUnidades(void){
	return nUnidades;
}

char getTipoCompra(void){
	return tipoCompra;
}

double getPreco(void){
	return preco;
}

char * getProduto(void){
	return codigoProduto;
}

char * getCliente(void){
	return codigoCliente;
}