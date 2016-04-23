/**
 * @file LStrings.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com funções que permitem criar e navegar em listas de strings genéricas
 *
 * Este ficheiro contém todas as funções e tipos abstratos de dados associados com a criação
 * e navegação de listas de strings do projeto @verbatim gereVendas @endverbatim. 
 */

#ifndef LSTRINGS_H
#define LSTRINGS_H

/** Número de strings por página de uma LStrings (a última página pode ter menos strings). */
#define STRINGS_POR_PAG 32

/** Lista de strings navegável. */ 
typedef struct lStrings* LStrings;
/** Página de uma LStrings. */
typedef struct pagina* Pagina;

/**
 * @brief Cria uma LStrings com o tamanho e com os elementos especificados nos argumentos.
 * @param total Número de elementos da LStrings a criar.
 * @param strings Array com as strings a introduzir na LStrings.
 * @return LStrings criada em caso de sucesso. NULL em caso de falha de alocação
 */
LStrings criaLStrings(int total, char* strings[]);

/** 
 * @brief Liberta a memória alocada para armazenar uma LStrings
 * @param l LStrings a apagar
 * @return É sempre retornado @c NULL.
 */
LStrings apagaLStrings(LStrings l);

/* Funções de navegação */

/** @brief Devolve a página atual de uma LStrings (ou NULL) em caso de erro. */
Pagina obterPag(LStrings l);

/** @brief Devolve a próxima linha de uma página */
char* obterLinha(Pagina pag);

/** @brief Liberta a memória alocada para uma página */
Pagina apagaPag(Pagina pag);

/** @brief Devolve o número da página atual */
int obterNumPag(LStrings l);

/** @brief Devolve o número total de Strings na LStrings */
int obterTotal(LStrings l);

/** @brief Devolve o número total de páginas numa LStrings */
int obterNumTotalPags(LStrings l);

/**
 * @brief Dada uma LStrings, passa para a página seguinte
 * da mesma, se a página atual não for a última. Estando
 * na última página, esta função não qualquer efeito.
 */
void proxPag(LStrings l);

/**
 * @brief Dada uma LStrings, passa para a página anterior
 * da mesma, se a página atual não for a primeira. Caso
 * contrário, esta função não qualquer efeito.
 */
void pagAnt(LStrings l);

/**
 * @brief Dada uma LStrings e uma página, vai para a página
 * escolhida, se esta for válido. Caso contrário, não há
 * qualquer alteração de página.
 */
void irParaPag(int pag, LStrings l);

/** @brief Coloca uma LStrings na sua primeira página. */
void primPag(LStrings l);

/** @brief Coloca uma LStrings na sua última página. */
void ultimaPag(LStrings l);

#endif
