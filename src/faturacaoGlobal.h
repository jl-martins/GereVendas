#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"
#include "produto.h" /* import o tipo 'Produto' */
#include "venda.h" /* importa 'TipoVenda' */

/* faturação global do ano */
typedef struct fatG* FaturacaoGlobal;
/* informação sobre a faturação de um produto num dado mês */
typedef struct fatProd* FaturacaoProd;

FaturacaoGlobal criaFaturacaoGlobal();
/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(FaturacaoGlobal fg, Produto p, double preco_unit,
							 int quantidade, TipoVenda tipo, int filial, int mes);

/* Devolve uma estrutura com informção sobre a faturação de um produto num dado mês */
FaturacaoProd obterFaturacaoProduto(const FaturacaoGlobal fg, const Produto p, int mes);

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
int* vendasPorFilial(FaturacaoProd fProd, TipoVenda tipo);

/* Dada a faturação de produto 'fProd' e o tipo de venda 'tipo', devolve
   o total de vendas do tipo especificado que estão registadas em 'fprod' */
int vendasTotais(FaturacaoProd fProd, TipoVenda tipo);

/* Dada a faturação de produto 'fprod' e o tipo de venda 'tipo',
   devolve um array que em cada posição i tem a faturação
   registada em 'fprod', para a filial i+1. */
double* faturacaoPorFilial(FaturacaoProd fProd, TipoVenda tipo);

/* Devolve, para a faturação de produto 'fProd' e para o tipo de venda 'tipo',
   a faturação total que está registada em 'fprod' */
double faturacaoTotal(FaturacaoProd fProd, TipoVenda tipo);

#endif
