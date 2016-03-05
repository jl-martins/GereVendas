#include <stdio.h>
#include <stdlib.h>
#define TAM_CODIGOS 10 /* tamanho máximo dos códigos de cliente/produto */
 
typedef struct{
	char codigoProduto[TAM_CODIGOS];
	float preco;
	int nUnidades;
	char tipoCompra;
	char codigoCliente[TAM_CODIGOS];
	int mes;
	int filial;
} venda_t;

FILE *askOverwrite(const char *filename);
void imprimeDadosVenda(FILE *fp, venda_t *v);

/* imprime de dados das vendas válidas num ficheiro de texto */
int main(){
	venda_t venda;
	FILE *src, *dest;
	const char srcPath[22] = "../data/VendasVal.dat"; /* caminho do ficheiro com os dados das vendas */
	const char dstPath[22] = "../data/VendasVal.txt";

	src = fopen(srcPath, "r");
	if(src == NULL){
		perror(srcPath);
		exit(-1);
	}

	dest = (fopen(dstPath, "r") == NULL) ? fopen(dstPath, "w") : askOverwrite(dstPath);
	if(dest == NULL){
		fprintf(stderr, "Não existe um ficheiro destino\n");
		fclose(src);
		exit(-1);
	}

	while(fread(&venda, sizeof(venda_t), 1, src) == 1)
		imprimeDadosVenda(dest, &venda);

	printf("Resultados impressos em %s\n", dstPath);
	
	fclose(src); 
	fclose(dest);
	
	return 0;
}

/* Pergunta ao utilizador se pretende escrever por cima de um ficheiro já existente.
 * @return File pointer, se o utilizador quiser escrever por cima, NULL c.c.
 */
FILE *askOverwrite(const char *filename){
	char answer;
	FILE *fp;

	printf("File %s already exists. Do you wish to overwrite it [Y/N]? ", filename);
	answer = getchar();

	switch(answer){
		case 'y':
		case 'Y':
			fp = fopen(filename, "w");
			break;
		default:
			fp = NULL;
	}

	while(getchar() != '\n'); /* clear buffer */

	return fp;
}

void imprimeDadosVenda(FILE *fp, venda_t *v){
	fprintf(fp, "%s %f %d %c %s %d %d\n", 
				 v->codigoProduto, 
				 v->preco, 
				 v->nUnidades, 
				 v->tipoCompra, 
				 v->codigoCliente, 
				 v->mes, 
				 v->filial);                 
}