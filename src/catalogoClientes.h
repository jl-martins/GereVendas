#ifndef CATALOGO_CLIENTES_H
#define CATALOGO_CLIENTES_H

#include "catalogo.h"

typedef Catalogo CatClientes;
typedef Elem Cliente;
typedef ConjuntoElems ConjuntoClientes;

CatClientes initCatClientes(int (*compara) (const void *, const void *));
CatClientes insereCliente(CatClientes, Cliente);
bool existeCliente(CatClientes, Cliente);
int totalClientes(CatClientes);
int totalClientesLetra(CatClientes, char);
void removeCatClientes(CatClientes);
ConjuntoElems elemsPorLetra(CatClientes, char);

#endif
