#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_TAM 1000000000

void strlwr(char str[MAX_TAM]) {
	size_t i;
	size_t len = strlen(str);

	for (i = 0; i < len; i++)
		str[i] = tolower((unsigned char) str[i]);
}

void imprimeErroComTab(char line[MAX_TAM], int where, int a, int b) {
	int c;
	printf("LINHA %d: ", where);
	for (c = a; c <= b; c++)
		printf("%c", line[c]);
	printf("\n");
}

void imprimeErroSemTab(char line[MAX_TAM], int where, int a, int b) {
	int c;
	printf("LINHA %d: ", where);
	for (c = a; c < b; c++)
		printf("%c", line[c]);
	printf("\n");
}

/*int buscaCaracterInvalidoR(char line[MAX_TAM], int where, int j) {
 //printf("j:%d\n",j);
 while ((line[j] != '\0') && (line[j] != 39) && (line[j] != 34) && !(line[j] >= 48 && line[j] <= 57)) {
 if ((line[j] == 33 || line[j] == 35 || line[j] == 36 || line[j] == 38
 || line[j] == 58 || line[j] == 59 || line[j] == 63
 || line[j] == 64 || (line[j] == 92)
 || (line[j] >= 94 && line[j] <= 96)
 || (line[j] >= 123 && line[j] <= 126)
 || (line[j] == 46 && line[j + 1] == 46))) {
 printf("LINHA %d: %c\n", where, line[j]);
 }
 j++;
 }
 return j-1;
 }*/

/*void buscaCaracterInvalido(char line[MAX_TAM], int where, int j) {
 //printf("j:%d\n",j);
 while ((line[j] != '\0') && (line[j] != 39) && (line[j] != 34) && !(line[j] >= 48 && line[j] <= 57)) {
 if ((line[j] == 33 || line[j] == 35 || line[j] == 36 || line[j] == 38
 || line[j] == 58 || line[j] == 59 || line[j] == 63
 || line[j] == 64 || (line[j] == 92)
 || (line[j] >= 94 && line[j] <= 96)
 || (line[j] >= 123 && line[j] <= 126)
 || (line[j] == 46 && line[j + 1] == 46))) {
 printf("LINHA %d: %c\n", where, line[j]);
 }
 j++;
 }
 }*/

int numberGreaterThanAndInVariableR(char line[MAX_TAM], int where, int j) {
	int k, l, m;
	//while ((line[j] != '\0') && (line[j] != 39) && (line[j] != 34) && (line[j]!=) {
	if ((line[j] >= 48 && line[j] <= 57)
			&& (line[j - 1] >= 97 && line[j - 1] <= 122)) { //se for numero e anteriormente tiver uma letra
		for (m = j + 1; m < strlen(line) - 1; m++) {
			if (!(line[m] >= 48 && line[m] <= 57)) {
				//printf("m=%d\n",m);
				break;
			}
		}
		return m;
	} else if ((line[j] >= 48 && line[j] <= 57)) { //se for numero
		for (k = j + 1; k < strlen(line) - 1; k++) {
			if ((line[k] >= 0 && line[k] <= 31)
					|| (line[k] >= 32 && line[k] <= 47)
					|| (line[k] >= 58 && line[k] <= 96)
					|| (line[k] >= 123 && line[k] <= 127))
				break;
		}
		//printf("j=%d, k=%d\n",j,k);
		if (line[k - 1] >= 97 && line[k - 1] <= 122) { //se o ultimo caracter antes do break for letras (ex: 123abc)
			imprimeErroComTab(line, where, j, k - 1);
			j = k - 1;
		} else if (line[k - 1] >= 48 && line[k - 1] <= 57) { //se o ultimo caracter antes do break for numeros
		//verificar se entre j e k existe só digitos ou só caracteres
			for (l = j; l <= k - 1; l++) {
				if (line[l] >= 97 && line[l] <= 122)
					break;
			}
			if (l < k) { //se ele encontrou um caracter entre os numeros
				imprimeErroComTab(line, where, j, k - 1);
				j = k - 1;
			} else { //se ele nao encontrou caracter no meio dos numeros
				if (k - 1 - j >= 10)
					imprimeErroComTab(line, where, j, k - 1); //se a quantidade de digitos for maior que 10
				// "abc" 12345678901'\0' j=6; line[j]=1
				// k=17, line[k]='\0'
				// k-1 = 16, line[k-1]=1
				j = k - 1;
			}
		}
	}
	//j++;
	//}
	return (k - 1);
}

/*void numberGreaterThanAndInVariable(char line[MAX_TAM], int where, int j) {
 int k, l;
 while ((line[j] != '\0') && (line[j] != 39) && (line[j] != 34)) {
 if (line[j] >= 48 && line[j] <= 57) { //se for numero
 for (k = j + 1; k < strlen(line) - 1; k++) {
 if ((line[k] >= 0 && line[k] <= 31)
 || (line[k] >= 32 && line[k] <= 47)
 || (line[k] >= 58 && line[k] <= 96)
 || (line[k] >= 123 && line[k] <= 127))
 break;
 }
 if (line[k - 1] >= 97 && line[k - 1] <= 122) { //se o ultimo caracter antes do break for letras (ex: 123abc)
 imprimeErroComTab(line, where, j, k - 1);
 j = k - 1;
 } else if (line[k - 1] >= 48 && line[k - 1] <= 57) { //se o ultimo caracter antes do break for numeros
 //verificar se entre j e k existe só digitos ou só caracteres
 for (l = j; l <= k - 1; l++) {
 if (line[l] >= 97 && line[l] <= 122)
 break;
 }
 if (l < k) { //se ele encontrou um caracter entre os numeros
 imprimeErroComTab(line, where, j, k - 1);
 j = k - 1;
 } else { //se ele nao encontrou caracter no meio dos numeros
 if (k - 1 - j >= 10)
 imprimeErroComTab(line, where, j, k - 1); //se a quantidade de digitos for maior que 10
 // "abc" 12345678901'\0' j=6; line[j]=1
 // k=17, line[k]='\0'
 // k-1 = 16, line[k-1]=1
 j = k - 1;
 }
 }
 }
 j++;
 }
 }*/

void singleAndDoubleQuotes(char line[MAX_TAM], int where) {
	int l, m, n, cont, contcb;
	int j = 0;
	while (line[j] != '\0') {
		//printf("line[j]=%c\n",line[j]);
		if ((line[j] == '\'')) {
			for (l = j + 1; l < strlen(line) - 1; l++) { //para quando achar uma aspa de fechamento
				if (((line[l] == '\'') && (line[l - 1] != '\\'))
						|| (line[l] == '\t'))
					break;
			}
			if ((line[l] == '\t')) { //se ele achou tab
				imprimeErroComTab(line, where, j, l - 1);
				//buscaCaracterInvalido(line, where, l);
				//numberGreaterThanAndInVariable(line, where, l);
				j = l;
			} else if (line[l] == '\'') { //se ele achou uma aspa de fechamento
				//se o usuário colocar '\\' '
				if (line[j + 1] == '\\' && line[j + 2] == '\\'
						&& line[j + 3] == '\'') {	//se o usuario colocou '\\'
					j = j + 3;
				} else {
					//verificar quantidade de caracteres
					cont = 0;
					for (m = j + 1; m < l; m++) {
						if (line[m] == '\\'
								&& (line[m + 1] == 't' || line[m + 1] == 'n'
										|| line[m + 1] == '\\'
										|| line[m + 1] == '\''
										|| line[m + 1] == '"')) {
							cont++;
							m = m + 1;
						} else
							cont++;
					}
					if (cont == 0) { //se o usuario colocou ''
						imprimeErroComTab(line, where, j, l);
						//buscaCaracterInvalido(line, where, l + 1);
						//numberGreaterThanAndInVariable(line, where, l + 1);
						j = l;
					} else if (cont == 1) {
						if (line[j + 1] == '"') { //se o usuario colocou '"'
							imprimeErroComTab(line, where, j, l);
							//buscaCaracterInvalido(line, where, l + 1);
							//numberGreaterThanAndInVariable(line, where, l + 1);
							j = l;
						} else { //se o usuaro colocou certinho
							//buscaCaracterInvalido(line, where, l + 1);
							//numberGreaterThanAndInVariable(line, where, l + 1);
							j = l;
						}
					} else if (cont > 1) { //se o caracter é maior que a permitida ex: 'abc' '\a'
						imprimeErroComTab(line, where, j, l);
						//buscaCaracterInvalido(line, where, l + 1);
						//numberGreaterThanAndInVariable(line, where, l + 1);
						j = l;
					}
				}
			} else { //se nao achou nem tab nem aspas de fechamento
				if (line[j + 1] == '\\' && line[j + 2] == '\\'
						&& line[j + 3] == '\'') { //se o usuario SOMENTE colocou '\\'
					j = j + 3;
				} else {
					imprimeErroSemTab(line, where, j, strlen(line) - 1);
					break;
				}
			}
		}
		/* SE FOR ASPAS DUPLAS */
		else if ((line[j] == '"')) {
			for (l = j + 1; l < strlen(line) - 1; l++) { //para quando achar uma aspa de fechamento ou tab
				if (((line[l] == '"') && (line[l - 1] != '\\'))
						|| (line[l] == '\t'))
					break;
			}
			if (line[l] == '\t') { //se ele achou tab
				imprimeErroComTab(line, where, j, l - 1); //se encontrou tab
				//buscaCaracterInvalido(line, where, l);
				//numberGreaterThanAndInVariable(line, where, l);
				j = l;
			} else if (line[l] == '"') { //se ele achou uma aspa de fechamento
				if (line[j + 1] == '\\' && line[j + 2] == '\\'
						&& line[j + 3] == '"') { //se o usuario colocou "\\"
					j = j + 3;
				} else {
					//contar quantos caracteres depois das aspas
					cont = 0;
					for (m = j + 1; m < l; m++) {
						if (line[m] == '\\'
								&& (line[m + 1] == 't' || line[m + 1] == 'n'
										|| line[m + 1] == '\\'
										|| line[m + 1] == '\''
										|| line[m + 1] == '"')) {
							cont++;
							m = m + 1;
						} else
							cont++;
					}
					//printf("cont:%d\n",cont);
					if (cont == 1) {
						if (line[j + 1] == '\'') { // exemplo "'"
							imprimeErroComTab(line, where, j, l);
							//buscaCaracterInvalido(line, where, l + 1);
							//numberGreaterThanAndInVariable(line, where, l + 1);
							j = l;
						} else { // exemplo :"a"
							//buscaCaracterInvalido(line, where, l + 1);
							//numberGreaterThanAndInVariable(line, where, l + 1);
							j = l;
						}
					} else if (cont >= 256) { //se a string é maior que a permitida
						imprimeErroComTab(line, where, j, l);
						//buscaCaracterInvalido(line, where, l);
						//numberGreaterThanAndInVariable(line, where, l);
						j = l;
					} else { //verificar se existe caracteres invalidos exemplo: "aihaiusuasash\aahausha"
						if (line[l - 1] == '"' && line[l - 2] == '\\') {
							//contar quantas contra barras tem "\\\\"" (erro) "\\\\\"" (certo)
							contcb = 0;
							for (n = l - 3; n != j; n--) {
								if (line[n] == '\\')
									contcb++;
							}
							if (contcb % 2 != 1) {
								//buscaCaracterInvalido(line, where, l + 1);
								//numberGreaterThanAndInVariable(line, where,l + 1);
								j = l;
							} else {
								//buscaCaracterInvalido(line, where, l);
								//numberGreaterThanAndInVariable(line, where, l);
								j = l - 1;
							}
						} else { //exemplo: "usaushaushaushaush" ou "aushashaushaushau\quahsuashaus"
							m = j + 1;
							while (m < l) {
								if ((line[m] == '\\')
										&& !(line[m + 1] == 't'
												|| line[m + 1] == 'n'
												|| line[m + 1] == '\\'
												|| line[m + 1] == '\''
												|| line[m + 1] == '"')) {
									imprimeErroComTab(line, where, j, l);
									m = m + 1;
								} else if ((line[m] == '\\')
										&& (line[m + 1] == 't'
												|| line[m + 1] == 'n'
												|| line[m + 1] == '\\'
												|| line[m + 1] == '\''
												|| line[m + 1] == '"'))
									m = m + 1;
								m++;
							}
							for (m = j + 1; m < l; m++) { // exemplo: " eu sou muito legal 'amor' "
								if ((line[m] == '\'')
										&& !(line[m - 1] == '\\')) {
									imprimeErroComTab(line, where, j, l);
									//buscaCaracterInvalido(line, where, l + 1);
									//numberGreaterThanAndInVariable(line, where, l + 1);
									break;
								}
							} // "abc" "asd"
							  //buscaCaracterInvalido(line, where, l + 1);
							  //numberGreaterThanAndInVariable(line, where, l + 1);
							j = l;
						}
					}
				}
			} else { //se não achou nem tab nem aspas de fechamento exemplo: "aushaushaushauhsuahsuash
				if (line[j + 1] == '\\' && line[j + 2] == '\\'
						&& line[j + 3] == '"') {
					j = j + 3;
				} else {
					imprimeErroSemTab(line, where, j, strlen(line) - 1);
					break;
				}
			}
		} else if (line[j] >= 48 && line[j] <= 57)
			j = numberGreaterThanAndInVariableR(line, where, j);
		else if ((line[j] == 33 || line[j] == 35 || line[j] == 36
				|| line[j] == 38 || line[j] == 58 || line[j] == 59
				|| line[j] == 63 || line[j] == 64 || (line[j] == 92)
				|| (line[j] >= 94 && line[j] <= 96)
				|| (line[j] >= 123 && line[j] <= 126)
				|| (line[j] == 46 && line[j + 1] == 46)))
			printf("LINHA %d: %c\n", where, line[j]);
		j++;
	}
}

char temp[MAX_TAM];
int i, linha = 1, ch;
int main(int argc, char **argv) {
	if (argc != 2) {
	 printf("Sintaxe: %s nome_do_arquivo.txt\n", argv[0]);
	 return 0;
	}
	
	FILE *arquivo;
	arquivo = fopen(argv[1], "r");
	if (arquivo == NULL) {
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return 0;
	}
	while ((ch = fgetc(arquivo)) != EOF) {
		if (ch > 126) {
			printf("ARQUIVO INVÁLIDO!\n");
			fclose(arquivo);
			return 0;
		}
	}

	fclose(arquivo);
	arquivo = fopen(argv[1], "r");
	if (arquivo == NULL) {
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return 0;
	}
	while (fgets(temp, sizeof temp, arquivo) != NULL) {
		strlwr(temp); //converter a string para minuscula
		singleAndDoubleQuotes(temp, linha);
		linha++;
	}
	fclose(arquivo);
	return 0;
}
