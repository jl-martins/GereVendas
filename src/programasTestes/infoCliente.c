/* Programa que dá informação relativa às compras dos clientes */
/* uso: infoCliente [Clientes] */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../venda.h"
#include "parseVenda.h"
#include "test.h"

#define BUFFSIZE 128

int main(int argc, char ** argv){
	char * gajo, buf[BUFFSIZE];
	int i, j, nCompras, quantidadeTotal;
        int comprasMeses[13]; 
	double totalPago;
	FILE * fp = fopen(VENDAS, "r");

	for(i = 1; i < argc; i++){
		gajo = argv[i];
		fseek(fp, 0, SEEK_SET);

		/* inicializações para cada cliente */
		for(j = 1; j <13; j++) comprasMeses[j] = 0;
		nCompras = 0;
		totalPago = 0;
		quantidadeTotal = 0;

		while(fgets(buf, BUFFSIZE, fp)){
			parseVenda(buf);
			if(!strcmp(getCliente(), gajo)){
				comprasMeses[getMes()]++;
				nCompras++;
				totalPago += getNUnidades() * getPreco();
				quantidadeTotal += getNUnidades();
			}
		}
		printf("===================================\n");
		printf("Cliente: %s\n", gajo);
		printf("Total de Registos de Compras = %d\n", nCompras); 
		printf("Quantidade Total de Produtos Comprados = %d\n", quantidadeTotal);
		printf("Total Pago pelo Cliente = %.2f\n", totalPago);

		printf("Compras mensais:\n");
		for(j = 1; j < 13; j++){
			printf("Mes %d Compras %d\n", j, comprasMeses[j]);
		}
		printf("===================================\n");
	}

	return 0;
}
