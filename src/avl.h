#ifndef AVL_H
#define AVL_H
#include "bool.h"

typedef struct TCD_AVL* AVL;  
typedef void (*Atualizador) (void *, void *);
typedef int (*Comparador) (const void *, const void *);
typedef void* (*Duplicador) (void*);
typedef void (*LibertarNodo) (void *);

/* Cria uma AVL vazia, que vai utilizar a função de comparação 'compara' e
 * A função de comparação é obrigatória. As restantes funções são opcionais.
 * Se não for fornecida uma função de atualização, a AVL criada admite repetições.
 * Se não for passada uma função de libertação de nodos, a AVL criada utiliza a 
 * função free() para libertar cada nodo. Se o utilizador não passar uma função
 * de duplicação, não é criada uma cópia aquando da inserção de um valor na AVL
 * e as funções de inorderAVL() e procuraAVL() devolvem o conteúdo da própria AVL.  */
AVL criaAVL(Atualizador atualiza, Comparador compara, Duplicador duplica, LibertarNodo liberta);

/* Insere uma cópia de um valor 'val' na AVL 'arvore' */
AVL insereAVL(AVL arvore, void * val);

/* Apaga uma AVL (i.e.: liberta toda a memória alocada para a mesma) */
AVL apagaAVL(AVL arvore);

/* Devolve o número de nodos de uma AVL */
int tamanhoAVL(const AVL arvore);

/* Devolve a altura de uma AVL */
int alturaAVL(const AVL arvore);

/* Devolve um array com os valores dos nodos(cópias se a função de duplicação tiver sido passada durante a criação da AVL) resultante da travessia inorder da AVL */
void ** inorderAVL(const AVL arv);

/* Procura um valor numa AVL. Devolve uma cópia do valor (se a função duplica tiver sido passada na altura da criação da árvore), se este for encontrado. Senão, devolve NULL. */
void * procuraAVL(const AVL arv, void * val);

/**
 * Testa se um determinado valor existe numa AVL
 * @param arv Árvore AVL onde o valor será procurado
 * @param val Valor a procurar
 * @return TRUE se o valor existir; FALSE caso contrário.
 */
bool existeAVL(const AVL arv, void * val);

#endif
