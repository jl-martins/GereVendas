#ifndef CLIENTE_T
#define CLIENTE_T

#define MAX_CODIGO_CLIENTE 8
typedef struct cliente* Cliente;

/** Cria um cliente com codigo codigoCliente */
Cliente criaCliente(char* codigoCliente);

/** Apaga um cliente */
Cliente apagaCliente(Cliente c);

/** Devolve o codigo do cliente */
char* obterCodigoCliente(Cliente c);

/** Devolve a 1a letra do codigo de um cliente */
char inicioCodigoCliente(Cliente c);  

/** Compara os codigos de Clientes */
int comparaCodigosCliente(Cliente c1, Cliente c2);

/** Cria um cliente com o mesmo codigo que o que lhe foi passado */
Cliente duplicaCliente(Cliente c);

#endif
