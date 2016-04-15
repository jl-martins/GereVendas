#include "memUtils.h"
#include <stdlib.h>

/* Apaga um array de estruturas */
void apagaArray(void** array, int total, void (*apagaElem) (void*))
{
	if(array != NULL){
		int i;

		for(i = 0; i < total; ++i)
			apagaElem(array[i]);
		free(array);
	}
}

/* Recebe o #linhas e #colunas de uma matriz e o número de bytes
 * de cada elemento. Aloca uma matriz com as dimensões especificadas. */
void** alocaMatriz(int nlinhas, int ncolunas, size_t nBytesElem)
{	
	int i;
	void** mat = malloc(nlinhas * sizeof(void *));

	if(mat == NULL)
		return NULL;

	for(i = 0; i < ncolunas; ++i){
		mat[i] = malloc(ncolunas * nBytesElem);
		
		if(mat[i] == NULL){ 
			for(i = i-1; i >= 0; --i)
				free(mat[i]);
			return NULL;
		}
	}
	return mat;
}
