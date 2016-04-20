#ifndef LSTRINGS_H
#define LSTRINGS_H

#define STRINGS_POR_PAG 32

typedef struct lStrings* LStrings;
typedef struct pagina* Pagina;

/* Cria uma LStrings com o tamanho e com os elementos especificados nos argumentos */
LStrings criaLStrings(int tamanho, char* elems[]);

/* Liberta a memória alocada para armazenar uma LStrings */
LStrings apagaLStrings(LStrings l);

/* Funções de navegação */

/* Devolve a página atual de uma LStrings ou NULL, em caso de erro. */
Pagina obterPag(LStrings l);

/* Devolve a próxima linha de uma página */
char* obterLinha(Pagina pag);
/* Liberta a memória alocada para uma página */
void apagaPag(Pagina pag);

/* Devolve o número da página atual */
int obterNumPag(LStrings l);

/* Devolve o número total de Strings na LStrings */
int obterTotal(LStrings l);

/* Devolve o número total de páginas numa LStrings */
int obterNumTotalPags(LStrings l);

/**
 * A função proxPag() atualiza o índice e a página de uma LStrings
 * se a página atual não for a última. Se o utilizador estiver na
 * última página, esta função não tem qualquer efeito.
 */
void proxPag(LStrings l);

/** 
 * A função pagAnt() atualiza o índice e a página de uma LStrings
 * se a página atual não for a primeira. Se o utilizador estiver na
 * primeira página, esta função não tem qualquer efeito.
 */
void pagAnt(LStrings l);

/**
 * A função irParaPag() avança para a página especificada, se esta
 * não for inferior à primeira ou superior à última página da LStrings.
 */
void irParaPag(int pag, LStrings l);

/* A função primPag() coloca uma LStrings na sua primeira página. */
void primPag(LStrings l);

/* A função primPag() coloca uma LStrings na sua última página. */
void ultimaPag(LStrings l);

#endif