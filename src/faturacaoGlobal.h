#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"
#include "catalogoProds.h" /* inclui tipos 'Produto' e 'CatalogoProds' */
#include "venda.h" /* importa 'TipoVenda' */

/* faturação global do ano */
typedef struct fatGlobal* FaturacaoGlobal;
/* guarda informação sobre a faturação de um produto num dado mês */
typedef struct fatProdMes* FatProdMes;

FaturacaoGlobal criaFaturacaoGlobal();

/* Regista um produto na faturação global */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p);

/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda
(
	FaturacaoGlobal fg,
	Produto p,
	double precoUnit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes
);

/* Devolve o total de unidades vendidas num mês */
int totalVendasMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total faturado num mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total de vendas registadas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Devolve uma cópia da estrutura com informção
 * sobre a faturação de um produto num dado mês */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda
 * ('tipo'). Devolve um array que em cada posição i tem o nº de vendas 
 * do tipo especificado, registadas em 'fProdMes' para a filial i+1. */
int* vendasPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda ('tipo'). 
 * Devolve o total de vendas do tipo especificado, registadas em 'fProdMes' */
int vendasTotaisProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda ('tipo').
 * Devolve um array que em cada posição i tem o total faturado na filial 
 * i+1, com vendas registadas em 'fProdMes', para o tipo especificado.  */
double* faturacaoPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Devolve, para a faturação de produto 'fProdMes' e para o tipo de
 * venda tipo', a faturação total registada em 'fProdMes' */
double faturacaoTotalProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Devolve um conjunto com os produtos que não foram comprados em nenhuma filial */
ConjuntoProds naoCompradosGlobal(const FaturacaoGlobal);

/* Devolve um array de conjuntos de produtos, um por filial. Cada conjunto de
 * produtos armazena os códigos de produto não vendidos na respetiva filial. */
ConjuntoProds* naoCompradosPorFilial(const FaturacaoGlobal);

/* Liberta a memória alocada para armazenar a faturação global */
void removeFaturacaoGlobal(FaturacaoGlobal fg);

/* Liberta uma cópia do tipo FatProdMes que
 * foi devolvida pela função obterFatProdMes()  */
void removeFatProdMes(FatProdMes fProdMes);

#endif
