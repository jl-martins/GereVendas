#ifndef CATALOGO_H
#define CATALOGO_H

#include "bool.h"

/* Catálogo genérico */
typedef struct cat* Catalogo;
/* Elemento de um catálogo */
typedef struct elem* Elem;
/* Conjunto de elementos de um catálogo */
typedef struct conjElems* ConjuntoElems;

/* Inicializa um catálogo */
Catalogo initCatalogo(int (*compara) (const void *, const void *));
/* Insere um elemento num catálogo */
Catalogo insereElem(Catalogo c, Elem e);
/* Testa se um elemento ocorre num catálogo */
bool existeElem(Catalogo c, Elem e);
/* Devolve o número total de elementos de um catálogo */
int totalProdutos(Catalogo c);
/* Devolve o número total de elementos começados por uma letra */
int totalProdutosLetra(Catalogo c, char l);
/* Apaga um catálogo */
void removeCatProds(Catalogo c);
/* Devolve conjunto com os elementos de um catálogo começados por uma letra */
ConjuntoElems elemsPorLetra(Catalogo c, char l);

#endif
