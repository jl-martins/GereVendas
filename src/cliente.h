#ifndef CLIENTE_T
#define CLIENTE_T

typedef struct cliente* Cliente;

Cliente criaCliente(char * codigoCliente);
char* obterCodigoCliente(Cliente c);

#endif
