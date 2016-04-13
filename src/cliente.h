#ifndef CLIENTE_T
#define CLIENTE_T

#define MAX_CODIGO_CLIENTE 8
typedef struct cliente* Cliente;

Cliente criaCliente(char* codigoCliente);
void apagaCliente(Cliente c);
char* obterCodigoCliente(Cliente c);
char inicioCodigoCliente(Cliente c);  /* Devolve a 1a letra do codigo de um cliente */
int comparaCodigosCliente(Cliente c1, Cliente c2);
Cliente duplicaCliente(Cliente c);

#endif
