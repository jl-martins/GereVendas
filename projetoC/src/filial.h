/**
 * @file filial.h
 * @author LI3_Grupo1
 * @date 23 de Abril de 2016
 * @brief Ficheiro com as funções e TADs de uma filial do projeto @verbatim gereVendas @endverbatim
 *
 * Este ficheiro contém todas as funções e tipos abstratos de dados associados
 * a uma dada filial do projeto @verbatim gereVendas @endverbatim. As funções
 * referenciadas neste .h permitem criar e apagar uma filial, bem como obter
 * informações sobre os clientes que compraram cada produto, a quantidade que
 * compraram e o mês em que o fizeram.
 */

#ifndef FILIAL_H
#define FILIAL_H

#include "cliente.h"
#include "produto.h"
#include "bool.h"
#include "venda.h"
#include "LStrings.h"

/** @brief Referência da estrutura que guarda as informações relativas a uma filial */
typedef struct filial* Filial;

/** @brief Aloca espaço e inicializa uma Filial */
Filial criaFilial();

/** @brief Liberta o espaço alocado para guardar uma Filial */
Filial apagaFilial(Filial filial);

/** @brief Regista os dados de uma compra numa filial */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     		   TipoVenda tipoVenda, int unidades, double preco);

/**
 * @brief Devolve um array com 13 entradas (12 válidas) que à entrada i faz 
 * corresponder o numero de unidades compradas no mes i pelo cliente escolhido. 
 */
int* unidadesClientePorMes(Filial filial, Cliente cliente);

/** 
 * @brief Recebe uma Filial. Devolve um array com os clientes
 * que compraram na filial passada como parâmetro para a função.
 */
Cliente* clientesCompraramNaFilial(Filial filial);

/** @brief Devolve TRUE se um cliente tiver feito uma compra na filial e FALSE c.c. */
bool clienteComprouNaFilial(Filial filial, Cliente cliente);

/** @brief Devolve o número total de clientes que compraram numa filial. */
int quantosClientesCompraram(Filial filial);

/**
 * @brief Recebe uma Filial, um cliente e um mês. Devolve uma lista de Strings com os códigos
 * de produto que o cliente mais comprou (por quantidade) nessa filial, ordenada decrescentemente.
 */
LStrings produtosClienteMaisComprou(Filial filial, Cliente c, int mes);

/**
 * @brief Recebe uma Filial e um cliente. Devolve um array com os códigos
 * dos 3 produtos em que o cliente gastou mais dinheiro durante o ano.
 */
char** tresProdsEmQueMaisGastou(Filial filial, Cliente c);

/**
 * @brief Dado um cliente, uma Filial e um produto, verifica se o cliente comprou 
 * em modo Normal e se comprou em modo de promoção. Os resultados sao guardados nas
 * variaveis passadas por referência 
 */
void comprou(Filial filial, Cliente cliente, Produto produto, bool* comprouN, bool* comprouP);

/**
 * @brief Dada uma Filial e um codigo de Cliente, devolve o numero de clientes que
 * compraram o Produto e a quantidade na variavel apontada por unidadesCompradas.
 */
int numeroClientesCompraramProduto(Filial filial, char* produto, int* unidadesCompradas);

#endif
