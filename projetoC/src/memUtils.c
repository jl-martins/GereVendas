/************************************************************
 * 															*
 * Módulo com funções úteis para alocar e desalocar memória *
 *															*
 ************************************************************/

#include "memUtils.h"
#include <stdlib.h>

void apagaArray(void* array[], int total, void (*apagaElem) (void*))
{
	int i;

	for(i = 0; i < total; ++i)
		apagaElem(array[i]);
	free(array);
}

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
