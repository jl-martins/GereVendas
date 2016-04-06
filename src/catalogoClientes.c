#include "catalogoClientes.h"

CatClientes initCatClientes(int (*compara) (const void *, const void *)){
	return initCatalogo(compara);
}

CatClientes insereElem(CatClientes cc, Cliente codCliente){
	return insere(cc, codCliente);
}

bool existeCliente(Catalogo cc, Cliente codCliente){
	return existeElem(cc, codCliente);
}

int totalClientes(Catalogo cc){
	return totalElems(cc); 
}

int totalClientesLetra(Catalogo cc, char l){
	return totalElemsLetra(cc);
}

void removeCatClientes(Catalogo cc){
	removeCatalogo(cc);
}
/*
ConjuntoElems elemsPorLetra(Catalogo c, char l){
}
*/
