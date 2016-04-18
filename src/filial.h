#ifndef FILIAL_T 
#define FILIAL_T

#include "cliente.h"
#include "produto.h"
#include "bool.h"
#include "venda.h"

typedef struct filial * Filial;
/*typedef struct vendasAnuaisFilial * VendasAnuaisFilial;*/

Filial criaFilial();
Filial apagaFilial(Filial filial);
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco);
int * unidadesClientePorMes(Filial filial, Cliente cliente);
Cliente * clientesCompraramNaFilial(Filial filial);

/* Devolve TRUE se um cliente tiver feito uma compra na filial */
bool clienteComprouNaFilial(Filial filial, Cliente cliente);
/* Devolve o número total de clientes que compraram numa filial */
int quantosClientesCompraram(Filial filial);

#endif
