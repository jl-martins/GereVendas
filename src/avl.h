#ifndef AVL_H
#define AVL_H

#include "bool.h"

typedef struct TCD_AVL* AVL;  
typedef void* ValorNodo;

typedef void (*Atualizador)(void *, const void *);
typedef int (*Comparador) (const void *, const void *);

/* Cria uma AVL vazia, que vai utilizar a função de comparação 'compar' */
AVL criaAVLgenerica (Comparador compara, Atualizador atualiza);
/* Insere um valor na AVL 'arvore' */
AVL insere(AVL arvore, ValorNodo val);
/* Testa se o valor 'val' ocorre na AVL 'arvore' */
bool existeAVL(const AVL arvore, ValorNodo val);
/* Apaga uma AVL (i.e.: liberta toda a memória alocada para a mesma) */
AVL apagaAVL(AVL arvore);
/* Devolve o número de nodos de uma AVL */
int tamanho(const AVL arvore);
/* Devolve a altura de uma AVL */
int altura(const AVL arvore);
/* cria uma AVL cujos nodos com a mesma chave podem ser atualizados */
#define criaAVL(func) (criaAVLgenerica(func, NULL))

#endif
