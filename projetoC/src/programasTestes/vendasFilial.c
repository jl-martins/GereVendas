/* Programa que le do ficheiro de vendas validas o numero de vendas por filial e a respetiva faturação*/
#include <stdlib.h>
#include <stdio.h>

#include "vendasVal.h"
#include "parseVenda.h"

#define NFILIAIS 3
#define BUFFSIZE 128

int main(){
	/* o indice 0 deve ser ignorado */
    int i, lim, filial, unidades, vendas0 = 0, unidadesVend = 0;
    int vendas[NFILIAIS+1] = {0};
    double preco, faturacaoTotal = 0;
    double faturacao[NFILIAIS+1] = {0};
    char buf[BUFFSIZE];
    FILE * fp;

    fp = fopen(VENDAS, "r");
    if(fp == NULL){
    	fprintf(stderr, "Não foi possível abrir o ficheiro de vendas\n");
    	return -1;
    }

    while(fgets(buf, BUFFSIZE, fp)){
    	parseVenda(buf);

    	filial = getFilial();
	unidades = getNUnidades();
	preco = getPreco();
	
	if(preco == 0) vendas0++;

    	unidadesVend += unidades; 
    	faturacao[filial] += (unidades * preco);
    	vendas[filial]++;
    }

    lim = NFILIAIS + 1;
    for(i = 1; i < lim; i++){
    	faturacaoTotal += faturacao[i];
    	printf("Vendas da Filial %d: %d  Faturacao: %.2f\n", i, vendas[i], faturacao[i]);
    }
    printf("Faturacao total = %.2f  Vendas totais: %d\nVendas de preco 0: %d\n", faturacaoTotal, unidadesVend, vendas0);
    return 0;
}
