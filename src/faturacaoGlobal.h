#ifndef FATURACAO_H
#define FATURACAO_H

#include "bool.h"

/* tipos de vendas existentes */
typedef enum tipovenda{N = 0,P = 1} TipoVenda;
/* faturação global do ano */
typedef struct fatG* FaturacaoGlobal;
/* informação sobre a faturação de um produto */
typedef struct fatProd* FaturacaoProd;

#endif
