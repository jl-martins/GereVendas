/**
 * @file faturacaoGlobal.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com as funções e TADs da faturação global do projeto @verbatim gereVendas @endverbatim
 *
 * Este ficheiro contém todas as funções e tipos abstratos de dados associados
 * à faturação global do projeto @verbatim gereVendas @endverbatim. As funções
 * referenciadas neste .h permitem criar e apagar a faturação global, bem como 
 * obter informações sobre as vendas de mensais ou do ano todo no hipermercado em questão.
 */

#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"
#include "LStrings.h" /* listas de strings navegáveis */
#include "produto.h"
#include "venda.h" /* importa 'TipoVenda' */

/** @brief Referência da estrutura que guarda todas as informações relativas à faturação global. */
typedef struct fatGlobal* FaturacaoGlobal;
/** @brief Referência de uma estrutura que guarda dados sobre a faturação de um produto num dado mês. */
typedef struct fatProdMes* FatProdMes;

/**
 * @brief Cria a faturação global.
 * @return Referência da estrutura criada, em caso de sucesso. @c NULL caso contrário.
 */
FaturacaoGlobal criaFaturacaoGlobal();

/**
 * @brief Liberta a memória alocada para armazenar a faturação global.
 * @param fg Faturação global a apagar.
 * @return É sempre retornado @c NULL.
 */
FaturacaoGlobal apagaFaturacaoGlobal(FaturacaoGlobal fg);

/**
 * @brief Regista um produto na faturação global.
 * @param fg Faturação global em que o produto será inserido.
 * @param p Produto a inserir na faturação global.
 * @return Referência da faturação global em caso de sucesso. @c NULL c.c.
 */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p);

/**
 * @brief Regista uma venda na faturação global.
 * @param fg Faturação global onde a venda será registada.
 * @param p Produto vendido.
 * @param precoUnit Preço unitário do produto.
 * @param quantidade Quantidade vendida.
 * @param tipo Tipo de venda (N ou P).
 * @param mes Mês da venda.
 * @return Referência da faturação global em caso de sucesso. @c NULL c.c.
 */
FaturacaoGlobal registaVenda(
	FaturacaoGlobal fg,
	Produto p,
	double precoUnit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes
);

/**
 * @brief Calcula o total de vendas num dado mês.
 * @param fg Faturação global a consultar.
 * @param mes Mês para o qual se pretende calcular o total de vendas.
 * @return Total de vendas do mês especificado.
 */
int totalVendasMes(const FaturacaoGlobal fg, int mes);

/**
 * @brief Calcula o total faturado num dado mês. 
 * @param fg Faturação global a consultar.
 * @param mes Mês para o qual se pretende calcular o total faturado.
 * @return Total faturado no mês especificado.
 */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes);

/**
 * @brief Calcula o total de vendas registadas num intervalo fechado de meses. 
 * @param fg Faturação global a consultar.
 * @param inicio Início do intervalo fechado de meses.
 * @param fim Fim do intervalo fechado de meses.
 * @return Total de vendas do intervalo fechado de meses.
 */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/**
 * @brief Calcula o total faturado num intervalo fechado de meses.
 * @param fg Faturação global a consultar.
 * @param inicio Início do intervalo fechado de meses.
 * @param fim Fim do intervalo fechado de meses.
 * @return Total faturado no intervalo fechado de meses.
 */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/** 
 * @brief Dado um produto e um mês, devolve a sua faturação nesse mês
 * @param fg Faturação global a consultar.
 * @param p Produto cuja faturação será consultada.
 * @param mes Mês a considerar.
 * @return Se o produto passado como parâmetro tiver pelo menos uma venda
 * no mês especificado, é devolvida uma referência para uma estrutura com
 * a informação sobre a faturação desse produto nesse mês. Se não, é devolvido @c NULL.
 *
 * @warning É devolvida uma cópia da informação que está na faturação global.
 * Depois de o utilizador obter as informações que pretende, deve libertar a
 * cópia obtida, passando-a para a função @c apagaFatProdMes().
 */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes);

/**
 * @brief Liberta a memória alocada para guardar o resultado de obterFatProdMes().
 * @param fProdMes FatProdMes a apagar.
 * @return É sempre retornado @c NULL.
 */
FatProdMes apagaFatProdMes(FatProdMes fProdMes);

/**
 * @brief A partir da faturação de um produto num mês (obtida com @c obterFatProdMes()),
 * calcula o total de vendas do tipo especificado (N ou P).
 * @param fProdMes Faturação de um produto num mês, obtida com obterFatProdMes().
 * @param tipo Tipo de venda pretendido (N ou P)
 * @return Total de vendas registadas em 'fProdMes', para o tipo especificado.
 */
int totalUnidsProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/**
 * @brief A partir da faturação de um produto num mês (obtida com @c obterFatProdMes())
 * produz um array com as unidades vendidas em cada filial, para o tipo de venda escolhido.
 * @param fProdMes Faturação de um produto num mês, obtida com obterFatProdMes().
 * @param tipo Tipo de venda pretendido (N ou P).
 * @return Array que na posição i tem o número de unidades registadas
 * em 'fProdMes', para a filial i e para o tipo de venda especificado.
 * @warning Se ocorrer uma falha de alocação, é devolvido NULL
 */
int* unidsPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/**
 * @brief A partir da faturação de um produto num mês (obtida com @c obterFatProdMes()),
 * calcula o total faturado com vendas do tipo especificado (N ou P).
 * @param fProdMes Faturação de um produto num mês, obtida com obterFatProdMes().
 * @param tipo Tipo de venda pretendido (N ou P)
 * @return Faturação total registada em 'fProdMes', para vendas do tipo escolhido.
 */
double faturacaoTotalProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/**
 * @brief A partir da faturação de um produto num mês (obtida com @c obterFatProdMes())
 * produz um array com a faturação total das vendas do tipo especificado, para cada filial.
 * @param fProdMes Faturação de um produto num mês, obtida com obterFatProdMes().
 * @param tipo Tipo de venda pretendido (N ou P).
 * @return Array que na posição i tem a faturação total registada
 * em 'fProdMes', para a filial i e para o tipo de venda escolhido.
 * @warning Se ocorrer uma falha de alocação, é devolvido NULL
 */
double* faturacaoPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/**
 * @brief Calcula o número total de produtos não comprados em nenhuma filial.
 * @param fg Faturação global a consultar.
 * @return Número total de produtos não comprados em caso de sucesso. -1 c.c.
 */
int quantosNaoComprados(const FaturacaoGlobal fg);

/**
 * @brief Produz uma lista de Strings com os produtos que não foram comprados
 * em nenhuma filial.
 * @param fg Faturação global a consultar.
 * @return LStrings com os códigos dos produtos que não foram comprados em
 * nenhuma filial. NULL em casos de falha de alocação.
 * @warning Se ocorrer uma falha de alocação, é devolvido NULL.
 */
LStrings naoCompradosGlobal(const FaturacaoGlobal fg);

/**
 * @brief Produz um array de lista de strings em que a lista na posição i
 * tem os códigos de todos os produtos que não foram comprados na filial i.
 * @param fg Faturação global a consultar.
 * @return Array de LStrings cujo elemento na posição i tem os códigos de todos
 * os produtos não comprados na filial i.
 * @warning Se ocorrer uma falha de alocação, é devolvido NULL.
 */
LStrings* naoCompradosPorFilial(const FaturacaoGlobal fg);

/**
 * @brief Dada uma filial e um número 'N', produz um array com 
 * os códigos dos 'N' produtos mais vendidos nessa filial.
 * @param fg Faturação global a consultar.
 * @param N Tamanho do array que vai ser devolvido (se N <= totalProdutos)
 * @param filial Filial a considerar.
 * @return Array com os códigos dos 'N' produtos mais vendidos na filial
 * escolhida, ordenado decrescentemente pelo total de vendas.
 * @warning Se N for maior que #totalProdutos, é devolvido um array
 * de comprimento igual a ao #totalProdutos. 
 */
char** NmaisVendidosFilial(const FaturacaoGlobal fg, int N, int filial);

#endif
