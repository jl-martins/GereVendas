CFLAGS =-Wall -Wextra -ansi -pedantic -O2

all: tests
src/gereVendas: $(OBJS)
	$(CC) $(CFLAGS) src/main.c -o src/gereVendas
tests: src/gereVendas
	cd tests; bash runtests.sh t ../src/gereVendas
VendasValTxt:
	$(CC) $(CFLAGS) src/fprintVendasVal.c -o src/fprintVendasVal
	cd src; ./fprintVendasVal
limpar:
	rm -f src/gereVendas src/fprintVendasVal 
	rm -f tests/*.res
	rm -f data/VendasVal.*
