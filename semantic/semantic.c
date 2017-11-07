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
#define validOP(a) ((a == '+') || (a == '-') || (a == '*') || (a == '/') || (a == '%') || (a == '(') || (a == ')') ? 1:0)
#define validOR(a) ((a == '<') || (a == '>') || (a == '=') ? 1:0)
#define validLetter(a) (((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z')) ? 1:0)
#define validConsChar(a) ((a=='\'') ? 1:0)
#define validString(a) ((a=='"') ? 1:0)
#define RULES_LINE	63
#define RULES_COLUMN	9
#define	STATUS_LINE	124
#define STATUS_COLUMN	69

/* VARIAVEIS GLOBAIS */
char rules[RULES_LINE][RULES_COLUMN]={"$","%","(",")","*","+",",","-",".","/","<","<=","<>","=",">",">=","[","]","as","char","declare","do","else","for","foreach","from","id","if","in","int","letter","number","print","put","read","resize","string","then","to","C","COMANDOS","D","ELSE","EXP","EXP_2","EXR","F","FE","I","L","M","OPERANDO","OPR","P","PU","R","RS","S","T","TIPO","TYPE","T_2","VETOR"};
char productions[RULES_LINE][RULES_COLUMN]=	{"S","COMANDOS","COMANDOS","C","C","C","C","C","C","C","C","D","L","L","VETOR","VETOR","TIPO","TIPO","RS","EXP","EXP","EXP","T","T","T","T","M","M","M","M","OPERANDO","OPERANDO","OPERANDO","PU","PU","T_2","EXP_2","EXP_2","I","ELSE","ELSE","EXR","OPR","OPR","OPR","OPR","OPR","OPR","F","FE","R","P","TYPE","TYPE","TYPE","TYPE"};
int size[56]=								{1,		2,			0,		1,	1,	1,	1,	1,	1,	1,	1,	5,	4,	2,		2,		0,	1,		1,		6,	3,		3,	1,	 3,  3,	 3,	 1,	 4,	 3,	 2,	 1,		2,			1,			1,	  6,	5,	1,		3,		0,	  7,	4,		0,	 3,	   1,	 1,	   1,	 1,	   1,	 1,	  11,  9,  4,  3,   2,		1,	   1,		1};
int line = 1;
char *token;
int validLexico, validSintatico, cont, erro=0;
char* last = NULL;

/* 					FUNÇÕES DA ANÁLISE SINTÁTICA
 */

/* ESTRUTURA E FUNÇÕES FILA */
typedef struct no{
	char * token;
	struct no* next;
}No;

No *rear, *front; //fila da análise sintática
No *rearCopy, *frontCopy; //fila da análise semântica


void init_queue(){
	rear = front = NULL;
	rearCopy = frontCopy = NULL;
}

/* FUNÇÃO PARA ADICIONAR NA FILA DA ANALISE SINTATICA */
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

/* FUNÇÃO PARA ADICIONAR NA FILA DA ANALISE SEMÂNTICA */
void addc(char* word){
	No *t=(No*)malloc(sizeof(No));
	t->token=(char*)malloc(sizeof(char));
	strcpy(t->token, word);
	t->next=NULL;
	if (!frontCopy) rearCopy = frontCopy = t;
	else {
		rearCopy->next=t; rearCopy=rearCopy->next;
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

/* FUNÇÃO PARA RETIRAR NA FILA DA ANALISE SINTATICA */
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

/* FUNÇÃO PARA RETIRAR NA FILA DA ANALISE SEMANTICA*/
char* delc(){
	if (!frontCopy) {
		printf("Fila vazia.\n");
		return NULL;
	}
	else{
		No* temp;
		char* word = (char*)malloc(sizeof(char));
		strcpy(word,frontCopy->token);
		temp=frontCopy; frontCopy=frontCopy->next;
		free(temp);
		return word;
	}
}

/* FUNÇÃO PARA EXIBIR FILA SINTATICA */
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

/* FUNÇÃO PARA EXIBIR FILA SEMANTICA */
void displayc(){
	if (!frontCopy) printf("Fila vazia.\n");
	else {
		No*temp=frontCopy;
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

/* 					FUNÇÕES DA ANÁLISE SEMANTICA 
 */

typedef struct NoSemantica{
	char* name; //nome da variavel
	char* type; //number ou letter
	int eh_vetor;
	int eh_global;
	int index; //se for vetor, tamanho do vetor
	struct NoSemantica *next;
}Lista;

Lista *root = NULL;

void displayl(){
    if(root == NULL) printf("Lista vazia\n");
    else{
        Lista*p=root;
        while(p!= NULL){
            printf("%s %d %d %s\n",p->name,p->eh_vetor,p->index, p->type);
            p=p->next;
        }
    }
}

void insertSemantica (char* str, int global){
	Lista *new;
	if(!(new=(Lista*)malloc(sizeof(Lista)))){
		printf("Falta memoria!\n");
		return;
	}
	new->name = (char*)malloc(sizeof(char));
	new->type = (char*)malloc(sizeof(char));
	strcpy(new->name,str);
	new->eh_vetor = 0;
	new->index=0;
	new->eh_global=global;
	new->next =NULL;

	if (root==NULL) root=new;
	else{
		Lista *p;
		for(p=root; p->next!=NULL; p=p->next);
		p->next = new;
	}
}

void delete(int contador){
	if(!root) return;
	Lista *end=NULL;
	while (contador){
		Lista *begin=root;
		while(begin->next != NULL){
			end = begin;
			begin = begin->next;
		}
		if(end != NULL) end->next = NULL;
		free(begin);
		contador--;
	}
}

int searchToken(char *str){ 
	Lista *p;
	for(p = root;p; p=p->next){
		if(strcmp(p->name,str)==0 && !p->eh_global)
			return 1;
	}
	for(p = root;p; p=p->next){
		if(strcmp(p->name,str)==0 && p->eh_global)
			return 1;
	}
	return 0;
}

int searchTokenGlobal(char *str, int global){ 
	Lista *p;
	for(p = root;p; p=p->next){
		if(strcmp(p->name,str)==0 && p->eh_global==global)
			return 1;
	}
	return 0;
}

Lista *searchNoToken(char *str){
	Lista *p;
	for(p = root;p; p=p->next){
		if(strcmp(p->name,str)==0 && !p->eh_global)
			return p;
	}
	for(p = root;p; p=p->next){
		if(strcmp(p->name,str)==0 && p->eh_global)
			return p;
	}
	return NULL;
}

Lista *searchLastToken(){
	Lista *p;
	for (p=root; p->next; p=p->next);
	return p;
}

void changeType(char *begin, char *end, char *str, int global){
	Lista *p=root;
	for (;p;p=p->next){
		if (!strcmp(p->name,begin) && p->eh_global==global){
			strcpy(p->type,str);
			p=p->next;
			break;
		}
	}
	if (p){
		while (strcmp(p->name,end) && p->eh_global==global){
			strcpy(p->type,str);
			p=p->next;
		}
		p->type=str;
	}
}

void declarar(int global){
	cont=0;
	char* word, *primeiro, *ultimo;
	word = (char*)malloc(sizeof(char));
	primeiro = (char*)malloc(sizeof(char));
	ultimo = (char*)malloc(sizeof(char));
	
	strcpy(word,delc());
	strcpy(primeiro,word);
	strcpy(ultimo,word);
	if (searchTokenGlobal(word,global)){
		erro=1;
		return;
	}
	else insertSemantica(word,global);
	cont++;
	
	strcpy(word,delc());
	while (strcmp(word,"as")){
		if (!strcmp(word,"[")){// eh vetor
			Lista *p = searchLastToken(root);
			p->eh_vetor=1;
			strcpy(word,delc()); //receberá FECHA_COLCHETE
		}
		else if (!strcmp(word,",")){ //declaracao de uma nova variavel
			strcpy(word,delc());
			//verificar se existe essa variavel
			if (searchTokenGlobal(word,global)){//se sim
				erro=1;
				return;
			}
			else {
				insertSemantica(word,global);
				cont++;
				strcpy(ultimo,word);
			}
		}
		strcpy(word,delc());
	}
	strcpy(word,delc()); //receberá o tipo das variaveis
	changeType(primeiro,ultimo,word,global);
	strcpy(word,delc()); //receberá .
	if (global) cont=0;
}

void expressao(char* palavra){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,palavra);
	while ( strcmp(word,"to") && strcmp(word,".") && strcmp(word,"]") && strcmp(word,"do") &&
			!validOR(word[0]) && strcmp(word,"in") && strcmp(word,"then") ) {
		strcpy(last,word);
		//printf("%s\n",word);
		//displayl();
		if (validOP(word[0])) strcpy(word,delc()); //se for operações aritiméticas ou abre e fecha parenteses
		//verificar se existe variavel na lista
		else if (!searchToken(word)){ //se nao existe variavel na lista
			//verificar se e numero
			if (validNumber(word[0]) || validConsChar(word[0])) strcpy(word,delc());
			else { //erro semantico
				erro=1;
				return;
			}
		}
		else { //se existe variavel na lista
			// verificar se é vetor
			Lista *p=searchNoToken(word);
			strcpy(word,delc()); //receberá a proxima palavra depois do ID
			if (!strcmp(word,"[")){
				//verificar se id é vetor
				if ( (!p->eh_vetor) || (p->eh_vetor && !p->index)  ){ 
				/* caso a variavel existe mas nao eh vetor
				 * ou eh vetor mas não foi alocado
				 */
					erro=1;
					return;
				}
				else {
					strcpy(word,delc());
					expressao(word);
				}
			}
			else {
				Lista *q=searchNoToken(last);
				if (p->eh_vetor && p->index && q && p->eh_vetor!=q->eh_vetor){
					erro=1;
					return;
				}
			}
		}
	}
}

void alocacao(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc());
	//verificar se id foi declarado
	if (!searchToken(word)){//se nao
		erro=1;
		return;
	}
	else{
		//verificar se é vetor
		Lista *p=searchNoToken(word);
		if (!p->eh_vetor){//se nao é vetor
			erro=1;
			return;
		}
		else { //verificar se é alocação de vetor
			strcpy(word,delc());
			if (!strcmp(word,"[")) {
				expressao(delc());
				if (!erro){
					p->index=1;
				}
				else return;
			}
			else{ //então a palavra em word é TO
				expressao(delc());
				if (!erro) {
					p->index=1;
				}
				else return;
			}
		}
	}
}

void atribuicao(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc());
	//printf("%s\n",word);
	if (validString(word[0])){
		strcpy(word,delc()); //recebe IN
		strcpy(word,delc()); //recebe id
		if (!searchToken(word)){
			erro=1;
			return;
		}
		else {
			Lista *p = searchNoToken(word);
			if (!p->eh_vetor){ 
				erro=1;
				return;
			}
			else if (p->eh_vetor && !p->index){ 
				erro=1;
				return;
			}
			else if (p->eh_vetor && !strcmp(p->type,"number")){
				erro=1;
				return;
			}
			else {
				strcpy(word,delc()); //recebe ponto final
				return;
			}
		}
	}
	else { //é uma expressao aritmética
		expressao(word);
		strcpy(word,delc()); //recebe id
		//verificar se id foi declarado
		if (!searchToken(word)){//se nao
			erro=1;
			return;
		}
		else {
			// verificar se é vetor
			Lista *p=searchNoToken(word);
			strcpy(word,delc());
			if (!strcmp(word,"[")){
				if ( (!p->eh_vetor) || (p->eh_vetor && !p->index) ){ 
					/* caso nao exista a variavel 
					 * ou a variavel existe mas nao eh vetor
					 * ou eh vetor mas nao foi alocado
					 */
					erro=1;
					return;
				}
				else { //eh vetor
					expressao(delc());
					if (!erro){
						strcpy(word,delc()); //recebe ponto final
						return;
					}
				}
			}
			else {
				Lista *q=searchNoToken(last);
				if (q && p->eh_vetor!=q->eh_vetor){
					erro=1;
					return;
				}
			}
			
		}
	}	
}

void expressao_rel(){
	expressao(delc());
	if (!erro) expressao(delc());
	else return;
}

void leitura(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe id, number ou posicao de vetor
	//verificar se id foi declarado
	if (!searchToken(word)){//se nao
		erro=1;
		return;
	}
	else {
		// verificar se é vetor
		Lista *p=searchNoToken(word);
		strcpy(word,delc());
		if (!strcmp(word,"[")){
			//verificar se id é vetor
			if ( !p || (!p->eh_vetor) || (p->eh_vetor && !p->index) ){ 
				/* caso nao exista a variavel 
				 * ou a variavel existe mas nao eh vetor
				 * ou eh vetor mas nao foi alocado
				 */
				erro=1;
				return;
			}
			else {
				expressao(delc());
				if (erro) return;
				else strcpy(word,delc()); //recebe .
			}
		}
	}
}

void escrita(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe id, number ou posicao de vetor
	//verificar se id foi declarado
	if (!searchToken(word)){ //se nao existe variavel na lista
		//verificar se e numero, char ou string
		if ( !validNumber(word[0]) && !validConsChar(word[0]) && !validString(word[0]) ){
			erro=1;
			return;
		}
	}
	else { //se existe variavel na lista
		// verificar se é vetor
		Lista *p=searchNoToken(word);
		strcpy(word,delc()); //recebe .
		if (!strcmp(word,"[")){
			//verificar se id é vetor
			if ( !p || (!p->eh_vetor) || (p->eh_vetor && !p->index) ){ 
				/* caso nao exista a variavel 
				 * ou a variavel existe mas nao eh vetor
				 * ou eh vetor mas nao foi alocado
				 */
				erro=1;
				return;
			}
			else { //eh vetor do tipo number
				expressao(delc());
				if (erro) return;
				else strcpy(word,delc()); //recebe .
			}
		}
	}
}

void desvio_if();
void desvio_else();
void loop_for();
void loop_foreach();

void desvio_if(){
	expressao_rel();
	//Expressão para quando achar o THEN
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe [
	strcpy(word,delc()); //recebe COMANDOS
	while (strcmp(word,"]")){
			if (!strcmp(word,"declare")) declarar(0);
			else if (!strcmp(word,"put")) atribuicao();
			else if (!strcmp(word,"resize")) alocacao();
			else if (!strcmp(word,"if")) desvio_if();
			else if (!strcmp(word,"else")) desvio_else();
			else if (!strcmp(word,"for")) loop_for();
			else if (!strcmp(word,"foreach")) loop_foreach();
			else if (!strcmp(word,"print")) escrita();
			else if (!strcmp(word,"read")) leitura();
			if (erro) return;
			strcpy(word,delc());
	}
	delete(cont);
}

void desvio_else(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe ABRE_COLCHETES
	while (strcmp(word,"]")){
			//printf("%s\n",word);
			if (!strcmp(word,"declare")) declarar(0);
			else if (!strcmp(word,"put")) atribuicao();
			else if (!strcmp(word,"resize")) alocacao();
			else if (!strcmp(word,"if")) desvio_if();
			else if (!strcmp(word,"else")) desvio_else();
			else if (!strcmp(word,"for")) loop_for();
			else if (!strcmp(word,"foreach")) loop_foreach();
			else if (!strcmp(word,"print")) escrita();
			else if (!strcmp(word,"read")) leitura();
			if (erro) return;
			strcpy(word,delc());
	}
	delete(cont);
}

void loop_for(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe id, number ou posicao de vetor
	//verificar se id foi declarado
	if (!searchToken(word)){//se nao
		erro=1;
		return;
	}
	else {
		// verificar se é vetor
		Lista *p=searchNoToken(word);
		strcpy(word,delc());
		if (!strcmp(word,"[")){
			//verificar se id é vetor
			if ( !p || (!p->eh_vetor) || (p->eh_vetor && !p->index) ){ 
				/* caso nao exista a variavel 
				 * ou a variavel existe mas nao eh vetor
				 * ou eh vetor mas nao foi alocado
				 */
				erro=1;
				return;
			}
			else{
				expressao(delc());
				strcpy(word,delc()); //recebe FROM
			}		
		}
		else {
			if (p->eh_vetor){
				erro=1;
				return;
			}
		}
	}
		
	/* DEPOIS DO FROM */
	expressao(delc());
		
	/* SE DEU TUDO CERTO, DEPOIS DO TO */
	if (!erro) expressao(delc());
	else return;
	
	/* SE DEU TUDO CERTO, DEPOIS DO DO */
	if (!erro){
		while (strcmp(word,"]")){
			//printf("%s\n",word);
			if (!strcmp(word,"declare")) declarar(0);
			else if (!strcmp(word,"put")) atribuicao();
			else if (!strcmp(word,"resize")) alocacao();
			else if (!strcmp(word,"if")) desvio_if();
			else if (!strcmp(word,"else")) desvio_else();
			else if (!strcmp(word,"for")) loop_for();
			else if (!strcmp(word,"foreach")) loop_foreach();
			else if (!strcmp(word,"print")) escrita();
			else if (!strcmp(word,"read")) leitura();
			if (erro) return;
			strcpy(word,delc());
		}
		delete(cont);
	}
	else return;
}

void loop_foreach(){
	char* word = (char*)malloc(sizeof(char));
	strcpy(word,delc()); //recebe id, number ou posicao de vetor
	//verificar se id foi declarado
	if (!searchToken(word)){//se nao
		erro=1;
		return;
	}
	else {
		// verificar se é vetor
		Lista *p=searchNoToken(word);
		strcpy(word,delc());
		if (!strcmp(word,"[")){
			//verificar se id é vetor
			if ( (!p->eh_vetor) && (p->eh_vetor && !p->index) ){ 
				/* caso nao exista a variavel 
				 * ou a variavel existe mas nao eh vetor
				 * ou eh vetor mas nao foi alocado
				 */
				erro=1;
				return;
			}
			else expressao(delc());
		}
		else {
			if (p->eh_vetor){
				erro=1;
				return;
			}
		}
	}
	
		
	/* DEPOIS DO IN */
	strcpy(word,delc()); //recebe id tipo vetor
	
	//verificar se id foi declarado
	Lista *p=searchNoToken(word);
	//verificar se id é vetor
	if ( !p || (!p->eh_vetor) || (p->eh_vetor && !p->index) ){ 
		/* caso nao exista a variavel 
		 * ou a variavel existe mas nao eh vetor
		 * ou eh vetor mas nao foi alocado
		*/
		erro=1;
		return;
	}
	
	strcpy(word,delc()); //recebe DO
				
	/* DEPOIS DO DO */
	strcpy(word,delc()); //recebe ABRE_COLCHETES
	while (strcmp(word,"]")){
		if (!strcmp(word,"declare")) declarar(0);
		else if (!strcmp(word,"put")) atribuicao();
		else if (!strcmp(word,"resize")) alocacao();
		else if (!strcmp(word,"if")) desvio_if();
		else if (!strcmp(word,"else")) desvio_else();
		else if (!strcmp(word,"for")) loop_for();
		else if (!strcmp(word,"foreach")) loop_foreach();
		else if (!strcmp(word,"print")) escrita();
		else if (!strcmp(word,"read")) leitura();
		if (erro) return;
		strcpy(word,delc());
	}
	delete(cont);
}


/* 					FUNÇÕES DA ANÁLISE LÉXICA 
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
	for (j=18; j<39; j++){
		if (strcmp(tokenLower,rules[j])==0){
			addq(tokenLower);
			addc(tokenLower);
			return i;
		}
	}
	//SE NÃO FOR, É UMA VARIAVEL
	addq("id");
	addc(tokenLower);
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
	else {
		addq("int");
		addc(token);
	}
	return i;
}

int treatseparador(char *key) {
	int i=1;
	erro=0;
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
	addc(token);
	return i;
}

int treatString(char *key) {
	int i =1, qntd = 0, erro=0;
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
	else {
		addq("string");
		addc(token);
	}
	return i;
}

int treatChar(char *key) {
	int i=1, qntd = 0, erro=0;
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
	else {
		addq("char");
		addc(token);
	}
	return i;
}

int main(int argc, char **argv) {
	
	/* ABRIR CÓDIGO DO USUÁRIO */
    FILE *arquivo = fopen(argv[1], "r");
    char c;
    int i=0, j;
    struct stat estado;
	stat(argv[1], &estado);
	int tam = estado.st_size+1;
	char Key[tam];
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
	init_queue(); //INICIANDO FILAS
	
	while (i < (tam-1)) {
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
		free(token);
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
				validSintatico=1;
				break;
			}
		}
	}
	if (!validSintatico && !validLexico) { //INICIO DA ANALISE SEMANTICA
		char* word = (char*)malloc(sizeof(char));
		last = (char*)malloc(sizeof(char));
		//displayc();
		free(front);
		free(rear);
		while (frontCopy){
			strcpy(word,delc());
			//printf("%s\n",word);
			if (!strcmp(word,"declare")) declarar(1);
			else if (!strcmp(word,"put")) atribuicao();
			else if (!strcmp(word,"resize")) alocacao();
			else if (!strcmp(word,"if")) desvio_if();
			else if (!strcmp(word,"else")) desvio_else();
			else if (!strcmp(word,"for")) loop_for();
			else if (!strcmp(word,"foreach")) loop_foreach();
			else if (!strcmp(word,"print")) escrita();
			else if (!strcmp(word,"read")) leitura();
			if (erro){
				printf("NAO\n");
				return 0;
			}
			
		}
		printf("SIM\n");
	}
	return 0;
}
