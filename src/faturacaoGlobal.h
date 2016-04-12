#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"
#include "catalogoProds.h" /* inclui tipos 'Produto' e 'CatalogoProds' */
#include "venda.h" /* importa 'TipoVenda' */

/* faturação global do ano */
typedef struct fatGlobal* FaturacaoGlobal;
/* informação sobre a faturação de um produto num dado mês */
typedef struct fatMensalProd* FatMensalProd;

FaturacaoGlobal criaFaturacaoGlobal();

/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(
	FaturacaoGlobal fg,
	Produto p,
	double preco_unit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes);

/* Regista um produto na faturação global */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p);

/* Devolve uma estrutura com informção sobre a faturação de um produto num dado mês */
FatMensalProd obterFatMensalProd(const FaturacaoGlobal fg, const Produto p, int mes);

/* Devolve o total de unidades vendidas num mês */
int totalVendasMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total faturado num mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total de vendas registadas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Dada a faturação de produto 'fprod' e o tipo de venda 'tipo',
   devolve um array que em cada posição i tem o nº de vendas 
   registadas em 'fprod', para a filial i+1. */
int* vendasPorFilial(FatMensalProd fProd, TipoVenda tipo);

/* Dada a faturação de produto 'fProd' e o tipo de venda 'tipo', devolve
   o total de vendas do tipo especificado que estão registadas em 'fprod' */
int vendasTotais(FatMensalProd fProd, TipoVenda tipo);

/* Dada a faturação de produto 'fprod' e o tipo de venda 'tipo',
   devolve um array que em cada posição i tem a faturação
   registada em 'fprod', para a filial i+1. */
double* faturacaoPorFilial(FatMensalProd fProd, TipoVenda tipo);

/* Devolve, para a faturação de produto 'fProd' e para o tipo de venda 'tipo',
   a faturação total que está registada em 'fprod' */
double faturacaoTotal(FatMensalProd fProd, TipoVenda tipo);

#endif
