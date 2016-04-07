#define MAX_CODIGO_CLIENTE 10

struct vendas{
	char codigoCliente[MAX_CODIGO_CLIENTE];
	AVL vendasCliente;
	bool comprouFilial[N_FILIAIS];
}
