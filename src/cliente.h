#ifndef CLIENTE_T
#define CLIENTE_T

typedef struct cliente* Cliente;

Cliente criaCliente(char* codigoCliente);
void removeCliente(Cliente c);
char* obterCodigoCliente(Cliente c);
/* Devolve a 1a letra do codigo de um cliente */
char inicioCodigoCliente(Cliente c);
#endif
