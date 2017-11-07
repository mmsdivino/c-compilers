# Regra default para criar o programa
all: semantic Makefile.uptodate

# Compilando o programa
semantic.o: semantic.c
	gcc -g -Wall -c semantic.c

# Ligando o programa
semantic: semantic.o
	gcc -g semantic.o -o a.out -ggdb

# Limpando tudo o que foi gerado automaticamente pelo comando clean
Makefile.uptodate: Makefile
	make clean
clean:
	rm -rf *.o
