CFLAGS =-Wall -Wextra -ansi -pedantic -O2

all: tests
src/gereVendas: src/main.c
	$(CC) $(CFLAGS) src/main.c -o src/gereVendas
tests: src/gereVendas
	cd tests; bash runtests.sh t ../src/gereVendas
tests1: src/gereVendas
	cd src/programasTestes; $(CC) $(CFLAGS) infoCliente.c parseVenda.c -o infoCliente; 
	cd src/programasTestes; $(CC) $(CFLAGS) vendasFilial.c parseVenda.c -o vendasFilial;
limparTests1: 
	rm -f src/programasTestes/infoCliente
	rm -f src/programasTestes/vendasFilial
limpar:
	rm -f src/gereVendas src/fprintVendasVal 
	rm -f tests/*.res
	rm -f data/VendasValidas.*
