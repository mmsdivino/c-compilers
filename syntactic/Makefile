# Regra default para criar o programa
all: slr1 Makefile.uptodate

# Compilando o programa
trabalho1.o: slr1.c
	gcc -g -Wall -c slr1.c

# Ligando o programa
trabalho1: slr1.o
	gcc -g slr1.o -o slr1 -ggdb

# Limpando tudo o que foi gerado automaticamente pelo comando clean
Makefile.uptodate: Makefile
	make clean
clean:
	rm -rf *.o
