/**
 * @file avl.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com os TAD e API para criar e manipular AVLs genéricas.
 */

#ifndef AVL_H
#define AVL_H
#include "bool.h"

typedef struct TCD_AVL* AVL;
/** Tipo de função que permite atualizar nodos de uma AVL. */
typedef void (*Atualizador) (void *, void *);
/** Tipo da função a utilizar na comparação das chaves de dois nodos. */
typedef int (*Comparador) (const void *, const void *);
/** Tipo da função que permite criar um duplicado do valor de um nodo. */
typedef void* (*Duplicador) (void *);
/** Tipo da função usada para libertar o valor contido num nodo. */
typedef void (*LibertarNodo) (void *);

/** 
 * Cria uma AVL vazia, que vai utilizar a função de comparação 'compara' e
 * A função de comparação é obrigatória. As restantes funções são opcionais.
 * Se não for fornecida uma função de atualização, a AVL criada admite repetições.
 * Se não for passada uma função de libertação de nodos, a AVL criada utiliza a 
 * função free() para libertar cada nodo. Se o utilizador não passar uma função
 * de duplicação, não é criada uma cópia aquando da inserção de um valor na AVL
 * e as funções de inorderAVL() e procuraAVL() devolvem o conteúdo da própria AVL.
 * @return Em caso de sucesso é retornada a AVL criada. Quando não é fornecida
 * uma função de comparação ou há uma falha de alocação, é devolvido NULL.
 */
AVL criaAVL(Atualizador atualiza, Comparador compara, Duplicador duplica, LibertarNodo liberta);

/**
 * @brief Insere um valor numa AVL previamente criada. Se tiver sido passado um duplicador aquando 
 * da criacão da arvore, o valor inserido é uma cópia do original. Caso contrário, é o original
 * @param arv Árvore AVL onde o valor será inserido 
 * @param val Valor a inserir 
 * @return Árvore atualizada com o valor inserido em caso de sucesso. NULL em caso de falha
 * de alocação.
 */
AVL insereAVL(AVL arvore, void* val);

/** 
 * Apaga uma AVL (i.e.: liberta toda a memória alocada para a mesma) 
 * @param arvore Arvore que vai ser libertada 
 * @return NULL
 */
AVL apagaAVL(AVL arvore);

/** @brief Devolve o número de nodos de uma AVL */
int tamanhoAVL(const AVL arvore);

/** @brief Devolve a altura de uma AVL */
int alturaAVL(const AVL arvore);

/**
 * Devolve um array com os valores dos nodos (cópias se a função de duplicação
 * tiver sido passada durante a criação da AVL) resultante da travessia inorder da AVL.
 */
void** inorderAVL(const AVL arv);

/**
 * Procura um valor numa AVL. Se este for encontrado e tiver sido passada uma função
 * de duplicação na altura da criação da árvore, é devolvida uma uma cópia do valor, se não
 * é devolvido o próprio valor que está na árvore. Se o valor não for encontrado ou ocorrer
 * uma falha de alocação, é devolvido NULL. 
 * @warning Usar esta função com muito cuidado quando a árvore não foi criada com função de duplicação.
 */
void* procuraAVL(const AVL arv, void* val);

/**
 * Testa se um determinado valor existe numa AVL
 * @param arv Árvore AVL onde o valor será procurado
 * @param val Valor a procurar
 * @return TRUE se o valor existir; FALSE caso contrário.
 */
bool existeAVL(const AVL arv, void* val);

#endif
