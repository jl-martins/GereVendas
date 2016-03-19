/* Programa que le do ficheiro de vendas validas o numero de vendas por filial e a respetiva faturação*/
#include <stdlib.h>
#include <stdio.h>

#include "../venda.h"
#include "test.h"
#include "parseVenda.h"

#define NFILIAIS 3
#define BUFFSIZE 128

int main(){
	/* o indice 0 deve ser ignorado */
    int i, unidadesVend = 0, lim, vendas[NFILIAIS+1] = {};
    double faturacao[NFILIAIS+1] = {};
    double faturacaoTotal = 0, preco;
    int filial, unidades;
    FILE * fp;
    char buf[BUFFSIZE];

    fp = fopen(VENDAS, "r");
    if(fp == NULL){
    	fprintf(stderr, "Não foi possível abrir o ficheiro de vendas\n");
    	return -1;
    }

    while(fgets(buf, BUFFSIZE, fp)){
    	parseVenda(buf);
    	filial = getFilial();
    	unidadesVend += getNUnidades();
    	faturacao[filial] += (getNUnidades() * getPreco());
    	vendas[filial]++;
    }

    lim = NFILIAIS + 1;
    for(i = 1; i < lim; i++){
    	faturacaoTotal += faturacao[i];
    	printf("Vendas da Filial %d: %d  Faturacao: %.2f\n", i, vendas[i], faturacao[i]);
    }
    printf("Faturacao total = %.2f  Vendas totais: %d\n", faturacaoTotal, unidadesVend);
    return 0;
}