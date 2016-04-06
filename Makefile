EXEC = gereVendas
CFLAGS = -Wall -Wextra -Wno-unused-function -Wno-unused-result -ansi -pedantic -O2
OBJS = $(patsubst(src/%.c, src/%.o, $(wildcard *.c)))
TARGET_ARCH := -march=native

# diretorias onde o utilitário 'make' vai procurar pelas dependências e objetivos da makefile
VPATH = src/ src/programasTestes

all: filtraVendas avl.o catalogo.o tests

.PHONY: all doc tests limpar

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(OUTPUT_OPTION)
tests: $(EXEC)
	cd tests; bash runtests.sh t ../src/filtraVendas
filtraVendas: filtraVendas.c venda.h
	$(LINK.c) $< -o src/$@
# avl.o e catalogo.o são "targets" temporários.
# (não vão ser usados nas versões finais do projeto)
avl.o: avl.c avl.h
	$(CC) $(CFLAGS) -c $< -o src/avl.o
catalogo.o: catalogo.c catalogo.h avl.o avl.h
	$(CC) $(CFLAGS) src/avl.o -c $< -o src/catalogo.o

# executáveis para validar a leitura dos dados
tests1: filtraVendas infoCliente vendasFilial infoProduto

infoCliente: infoCliente.c parseVenda.c
	$(CC) $(CFLAGS) $^ -o src/programasTestes/$@
vendasFilial: vendasFilial.c parseVenda.c
	$(CC) $(CFLAGS) $^ -o src/programasTestes/$@
infoProduto: infoProduto.c parseVenda.c
	$(CC) $(CFLAGS) $^ -o src/programasTestes/$@
# documentação
doc: src/*.c
	cd doc; doxygen
limpar:
	$(RM) filtraVendas src/*.o src/programasTestes/*.o
	$(RM) src/programasTestes/{infoCliente,vendasFilial,infoProduto}
	$(RM) tests/*.res
	$(RM) data/VendasValidas.*
	