#include "../src/avl.c"
#define INFO_TEST(msg) puts("A testar: " msg) 

typedef enum bool{false = 0, true = 1} bool;

void assertBool(bool b, char msg_erro[]);
bool testaCriaAVL(AVL *avl_ptr, int (*compara) (const void *, const void *));
bool testaPrimeiraIns(AVL *avl_ptr, int x);
int comparaInts(const void *x_ptr, const void *y_ptr);

int main(){
	AVL a;

	INFO_TEST("criaAVL()");
	assertBool(testaCriaAVL(&a, comparaInts), "falha a criar AVL");

	INFO_TEST("inserção em AVL vazia");
	assertBool(testaPrimeiraIns(&a, 1), "falha ao fazer a 1ª inserção");

	return 0;
}

void assertBool(bool b, char msg_erro[]){
	if(b == false){
		puts(msg_erro);
		exit(-1);
	}
}

bool testaCriaAVL(AVL *avl_ptr, int (*compara) (const void *, const void *)){
	AVL novaArv = criaAVL(compara);

	*avl_ptr = novaArv; /* guarda a nova árvore na AVL da main() */

	return novaArv->raiz == NULL &&
		   novaArv->compara == compara &&
		   novaArv->tamanho == 0;
}

bool testaPrimeiraIns(AVL *avl_ptr, int x){
	AVL res;

	if(!arv || !(*arv))
		return false;

	*arv = res = insere(*arv, (ValorNodo) x);

	return existeAVL(res, x) && 
	       res->raiz->esquerda == NULL &&
	       res->raiz->direita == NULL;
}

bool testaInvarAVL(AVL_NODO a, int (*compara) (const void *, const void *)){
	bool r;

	if(!a)
		r = true;
	else{
		int he = altura(a->esquerda);
		int hd = altura(a->direita);

		r = abs(he - hd) <= 1 && 
			compara(a->esquerda->valor, a->valor) < 0 &&
			compara(a->direita->valor, a->valor) > 0 &&
			testaInvarAVL(a->esquerda, compara) &&
			testaInvarAVL(a->direita, compara);
	}

	return r;
}

int comparaInts(const void *x_ptr, const void *y_ptr){
	int x, y;

	x = *((int *) x_ptr);
	y = *((int *) y_ptr);

	return x-y;
}