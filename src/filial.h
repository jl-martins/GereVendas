#ifndef FILIAL_T 
#define FILIAL_T

#include "cliente.h"
#include "produto.h"
#include "bool.h"
#include "venda.h"
#include "LStrings.h"

typedef struct filial * Filial;

/* aloca espaço e inicializa uma Filial */
Filial criaFilial();
/* liberta o espaço de uma filial */
Filial apagaFilial(Filial filial);
/* regista os dados de uma compra numa filial */
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco);

/* devolve um array com 13 entradas(12 validas) que à entrada i faz 
corresponder o numero de unidades compradas no mes i pelo cliente */
int * unidadesClientePorMes(Filial filial, Cliente cliente);
Cliente * clientesCompraramNaFilial(Filial filial);

/* Devolve TRUE se um cliente tiver feito uma compra na filial */
bool clienteComprouNaFilial(Filial filial, Cliente cliente);

/* Devolve o número total de clientes que compraram numa filial */
int quantosClientesCompraram(Filial filial);

/* Recebe uma filial, um cliente e um mês. Devolve uma lista de Strings com os códigos
 * de produto que o cliente mais comprou (por quantidade), ordenada decrescentemente */
LStrings produtosClienteMaisComprou(Filial filial, Cliente c, int mes);

/* Recebe uma filial e um cliente. Devolve um array com os códigos
 * dos 3 produtos em que o cliente gastou mais dinheiro durante o ano */
char** tresProdsEmQueMaisGastou(Filial filial, Cliente c);

/*Dado um cliente, uma filial e um produto, verifica se o cliente comprou em modo Normal e 
  se comprou em modo de promoção. os resultados sao guardados nas variaveis passadas cmo referencia */
void comprou(Filial filial, Cliente cliente, Produto produto, bool * comprouN, bool * comprouP);

/* Dada uma filial e um codigo de Cliente, devolve o numero de clientes que compraram o Produto e a quandtidade na variavel unidadesCompradas passada por referencia*/
int numeroClientesCompraramProduto(Filial filial, char * produto, int * unidadesCompradas);

#endif
