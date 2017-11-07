# Regra default para criar o programa
all: lexicon Makefile.uptodate

# Compilando o programa
lexicon.o: lexicon.c
	gcc -g -Wall -c lexicon.c

# Ligando o programa
lexicon: lexicon.o
	gcc -g lexicon.o -o a.out

# Limpando tudo o que foi gerado automaticamente pelo comando clean
Makefile.uptodate: Makefile
	make clean
clean:
	rm -rf *.o
