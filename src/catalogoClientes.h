#ifndef CATALOGO_CLIENTES_H
#define CATALOGO_CLIENTES_H

#include "cliente.h" /* definição do tipo Cliente */

typedef struct catClientes* CatClientes;
typedef struct conjClientes* ConjClientes;

CatClientes criaCatClientes();
CatClientes insereCliente(CatClientes, Cliente);
bool existeCliente(CatClientes, Cliente);
int totalClientes(CatClientes);
int totalClientesLetra(CatClientes, char);
void removeCatClientes(CatClientes);
ConjClientes clientesPorLetra(CatClientes, char);

#endif
