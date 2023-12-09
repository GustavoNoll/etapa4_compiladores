# Makefile para compilar o analisador léxico

# Integrantes do grupo V:
# - Bruno Marques Bastos (314518)
# - Gustavo Lopes Noll (322864)

# Compilador a ser usado
CC = gcc

# Ferramentas 
LEX = flex
YACC = bison

# Opções de compilação
CFLAGS = -Wall -Wextra

# Nome do executável
EXECUTABLE = etapa3

# Listagem de arquivos fonte
SOURCES = lex.yy.c parser.tab.c functions.c ast.c main.c valor_lexico.c

# Objetos gerados
OBJECTS = $(SOURCES:.c=.o)

# Alvo padrão
all: $(EXECUTABLE)

# Dependências para construção do executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra genérica para compilar arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra para construir o analisador sintático
parser.tab.c: parser.y valor_lexico.h ast.h functions.h
	$(YACC) -v -d $< -o $@

# Regra para construir o analisador léxico
lex.yy.c: scanner.l parser.tab.c
	$(LEX) $<

# Alvo para execução do programa
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Alvo para limpar arquivos gerados
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) lex.yy.c parser.tab.* parser.output
