#ifndef VENDA_H
#define VENDA_H

#define N_TIPOS_VENDA 2

/* Definição de tipos enumerados relacionados com as vendas */
enum campoVenda {CODIGO_PROD = 0, PRECO, UNIDADES, TIPO_COMPRA, CODIGO_CLIENTE, MES, FILIAL};
typedef enum tipoVenda{N = 0, P = 1} TipoVenda;

#endif
