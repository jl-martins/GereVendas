#ifndef CATALOGO_PRODS_H
#define CATALOGO_PRODS_H

#include "produto.h" /* definição do tipo Produto */
#include "bool.h"

/* Tipos exportados */
typedef struct catProds* CatProds;
typedef struct conjProds* ConjuntoProds;

/* Funções sobre CatProds */

/* Cria um catálogo de produtos */
CatProds criaCatProds();
/* Insere um produto num catálogo de produtos */
CatProds insereProduto(CatProds, Produto);
/* Testa se um produto existe num catálogo de produtos */
bool existeProduto(CatProds, Produto);
/* Devolve o número total de produtos de um catálogo de produtos */
int totalProdutos(CatProds);
/* Devolve o número total de produtos de um catálogo de produtos
 * cujo código começa pela letra especificada */
int totalProdutosLetra(CatProds, char);
/* Liberta a memória alocada para um catálogo de produtos */
void apagaCatProds(CatProds);

/* Funções sobre ConjuntoProds */

/* Recebe o número total de produtos e um array de produtos
 * (sem repetições) e cria um conjunto de produtos. */
ConjuntoProds criaConjuntoProds(int, Produto*);
/* Liberta a memória alocada para um conjunto de produtos */
void removeConjuntoProds(ConjuntoProds);
/* inicializa as vars ligadas á paginação e navegação*/
ConjuntoProds setCodsPorPag(ConjuntoProds, int);
/* Devolve um array de strings com todos os códigos de um conjunto de produtos */
char** obterCodigosP(ConjuntoProds);
/* Devolve um array de strings com todos os códigos de uma página do conjunto de produtos*/
char** obterCodigosPPag(ConjuntoProds);
/* Devolve a cardinalidade de um conjunto de produtos */
int cardinalidade(ConjuntoProds);
/* Devolve o número da página atual de um conjunto de produtos */
int obterPag(ConjuntoProds conjuntoP);
/* Devolve o índice do 1º elemento da página atual de um conjunto de produtos */
int obterIndice(ConjuntoProds);
/* Dado um catálogo de produtos e uma letra, retorna um conjunto
 * com os produtos cujo código começa pela letra fornecida */
ConjuntoProds prodsPorLetra(CatProds, char);
/*Avança 1 página*/
void nextPage(ConjuntoProds);
/*Retrocede 1 página */
void prevPage(ConjuntoProds);
/*Avança para a ultima página*/
void lastPage(ConjuntoProds);
/*Avança para a primeira página*/
void fstPage(ConjuntoProds);
/*Avança para uma página passada como argumento*/
int goToPage(ConjuntoProds, int);


#endif
