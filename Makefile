CFLAGS =-Wall -Wextra -ansi -pedantic -O2

all: doc tests
src/gereVendas: src/filtraVendas.c
	$(CC) $(CFLAGS) src/filtraVendas.c -o src/gereVendas
tests: src/gereVendas
	cd tests; bash runtests.sh t ../src/gereVendas
tests1: src/gereVendas
	cd src/programasTestes; $(CC) $(CFLAGS) infoCliente.c parseVenda.c -o infoCliente; 
	cd src/programasTestes; $(CC) $(CFLAGS) vendasFilial.c parseVenda.c -o vendasFilial;
	cd src/programasTestes; $(CC) $(CFLAGS) infoProduto.c parseVenda.c -o infoProduto;
doc: src/*.c
	cd doc; doxygen
limparTests1: 
	rm -f src/programasTestes/infoCliente
	rm -f src/programasTestes/vendasFilial
	rm -f src/programasTestes/infoProduto
limpar:
	rm -f src/gereVendas src/fprintVendasVal 
	rm -f tests/*.res
	rm -f data/VendasValidas.*
