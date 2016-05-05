/**
 * @file produto.h
 * @author LI3_Grupo1
 * @date 23 de Abril de 2016
 * @brief Ficheiro com a definição e API do tipo abstrato de dados @c Produto.
 */

#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct produto* Produto;

/** 
 * @brief Cria um produto com o código passado como parâmetro. 
 * @param codigoProduto Código do produto a criar.
 * @return Produto criado em caso de sucesso. NULL em caso de falha de alocação.
 */
Produto criaProduto(char* codigoProduto);

/** 
 * @brief Liberta a memória alocada para guardar um produto.
 * @param p Produto a libertar.
 * @return É sempre retornado @c NULL.
 */
Produto apagaProduto(Produto p);

/** 
 * @brief Devolve uma cópia do código de um produto ou NULL
 * em caso de falha de alocação.
 */
char* obterCodigoProduto(Produto prod);

/** @brief Devolve a 1a letra do código de um produto. */
char inicioCodigoProduto(Produto prod);

/** 
 * @brief Compara os códigos de dois produtos.
 * @return Um valor < 0 se o código de p1 for menor que o de p2, igual a 0
 * se os códigos forem iguais e > 0 se o código de p1 for maior que o de p2.
 */
int comparaCodigosProduto(Produto p1, Produto p2);

/** @brief Devolve uma cópia do produto passado como parâmetro. */
Produto duplicaProduto(Produto p);

#endif
