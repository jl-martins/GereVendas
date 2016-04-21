/************************************************************
 * 															*
 * Módulo com funções úteis para alocar e desalocar memória *
 *															*
 ************************************************************/

#include "memUtils.h"
#include <stdlib.h>

/* Apaga um array de estruturas */
void apagaArray(void* array[], int total, void (*apagaElem) (void*))
{
	int i;

	for(i = 0; i < total; ++i)
		apagaElem(array[i]);
	free(array);
}

/* Recebe o #linhas e #colunas de uma matriz e o número de bytes
 * de cada elemento. Aloca uma matriz com as dimensões especificadas. */
void** alocaMatriz(int nlinhas, int ncolunas, size_t nBytesElem)
{	
	int i;
	void** mat = malloc(nlinhas * sizeof(void *));

	if(mat == NULL)
		return NULL;

	for(i = 0; i < nlinhas; ++i){
		mat[i] = malloc(ncolunas * nBytesElem);
		
		if(mat[i] == NULL){ /* falha a alocar a linha i */
			apagaArray(mat, i, free);
			return NULL;
		}
	}
	return mat;
}
