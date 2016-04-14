EXEC = gereVendas
CFLAGS = -Wall -Wextra -Wno-unused-function -Wno-unused-result -ansi -pedantic -O2
OBJS = $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))
TARGET_ARCH := -march=native

# diretorias onde o utilitário 'make' vai procurar pelas dependências e objetivos da makefile
VPATH = src/ src/programasTestes

all: $(EXEC)

tmp: avl.o catalogoProds.o catalogoClientes.o faturacaoGlobal.o filial.o LStrings.o

.PHONY: all debug leak-check tmp doc tests limpar

$(EXEC): $(OBJS)
	$(LINK.c) $(OBJS) $(OUTPUT_OPTION)

debug: CFLAGS += -g
debug: $(EXEC)
	gdb $(EXEC)

leak-check: CFLAGS += -g
leak-check: $(EXEC)
	valgrind --tool=memcheck --leak-check=yes ./$(EXEC)

tests: filtraVendas
	cd tests; bash runtests.sh t ../src/filtraVendas
filtraVendas: filtraVendas.c venda.h
	$(LINK.c) $< -o src/$@

# início dos targets temporários
avl.o: avl.c avl.h
	$(COMPILE.c) $< -o src/$@
catalogoProds.o: catalogoProds.c catalogoProds.h avl.h bool.h produto.h
	$(COMPILE.c) $< -o src/$@
catalogoClientes.o: catalogoClientes.c catalogoClientes.h avl.h bool.h cliente.h
	$(COMPILE.c) $< -o src/$@
faturacaoGlobal.o: faturacaoGlobal.c faturacaoGlobal.h avl.h bool.h produto.h venda.h
	$(COMPILE.c) $< -o src/$@
filial.o: filial.c filial.h avl.h venda.h cliente.h produto.h bool.h 
	$(COMPILE.c) $< -o src/$@
LStrings.o: LStrings.c LStrings.h
	$(COMPILE.c) $< -o src/$@

# fim dos targets temporários

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
	$(RM) gereVendas
	$(RM) data/VendasValidas.*
	# descobrir melhor forma de remover os executáveis de src/programasTestes
	$(RM) src/programasTestes/infoCliente src/programasTestes/vendasFilial src/programasTestes/infoProduto
	