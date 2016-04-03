/* roda AVL para a esquerda */

static AVL rodaEsquerda(AVL_NODO *raiz){
	AVL_NODO *aux;

	if(!raiz || !raiz->direita){
		/* tratamento de erros */
	}
	else{
		aux = raiz->direita;
		raiz->direita = aux->esquerda;
		aux->esquerda = raiz;
		raiz = aux;
	}

	return raiz;
}

static AVL_NODO* insereEsquerda(AVL_NODO *raiz, ValorNodo val, int (*compara) (void *, void *), int *cresceu){
	raiz->esquerda = insereNodo(raiz->esquerda, val, compara, cresceu);
	
	if(*cresceu){
		switch(raiz->fatorBalanco){
			case ESQ:
				raiz = equilibraEsquerda(raiz);
				*cresceu = 0;
				break;
			case EQ:
				raiz->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->fatorBalanco = EQ;
				*cresceu = 0;
				break;
		}
	}
	return raiz;
}

static AVL_NODO equilibraEsquerda(AVL_NODO *raiz){

	if(raiz->esquerda->fatorBalanco == ESQ){
		/* rotação simples à direita */
		raiz = rodaDireita(raiz);
		raiz->fatorBalanco = EQ;
		raiz->direita->fatorBalanco = EQ;
	}
	else{
		/* Dupla rotação */
		raiz->esquerda = rodaEsquerda(raiz->esquerda);
		raiz = rodaDireita(raiz);

		switch(raiz->fatorBalanco){
			case EQ:
				raiz->esquerda->fatorBalanco = EQ;
				raiz->direita->fatorBalanco = EQ;
				break;
			case ESQ:
				t->direita->fatorBalanco = EQ;
				t->esquerda->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->direita->fatorBalanco = DIR;
				raiz->esquerda->fatorBalanco = EQ;
				break;
		}
		raiz->fatorBalanco = EQ;
	}
	return raiz;
}

int altura(const AVL arv){
	return (arv == NULL) ? -1 : alturaAux(arv->raiz);
}

static int alturaAux(const AVL_NODO *raiz){
	int res;

	if(raiz == NULL)
		res = 0;
	else if(raiz->fatorBalanco == ESQ)
		res = 1 + alturaAux(raiz->esq);
	else
		res = 1 + alturaAux(raiz->dir);

	return res;
/bin/bash: line 0: fg: no job control
