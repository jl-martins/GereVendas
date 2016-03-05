CFLAGS =-Wall -Wextra -ansi -pedantic -O2

all: tests
src/gereVendas: $(OBJS)
	$(CC) $(CFLAGS) src/main.c -o src/gereVendas
tests: src/gereVendas
	cd tests; bash runtests.sh t ../src/gereVendas
limpar:
	rm src/gereVendas
	rm tests/*.res
	rm data/VendasVal.dat
