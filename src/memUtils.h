#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include <stddef.h>

void** alocaMatriz(int nlinhas, int ncolunas, size_t nBytesElem);
void apagaArray(void** array, int total, void (*apagaElem) (void*));

#endif