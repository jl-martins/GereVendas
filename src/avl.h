#ifndef AVL_H
#define AVL_H

#include "bool.h"


typedef struct TCD_AVL* AVL;  
typedef void* ValorNodo;

typedef void (*Atualizador) (void *, void *);
typedef int (*Comparador) (const void *, const void *);
typedef ValorNodo (*Duplicador) (void *);
typedef void (*LibertarNodo) (void *);
/* Predicado usado na filtraAVL() */
typedef bool (*Predicado) (const void *);

/* Cria uma AVL vazia, que vai utilizar a função de comparação 'compara' e
 * A função de comparação é obrigatória. As restantes funções são opcionais.
 * Se não for fornecida uma função de atualização, a AVL criada admite repetições.
 * Se não for passada uma função de libertação de nodos, a AVL criada utiliza a 
 * função free() para libertar cada nodo. Se o utilizador não passar uma função
 * de duplicação, não é criada uma cópia aquando da inserção de um valor na AVL
 * e as funções de inorder() e procuraAVL() devolvem o conteúdo da própria AVL.  */
AVL criaAVLgenerica(Atualizador atualiza, Comparador compara, Duplicador duplica, LibertarNodo liberta);

/* Insere uma cópia de um valor 'val' na AVL 'arvore' */
AVL insere(AVL arvore, ValorNodo val);

/* Apaga uma AVL (i.e.: liberta toda a memória alocada para a mesma) */
void apagaAVL(AVL arvore);

/* Devolve o número de nodos de uma AVL */
int tamanho(const AVL arvore);

/* Devolve a altura de uma AVL */
int altura(const AVL arvore);

/* Devolve um array com os nodos resultantes da travessia inorder da AVL */
ValorNodo* inorder(const AVL arv);

/* Procura um valor numa AVL. Devolve uma cópia do valor, se este for encontrado.
 * Se não, devolve NULL. */
ValorNodo procuraAVL(const AVL arv, ValorNodo val);

/* Testa se um valor existe num AVL */
bool existeAVL(const AVL arv, ValorNodo val);

/* cria uma AVL que permite repetições, que não copia os seus elementos
 * em operações como procuraAVL() e inorder() e usa free() para libertar cada nodo */
#define criaAVL(comp) (criaAVLgenerica(NULL, comp, NULL, NULL))

#endif
