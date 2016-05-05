/* Programa que dá informação relativa às vendas de um produto */
/* uso: infoProduto [Produtos] */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parseVenda.h"
#include "vendasVal.h"

#define NCLIENTES 20000
#define SIZE_CLIENT 7
#define BUFFSIZE 128

int existe(char * nome, char lista[][SIZE_CLIENT], int n){
	int i;
	for(i = 0; i < n; i++)
		if(!strcmp(nome, lista[i])) return 1;
	return 0;
}

int main(int argc, char ** argv){
	char * produto;
	char clientes[NCLIENTES][SIZE_CLIENT];

	int clientesDif, vendasTotais, i, j, mes;
	int vendasN[13], vendasP[13];
	double faturacaoTotal;
	double faturacaoN[13], faturacaoP[13];
	char buf[BUFFSIZE];
	FILE * fp;

	fp = fopen(VENDAS, "r");
	if(fp == NULL){
		fprintf(stderr, "Não foi possível abrir o ficheiro de vendas\n");
		return -1;
	}
	for(i = 1; i < argc; i++){
		produto = argv[i];
		fseek(fp, 0, SEEK_SET);
		
		/* inicializacoes para cada prod */
		for(j = 1; j < 13; j++) vendasN[j] = vendasP[j] = 0, faturacaoP[j] = faturacaoN[j] = 0;
		clientesDif = 0;
		vendasTotais = 0;
		faturacaoTotal = 0.0;
		
		while(fgets(buf, BUFFSIZE, fp)){
			parseVenda(buf);
			mes = getMes();
			if(!strcmp(getProduto(), produto)){
				if(getTipoCompra() == PROMOCAO){ 
					vendasP[mes] += getNUnidades();
					faturacaoP[mes] += getPreco() * getNUnidades();
				} else { 
					vendasN[mes] += getNUnidades();
					faturacaoN[mes] += getPreco() * getNUnidades();
				}
				if(!existe(getCliente(), clientes, clientesDif))
					strcpy(clientes[clientesDif++], getCliente());
			}
		}
		printf("===================================\n"); 
		printf("Numero de Clientes %d\n", clientesDif);
		for(j = 1; j < 13; j++){
			if(faturacaoN[j]){
				printf("Mes = %d Vendas N = %d Faturacao N = %.2f\n", j, vendasN[j], faturacaoN[j]);
				vendasTotais += vendasN[j];
				faturacaoTotal += faturacaoN[j];
			}
			if(faturacaoP[j]){
				printf("Mes = %d Vendas P = %d Faturacao P = %.2f\n", j, vendasP[j], faturacaoP[j]);
				vendasTotais += vendasP[j];
				faturacaoTotal += faturacaoP[j];
			}	
		}
		printf("Total de Vendas = %d\nFaturacao total = %.2f\n", vendasTotais, faturacaoTotal);
	}
	return 0;
}
