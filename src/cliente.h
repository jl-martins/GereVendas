#ifndef CLIENTE_T
#define CLIENTE_T

#define MAX_CODIGO_CLIENTE 8
typedef struct cliente* Cliente;

Cliente criaCliente(char* codigoCliente);
void removeCliente(Cliente c);
char* obterCodigoCliente(Cliente c);
/* Devolve a 1a letra do codigo de um cliente */
char inicioCodigoCliente(Cliente c);
int comparaCodigosCliente(Cliente c1, Cliente c2);

#endif
