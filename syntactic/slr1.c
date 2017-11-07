#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define validNumber(a) ((a >= '0') && (a <= '9') ? 1:0)
#define validSpace(a) ((a == ' ') || (a <= '\t') ? 1:0)
#define validChar(a) ((a!=9) && (a!=10) && ((a<32) || (a>126)) ? 1:0)
#define validSeparator(a) ((a == ',') || (a == '.') || (a == '[') || (a == ']') || (a == '+') || (a == '-') || (a == '*') || (a == '/') || (a == '%') || (a == '(') || (a == ')') || (a == '<') || (a == '>') || (a == '=') ? 1:0)
#define validLetter(a) (((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z')) ? 1:0)
#define RULES_LINE	69
#define RULES_COLUMN	9
#define	STATUS_LINE	124
#define STATUS_COLUMN	69

/* VARIAVEIS GLOBAIS */
char rules[RULES_LINE][RULES_COLUMN]={"$","%","(",")","*","+",",","-",".","/","<","<=","<>","=",">",">=","[","]","as","constant","declare","do","else","for","foreach","from","id","if","in","letter","number","print","put","read","resize","then","to","C","COMANDOS","D","DE","ELSE","EXP","EXP_2","EXR","F","FE","FOR","FORE","I","IF","L","M","OPERANDO","OPR","P","PR","PT","PU","R","RE","RS","RSE","S","T","TIPO","TYPE","T_2","VETOR"};
char productions[RULES_LINE][RULES_COLUMN]=	{"S","COMANDOS","COMANDOS","C","C","C","C","C","C","C","C","D","DE","L","L","VETOR","VETOR","TIPO","TIPO","RS","RSE","EXP","EXP","EXP","T","T","T","T","M","M","M","M","OPERANDO","OPERANDO","PU","PT","T_2","EXP_2","EXP_2","I","IF","ELSE","ELSE","EXR","OPR","OPR","OPR","OPR","OPR","OPR","F","FOR","FE","FORE","R","RE","P","PR","TYPE","TYPE"};
int size[60]={1,2,0,1,1,1,1,1,1,1,1,1,5,4,2,2,0,1,1,1,6,3,3,1,3,3,3,1,4,3,2,1,2,1,1,6,1,3,0,1,7,4,0,3,1,1,1,1,1,1,1,11,1,9,1,4,1,3,2,1};
int line = 1;
char *token;
int validLexico;

/*
 * 
 * 					FUNÇÕES DA ANÁLISE SINTÁTICA
 * 
 * 
 */

/* ESTRUTURA E FUNÇÕES FILA */
typedef struct no{
	char * token;
	struct no* next;
}No;

No *rear, *front;

void init_queue(){
	rear = front = NULL;
}

void addq(char* word){
	No *t=(No*)malloc(sizeof(No));
	t->token=(char*)malloc(sizeof(char));
	strcpy(t->token, word);
	t->next=NULL;
	if (!front) rear = front = t;
	else {
		rear->next=t; rear=rear->next;
	}
}

/* FUNÇÃO PARA ADICIONAR NO INICIO DA LISTA */
void addqe(char*word){
	No *t=(No*)malloc(sizeof(No));
	t->token=(char*)malloc(sizeof(char));
	strcpy(t->token,word);
	if (!front) rear = front = t;
	else{
		t->next=front;
		front=t;
	}
}

char* delq(){
	if (!front) {
		printf("Fila vazia.\n");
		return NULL;
	}
	else{
		No* temp;
		char* word = (char*)malloc(sizeof(char));
		strcpy(word,front->token);
		temp=front; front=front->next;
		free(temp);
		return word;
	}
}

void displayq(){
	if (!front) printf("Fila vazia.\n");
	else {
		No*temp=front;
		printf("Elementos na fila:\n");
		while(temp){
			printf("%s\n",temp->token);
			temp=temp->next;
		}
	}
}

/* ESTRUTURA E FUNÇÕES PILHA */
typedef struct node{
	int data;
	struct node *link;
} Node;

Node *status = NULL;

void push(int j){
	Node* m = (Node*)malloc(sizeof(Node));
	m->data = j; m->link = status;
	status=m;
}

int pop (){
	if (!status){
		//printf("Pilha vazia.\n");
		return -1;
	}
	else {
		Node* temp = NULL;
		int i = status->data;
		temp = status; status = status->link;
		free(temp);
		return i;
	}
}

void displays(){
	if (!status) printf("Pilha vazia.\n");
	else{
		Node* temp = status;
		while(temp){
			printf("%d\n",temp->data);
			temp = temp->link;
		}
	}
}

/* FUNÇÃO PARA LOCALIZAR A POSIÇÃO NO VETOR RULES */
int buscaIndice(char* word){
	int i;
	for (i=0; i<RULES_LINE; i++){
		if (strcmp(rules[i],word)==0) break;
	}
	return i;
}

/* FUNÇÃO QUE RETORNA NOME DA PRODUÇÃO */
char* producao (int numero){
	return (productions[numero-1]);
}

/* FUNÇÃO QUE RETORNA NUMERO DE VEZES PARA DESEMPILHAR NA PILHA STATUS*/
int tamanho (int numero){
	return (size[numero-1]);
}

/* FUNÇÃO PARA DESEMPILHAR N VEZES NA PILHA STATUS */
int desempilhe(int numero){
	int i=0;
	int retorno;
	while (i<numero){
		retorno = pop();
		i++;
	}
	return retorno;
}


/*
 * 
 * 					FUNÇÕES DA ANÁLISE LÉXICA
 * 
 * 
 */
 
char * strlwr(char *str) {
	size_t i;
	size_t len = strlen(str);

	for (i = 0; i < len; i++)
		str[i] = tolower((unsigned char) str[i]);
		
	return str;
}


int identifier(char *key) {
	int i =1, j;
 	while (validNumber(key[i]) || validLetter(key[i])) {
		token[i] = key[i];
		i++;
	}
	token[i] = '\0';
	char *tokenLower = strlwr(token);
	//VERIFICAR SE É PALAVRA RESERVADA
	for (j=18; j<37; j++){
		if (strcmp(tokenLower,rules[j])==0){
			addq(tokenLower);
			return i;
		}
	}
	addq("id");
	return i;
}

int treatNumber(char *key) {
	int i=1, erro=0;
	
	while (validNumber(key[i]) || validLetter(key[i])) {
		token[i] = key[i];
		if (validLetter(key[i]))
			erro = 1;
		i++;
	}
	
	token[i] = '\0';	
	if (erro || (strlen(token) > 10)) {
		printf("LINHA %d: %s\n",line,token);		
		validLexico = 1;
	}
	else addq("constant");
	return i;
}

int treatseparador(char *key) {
	int i=1;
    /* LEMBRETE: =>, <=, <> são um token */
	if ((token[0] == '<') && ((key[1] == '=') || (key[1] == '>'))){
		i++;
		token[1] = key[1];
	}
	else if ((token[0] == '>') && (key[1] == '=')){
		i++;
		token[1] = key[1];
	}
	token[i] = '\0';
	addq(token);	
	return i;
}

int treatString(char *key) {
	int i =1, erro = 0, qntd = 0; 
	
	while (key[i] != '\"') {	
		token[i] = key[i];
		if (key[i] == '\\') {
			i++;
			token[i] = key[i];
			if ((key[i] != 't') && (key[i] != 'n') && (key[i] != '\"') && (key[i] != '\'') && (key[i] != '\\'))  
				erro = 1;
		}
		else if (key[i] == '\'') erro = 1;
		else if ((key[i] == '\n') || (key[i] == '\t') || (key[i] == EOF)) {
			erro = 1;
			break;
		}			
		qntd++;
		i++;
	}	
	
	if ((key[i] != '\n') && (key[i] != '\t') && (key[i] != EOF)) {
		token[i] = key[i];
		i++;
	}
		
	token[i] = '\0';
	
	if ((erro) || (qntd > 256)) {
		printf("LINHA %d: %s\n",line,token);			
		validLexico = 1;
	}
	else addq("constant");
	return i;
}

int treatChar(char *key) {
	int i=1, erro = 0, qntd = 0; 
	
	while (key[i] != '\'') {		
		token[i] = key[i];
		if (key[i] == '\\') {
			i++;
			token[i] = key[i];
			if ((key[i] != 't') && (key[i] != 'n') && (key[i] != '\"') && (key[i] != '\'') && (key[i] != '\\'))  
				erro = 1;
		}
		else if (key[i] == '\"') erro = 1;
		else if ((key[i] == '\n') || (key[i] == '\t') || (key[i] == EOF)) {
			erro = 1;
			break;
		}			
		qntd++;
		i++;
	}	
	
	if ((key[i] != '\n') && (key[i] != '\t') && (key[i] != EOF)) {
		token[i] = key[i];
		i++;
	}
	token[i] = '\0';
	if ((erro) || (qntd != 1)) {
		printf("LINHA %d: %s\n",line,token);		
		validLexico = 1;
	}
	
	else addq("constant");
	return i;
}

int main(int argc, char **argv) {
	
	/* ABRIR CÓDIGO DO USUÁRIO */
    FILE *arquivo = fopen(argv[1], "r");
    char c;
    int i=0, j;
    struct stat status;
	stat(argv[1], &status);
	int size = status.st_size+1;
	char Key[size];
	token = (char*)malloc(sizeof(size)); 
	
	if (!arquivo) {		
		return 1;
	}
	else {
		while ((c=fgetc(arquivo)) != EOF){
			if (validChar(c)) {
				printf("ARQUIVO INVALIDO!\n");
				return 0;
			}
			Key[i] = c;
			i++;
		}
		Key[i] = c;
	}
	fclose(arquivo);
	
	/* ABRIR ARQUIVO COM A TABELA SLR(1)*/
	int tabela[STATUS_LINE][STATUS_COLUMN];
	struct stat table;
	stat("statusTable.txt",&table);
	
	FILE *matriz = fopen("statusTable.txt", "r");
	if (matriz == NULL) {
		printf("Erro, nao foi possivel abrir o texto!\n");
		return 0;
	}

	i=0;
	char *t1=(char*)malloc(table.st_size);
	while (fgets(t1, table.st_size, matriz) != NULL) {
		char *num=strtok(t1,"\t");
		j=0;
		while (num!=NULL){
			int val = atoi(num);
			tabela[i][j]=val;
			j++;
			num=strtok(NULL,"\t");
		}
		i++;
	}

	free(t1);
	fclose(matriz);
				
	i = validLexico = 0;
	init_queue(); //INICIANDO FILA
	
	while (i < (size-1)) {
		if (validSpace(Key[i])) {
			i++;
		}
		else if (validSeparator(Key[i])) {
			token[0] = Key[i];
			i+= treatseparador(Key+i);		
		}
		else if (validLetter(Key[i])) {
			token[0] = Key[i];
			i += identifier(Key+i);
		}
		else if (validNumber(Key[i])) {
			token[0] = Key[i];
			i += treatNumber(Key+i);		
		}
		else if (Key[i] == '\'') {
			token[0] = Key[i];
			i+= treatChar(Key+i);
		}
		else if (Key[i] == '\"') {
			token[0] = Key[i];
			i+= treatString(Key+i);
		}
		else { 
			if (Key[i] == '\n') 
				line++;
			else {  
				printf("LINHA %d: %c\n",line,Key[i]);
				validLexico = 1;
			}
			i++;
		}
	}
	if (!validLexico) {
		addq("$");
		//displayq();
		char* word = (char*)malloc(sizeof(char));
		int estado = 0;
		while (front){
			strcpy(word,delq());
			int prox = buscaIndice(word);
			int acao = tabela[estado][prox];
			//printf("%d %s %d\n",estado,word,acao);
			if (acao/200==0){ //ou seja, se ação for estado
				push(estado);
				estado=acao;
			}
			if (acao/200==1){ //ou seja, se ação é uma redução
				acao=acao%200;
				char* p=producao(acao);
				int n=tamanho(acao);
				if (n){
					int dest=desempilhe(n);
					estado=dest;
				}
				addqe(word);
				addqe(p);
			}
			if (acao==0){
				printf("SIM\n");
				break;
			} //accept
			if (acao==-1){
				printf("NAO\n");
				break;
			}
		}
	}
	return 0;
}
