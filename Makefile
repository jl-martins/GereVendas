EXEC = gereVendas
CFLAGS = -Wall -Wextra -Wno-unused-function -Wno-unused-result -ansi -pedantic -O2
OBJS = $(patsubst(src/%.c, src/%.o, $(wildcard src/*.c)))
TARGET_ARCH := -march=native

# diretorias onde o utilitário 'make' vai procurar pelas dependências e objetivos da makefile
VPATH = src/ src/programasTestes

all: tests avl.o catalogoProds.o catalogoClientes.o 

.PHONY: all doc tests limpar

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(OUTPUT_OPTION)
tests: filtraVendas
	cd tests; bash runtests.sh t ../src/filtraVendas
filtraVendas: filtraVendas.c venda.h
	$(LINK.c) $< -o src/$@

# os próximos 3 targets são temporários
avl.o: avl.c avl.h
	$(COMPILE.c) $< -o src/$@
catalogoProds.o: catalogoProds.c catalogoProds.h produto.h avl.h 
	$(COMPILE.c) $< -o src/$@
catalogoClientes.o: catalogoClientes.c catalogoClientes.h cliente.h avl.h
	$(COMPILE.c) $< -o src/$@

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
	$(RM) src/filtraVendas src/*.o src/programasTestes/*.o
	$(RM) tests/*.res
	$(RM) data/VendasValidas.*
	# descobrir melhor forma de remover os executáveis de src/programasTestes
	$(RM) src/programasTestes/infoCliente src/programasTestes/vendasFilial src/programasTestes/infoProduto
	