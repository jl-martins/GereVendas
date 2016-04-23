/**
 * @file memUtils.h
 * @author LI3_Grupo1
 * @date 23 de Abril de 2016
 * @brief Ficheiro com funções genéricas para alocar e desalocar memória.
 */

#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include <stddef.h> /* inclui o tipo size_t */

/**
 * @brief Aloca memória para armazenar uma matriz.
 * @param nlinhas Número de linhas da matriz a alocar.
 * @param ncolunas Número de colunas da matriz a alocar.
 * @param nBytesElem Tamanho (em bytes) de cada elemento da matriz.
 * @return Matriz criada.
 */
void** alocaMatriz(int nlinhas, int ncolunas, size_t nBytesElem);

/**
 * @brief Liberta um array de apontadores e os tipos/estruturas
 * para que estes apontam.
 * @param array Array de apontadores.
 * @param total Comprimento do array.
 * @param apagaElem Função usada para apagar cada elemento do array.
 */
void apagaArray(void* array[], int total, void (*apagaElem) (void*));

#endif