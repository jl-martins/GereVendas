#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"
#include "LStrings.h" /* listas de strings navegáveis */
#include "produto.h"
#include "venda.h" /* importa 'TipoVenda' */

/* faturação global do ano */
typedef struct fatGlobal* FaturacaoGlobal;
/* guarda informação sobre a faturação de um produto num dado mês */
typedef struct fatProdMes* FatProdMes;
/* guarda informação sobre a faturação de um produto no ano */
typedef struct fatAnualProd* FatAnualProd;

FaturacaoGlobal criaFaturacaoGlobal();

/* Liberta a memória alocada para armazenar a faturação global */
void apagaFaturacaoGlobal(FaturacaoGlobal fg);

/* Regista um produto na faturação global */
FaturacaoGlobal registaProduto(FaturacaoGlobal fg, Produto p);

/* Regista uma venda na faturação global */
FaturacaoGlobal registaVenda(
	FaturacaoGlobal fg,
	Produto p,
	double precoUnit,
	int quantidade,
	TipoVenda tipo,
	int filial,
	int mes
);

/* Devolve o total de unidades vendidas num dado mês */
int totalVendasMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total faturado num dado mês */
double totalFaturadoMes(const FaturacaoGlobal fg, int mes);

/* Devolve o total de vendas registadas num intervalo fechado de meses */
int totalVendasIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Devolve o total faturado num intervalo fechado de meses */
double totalFatIntervMeses(const FaturacaoGlobal fg, int inicio, int fim);

/* Devolve uma cópia da estrutura com informção
 * sobre a faturação de um produto num dado mês */
FatProdMes obterFatProdMes(const FaturacaoGlobal fg, const Produto p, int mes);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda ('tipo'). 
 * Devolve o total de vendas do tipo especificado, registadas em 'fProdMes' */
int vendasTotaisProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda
 * ('tipo'). Devolve um array que em cada posição i tem o nº de vendas 
 * do tipo especificado, registadas em 'fProdMes' para a filial i+1. */
int* vendasPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Devolve, para a faturação de produto 'fProdMes' e para o tipo de
 * venda tipo', a faturação total registada em 'fProdMes' */
double faturacaoTotalProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Recebe a faturação de um produto num mês ('fProdMes') e o tipo de venda ('tipo').
 * Devolve um array que em cada posição i tem o total faturado na filial 
 * i+1, com vendas registadas em 'fProdMes', para o tipo especificado.  */
double* faturacaoPorFilialProdMes(const FatProdMes fProdMes, TipoVenda tipo);

/* Liberta uma cópia do tipo FatProdMes que
 * é devolvida pela função obterFatProdMes()  */
void apagaFatProdMes(FatProdMes fProdMes);

/* Em caso de sucesso, devolve o número de produtos 
 * não comprados. Caso contrário, devolve -1. */
int quantosNaoComprados(const FaturacaoGlobal fg);

/* Devolve um conjunto com os produtos que não foram comprados em nenhuma filial */
LStrings naoCompradosGlobal(const FaturacaoGlobal);

/* Devolve um array de listas de produtos, um por filial. A lista de produtos
 * de índice i armazena os códigos dos produtos não vendidos na filial i. */
LStrings* naoCompradosPorFilial(const FaturacaoGlobal);

/* Liberta a memória alocada para guardar informação sobre a faturação anual de um produto */
void apagaFatAnualProd(FatAnualProd fAnualProd);

/* Devolve um array com informação sobre a faturação anual de cada um
 * dos N mais produtos mais vendidos durante o ano, ordenado pelo
 * total de vendas anuais. */
FatAnualProd* fatNmaisVendidos(const FaturacaoGlobal fg, int N);

/* Liberta a memória alocada para cada um dos elementos 
 * do array devolvido pela função fatNmaisVendidos */
void apagaFatAnualProd(FatAnualProd fAnualProd);

/* Recebe um array produzido pela função fatNmaisVendidos() e devolve
 * um array de 'N' produtos, ordenados de decrescentemente, pelo
 * total de vendas durante o ano */
Produto* obterArrNmaisVendidos(const FatAnualProd fatNmaisVend[], int N);

/* Recebe o array devolvido por fatNmaisVendidos(). Devolve uma matriz
 * de dimensões em que cada posição (i,j) armazena o número de vendas do
 * iesimo produto mais vendido, na filial j. NOTA: a contagem dos N
 * mais vendidos começa em 0, enquanto que a das filiais começa em 1. */
int** vendasPorFilialNmaisVend(const FatAnualProd fatNmaisVend[], int N);

#endif
