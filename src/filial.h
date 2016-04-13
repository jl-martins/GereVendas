#include "cliente.h"
#include "produto.h"
#include "bool.h"
#include "venda.h"

#ifndef FILIAL_T 
#define FILIAL_T

typedef struct filial * Filial;
/*typedef struct vendasAnuaisFilial * VendasAnuaisFilial;*/

Filial criaFilial();
Filial apagaFilial(Filial filial);
Filial registaCompra(Filial filial, Cliente cliente, Produto produto, int mes, 
		     TipoVenda tipoVenda, int unidades, double preco);
int * unidadesClientePorMes(Filial filial, Cliente cliente);
Cliente * clientesCompraramNaFilial(Filial filial);

#endif
