/**
 * @file cliente.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com a definição e API do tipo abstrato de dados @c Cliente.
 */

#ifndef CLIENTE_T
#define CLIENTE_T

typedef struct cliente* Cliente;

/** 
 * @brief Cria um cliente com o código passado como parâmetro. 
 * @param codigoCliente Código do cliente a criar.
 * @return Cliente criado em caso de sucesso. NULL em caso de falha de alocação.
 */
Cliente criaCliente(char* codigoCliente);

/** 
 * @brief Liberta a memória alocada para guardar um cliente.
 * @param c Cliente a libertar.
 * @return É sempre retornado @c NULL.
 */
Cliente apagaCliente(Cliente c);

/** 
 * @brief Devolve uma cópia do código de um
 * cliente ou NULL em caso de falha de alocação.
 */
char* obterCodigoCliente(Cliente c);

/** @brief Devolve a 1a letra do codigo de um cliente */
char inicioCodigoCliente(Cliente c);  

/** 
 * @brief Compara os códigos de dois clientes.
 * @return Um valor < 0 se o código de c1 for menor que o de c2, igual a 0
 * se os códigos forem iguais e > 0 se o código de c1 for maior que o de c2.
 */
int comparaCodigosCliente(Cliente c1, Cliente c2);

/** @brief Devolve uma cópia do cliente passado como parâmetro. */
Cliente duplicaCliente(Cliente c);

#endif
