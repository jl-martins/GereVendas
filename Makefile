EXEC = GereVendas
CFLAGS = -Wall -Wextra -Wno-unused-function -Wno-unused-result -ansi -pedantic -O2
OBJS = $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))
TARGET_ARCH := -march=native

# diretorias onde o utilitário 'make' vai procurar pelas dependências e objetivos da makefile
VPATH = src/

all: $(EXEC)

.PHONY: all debug profile leak-check limpar

$(EXEC): $(OBJS)
	$(LINK.c) $(OBJS) $(OUTPUT_OPTION)

profile: CFLAGS += -pg
profile: $(EXEC)
	./$(EXEC)
	gprof gereVendas gmon.out

debug: CFLAGS = -Wall -Wextra -ansi -pedantic -O0 -g
debug: $(EXEC)
	gdb ./$(EXEC)

leak-check: CFLAGS = -Wall -Wextra -ansi -pedantic -O0 -g
leak-check: $(EXEC)
	valgrind --tool=memcheck --leak-check=yes ./$(EXEC)

limpar:
	$(RM) src/*.o
	$(RM) GereVendas
	$(RM) gmon.out
