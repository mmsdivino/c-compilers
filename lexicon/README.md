# lexicon

An implementation of lexicon analyzer.

In folder **Test**, there are examples to execute code.

This code is a project for subject Compilers (MATA61) in Federal University of Bahia. Follow description in Portuguese.

## Analisador léxico

Cada equipe deve desenvolver um analisador léxico na linguagem C que receba um nome de arquivo como parâmetro. O exemplo abaixo mostra como o programa será testado.

Ex:
```
./a.out nome_do_arquivo.
```

Se o arquivo de entrada tiver qualquer caractere for a da faixa especificada, o programa deve imprimir “ARQUIVO INVÁLIDO!\n” e não deve executar a análise léxica.

Caso o arquivo seja válido, o analisador deve imprimir a sentença “LINHA $num$: $string$\n” cada vez que um erro léxico for encontrado, onde $num$ é o número da linha e $string$ é o elemento inválido.

Ex:
```
declare a123, b456, 56dfg as letter.
REAL a, b.
a = c*alfa*0,345;
RETORNAR '\a'.
IMPRIME b
sur%Gnsa$trq<<a123.
```

Saída:
```
LINHA 1: 56dfg
LINHA 3: ;
LINHA 4: '\a'
LINHA 6: $
```


