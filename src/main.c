#include <catalogoProds.h>
#include <catalogoClientes.h>
#include <faturacaoGlobal.h>
#include <vendasFilial.h>

const static char* opcoes[N_OPCOES] = {
	"Sair",
	"Ler ficheiros",
	"Listar produtos começados por uma letra (maiúscula)",
	"Apresentar vendas e faturação totais de um produto, num dado mês"
	"Listar ordenadamente códigos dos produtos que ninguém comprou",
	"Mostrar tabela com as compras efetuadas por um cliente",
	"Determinar vendas e faturação total para um intervalo fechado de meses",
	"Gerar lista ordenada de clientes que realizaram compras em todas as filiais",
	"Determinar códigos e nº total de clientes que compraram um dado produto numa dada filial",
	"Apresentar ordem decrescente dos produtos mais comprados por um cliente num dado mês",
	"Criar lista com os N produtos mais vendidos do ano",
	"Determinar códigos dos 3 produtos em que um cliente gastou mais dinheiro"
	"Listar clientes que não compraram e produtos não vendidos"
}

int main(){
	int r = interpretador();
	
	return r;
}

int interpretador(){
	int r = CONTINUAR;
	char linha[MAXLINHA];

	while(fgets(linha, MAXLINHA, stdin) && r == CONTINUAR){
		r = interpreta(linha);
	}

	return r;
}