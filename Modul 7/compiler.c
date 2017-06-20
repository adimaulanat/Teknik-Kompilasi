/*
	File name 		: compiler.c
	Vers			: 0.01a
	Written by		: Setiadi Rachmat
	Date			: Fri Aug 28 10:08:03 WIT 2009
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "compiler.h"
#include "stack.h"



extern  token_t token;
FILE *infile;
ListStack varTable;


int get_token(void);
void program(void);
void outblock(void);
void statement(void);
void factor(void);
void inblock(void);
void condition(void);
void paramlist(void);
void error(void);
void term(void);

int countBegin = 0;
char tmp;



int main(int argc, char *argv[])
{
	CreateListStack(&varTable);
	char d = 0;
	init_token(argv[1]);
	program();
	//PrintInfoStack(varTable);
	printf("Congratulations : No error is found \n");
}

void program(void)
{
	get_token();
	if(!isprogram){
		printf("Error : Reserved Word 'program' is expected\n");
		exit(-1);
	}
	
	get_token();	
	if(token.attr != 1){
		printf("Error : Identifier is expected after 'program' \n");
		exit(-1);
	}
	
	get_token();
	if (!issemicolon){
		printf("Error : Symbol ';' is expected \n");
		exit(-1);
	}
	
	get_token();
	outblock();
	
	if (isend){
		get_token();
		printf("%s %3d \n",token.charvalue, token.attr);
	}

	if (!isperiod){
		printf("Error : Symbol '.' is expected\n");
		exit(-1);
	}
	
	get_token();
	if (token.attr != 0){
		printf("Error: After symbol '.' must EOF");
		exit(-1);
	}
}

void expression(void)
{
	if (isplus || isminus){
		get_token();
		
	} 
	term();
	while (isplus || isminus) {
		
		get_token();
		
		term();
	}
}

void term(void)
{
	factor();
	while (isOperatorNumber) {
		if (!isplus && !isminus){
			get_token();
			factor();
		} else{
			return;
		}
	}
}

void factor(void)
{
	
	if (token.attr == 2) {
		get_token();
		
		return;
	} else if (token.attr == 1){
		get_token();
		return;
	}else if (islparen) {
		get_token();
		expression();
		
		if (isrparen) {
			get_token();
			return;
		} else error();
	} else error();
}

void statement (void)
{
	
	if (token.attr == 1){
		addressStack resultSearch= NULL;
     	int nparam=0;
     	resultSearch=search(varTable,token.charvalue);  
     		if(resultSearch == NULL) {
     			printf("%s %3d",token.charvalue,token.attr);
        		printf("Fatal : Syntax Error, Indentifier '%s' is undefined\n", token.charvalue);
        		printf("Fatal: Compilation Aborted\n");
        		exit(-1);
    		 }
		
		get_token();
		
		if (isbecomes){
			get_token();
			if (token.attr == 1){
				addressStack resultSearch= NULL;
     			int nparam=0;
     			resultSearch=search(varTable,token.charvalue);  
     			if(resultSearch == NULL) {
     				printf("%s %3d",token.charvalue,token.attr);
        			printf("Fatal : Syntax Error, Indentifier '%s' is undefined\n", token.charvalue);
        			printf("Fatal: Compilation Aborted\n");
        			exit(-1);
    		 	}
			}
			
			expression();
			if (isend){
				get_token();
				
			}
			return;
		} else
		{
			get_token();
			paramlist();
		}
	}
	if (isbegin){
		countBegin++;
		get_token();
		if(token.attr == 2 || token.attr == 4){
			printf("Error: setelah 'begin' haruslah identifier atau RWORD");
			exit(-1);
		}
		statement();
		while (issemicolon){
			get_token();
			
			statement();
			if (isend){
				get_token();
				
			}
		}
		
	} 
	
	if (iswhile){
		
		get_token();
		condition();
		
		if(!isdo){
			printf("Error: 'do' tidak di deklarasi");
			exit(-1);
		}
		get_token();
		
		statement();
		
	}
	
	if (isif){
		get_token();
		condition();
		if (!isthen){
			printf("Error: 'then' is expected");
			exit(-1);
		}
		
		get_token();
		statement();
		
		get_token();
		if (iselse){
			get_token();
			statement();
		}
	}
	
	if (iswrite){
		get_token();
		if (!islparen){
			printf("Error: '(' not declarated");
			exit(-1);
		}
			get_token();
			expression();
			while (iscomma){
				get_token();
				expression();
			}
			if (!isrparen){
				printf("Error: ')' not declarated");
				exit(-1);
			}
		
	}
	
	if (isread){
		get_token();
		if (!islparen){
			printf("Error: '(' not declarated");
			exit(-1);
		}
		get_token();
		if (token.attr != 1){
			printf("Error: 'identifier' not declarated");
			exit(-1);
		}
		
		get_token();
		while (iscomma){
			get_token();
			printf("%s %3d\n",token.charvalue,token.attr);
			if (token.attr != 1){
				printf("Error: 'identifier' not declarated");
				exit(-1);
			}
			get_token();
		}
		
		if (!isrparen){
			printf("Error: ')' not declarated");
			exit(-1);
		}
		get_token();
	}
}


void paramlist(void){
	if (islparen){
		get_token();
		expression();
		while (iscomma){
			get_token();
			expression();
		}
		if (!isrparen){
			printf("Error: ')' is expected");
			exit(-1);
		}
	}
}

void condition (void){
	expression();
	return;
}


void outblock (void){
	infotype X;
	if (!isvar){
		printf("Error: 'var' is expected");
		exit(-1);
	}
	
	get_token();
	
	if (token.attr != 1){
		printf("Error: 'identifier' is expected");
		exit(-1);
	} else{
		strcpy(X.key, token.charvalue);
		X.type=VGLOBAL;
		X.address=0;
		X.nparam=0;
		Push(&varTable,&X);
	}
	
	
	get_token();
	while (iscomma){
		
		get_token();
		printf("%s %3d\n",token.charvalue,token.attr);
		if (token.attr != 1){
			printf("%s %3d\n",token.charvalue,token.attr);
			printf("Error: 'identifier' is expected");
			exit(-1);
		} else{
			strcpy(X.key, token.charvalue);
			X.type=VGLOBAL;
			X.address=0;
			X.nparam=0;
			Push(&varTable,&X);
		}
		
		
		get_token();
	}
	
	if (!issemicolon){
		printf("Error: ';' is expected");
		exit(-1);
	}
	get_token();
	while (isprocedure){
		
		get_token();
		
		if (token.attr != 1){
			printf("Error: 'identifier' is expected");
			exit(-1);
		} else{
			strcpy(X.key, token.charvalue);
			X.type=PNAME;
			X.address=0;
			X.nparam=0;
			Push(&varTable,&X);
		}
		
		get_token();
		inblock();
	}
	statement();
	
}

void inblock(void){
	infotype X;
	addressStack addrProcedure;
	addrProcedure = varTable.TOP;
	int nparam=0;
	if (islparen){
		get_token();
		if (token.attr != 1){
			
			printf("Error: 'identifier' is expected");
			exit(-1);
		}
		
		addressStack resultSearch= NULL;
     	resultSearch=search(varTable,token.charvalue);  
     		if(resultSearch == NULL) {
     			
        		printf("Fatal : Syntax Error, Indentifier '%s' is undefined\n", token.charvalue);
        		printf("Fatal: Compilation Aborted\n");
        		exit(-1);
    		 }
		strcpy(X.key, token.charvalue);
		X.type=VLOCAL;
		X.address=0;
		X.nparam=0;
		nparam++;
		printf("%d\n",nparam);
		Push(&varTable,&X);
		printf("%d\n",nparam);
		get_token();
		while (iscomma){
			get_token();
			if (token.attr != 1){
			
				printf("Error: 'identifier' is expected");
				exit(-1);
			} else{
				strcpy(X.key, token.charvalue);
				X.type=VLOCAL;
				X.address=0;
				X.nparam=0;
				nparam++;
				Push(&varTable,&X);	
			}
			
			get_token();
		}
		
		if(!isrparen){
			printf("Error: ')' is expected");
			exit(-1);
		}
		printf("%d\n",nparam);
		addrProcedure->info.nparam=nparam;
		printf("A%d\n",addrProcedure->info.nparam);
		
		get_token();
	}
	
	if (!issemicolon){
		printf("Error: ';' is expected");
		exit(-1);
	}
		get_token();
		
		if (isvar){
			
			get_token();
			if (token.attr != 1){
			
				printf("Error: 'identifier' is expected");
				exit(-1);
			} else{
				strcpy(X.key, token.charvalue);
				X.type=VLOCAL;
				X.address=0;
				X.nparam=0;
				Push(&varTable,&X);
			}
			get_token();
			while (iscomma){
				get_token();
				if (token.attr != 1){
			
					printf("Error: 'identifier' is expected");
					exit(-1);
				} else{
					strcpy(X.key, token.charvalue);
					X.type=VLOCAL;
					X.address=0;
					X.nparam=0;
					nparam++;
					Push(&varTable,&X);
				}
				get_token();
				
			}
			addrProcedure->info.nparam=nparam;
			PrintInfoStack(varTable);
			if (!issemicolon){
				printf("Error: ';' is expected");
				exit(-1);
			}
			get_token();
		} else if (!isvar && !isbegin){
			printf("Error: Begin atau var tidak terdeklarasi");
			exit(-1);
		}
		
		statement();
	
	PopVarLocal(&varTable);
}

void error(void){
	printf("%s %3d",token.charvalue, token.attr);
	printf("Error");
	exit(0);
}
