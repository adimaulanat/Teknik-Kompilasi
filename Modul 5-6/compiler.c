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

extern  token_t token;
FILE *infile;
text tmp[50];

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
int countVar = 0;


int main(int argc, char *argv[])
{
	
	char d = 0;
	init_token(argv[1]);
	program();
	printf("Congratulations : No error is found \n");
	exit(-1);
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
	
	/* MASUK KE OUTBLOCK */
	outblock();
	if (isend){
		get_token();
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
	int count;
	if (token.attr == 1){
		count=0;
		while (countVar >= count){
				int resultSearch = strcmp(tmp[count].value , token.charvalue);
				if (resultSearch == 0){
					count = countVar + 1;
				}
				count++;
				if (count > countVar){
					if (resultSearch != 0){
						printf("Identifier '%s' not declarated",token.charvalue);
						exit(-1);
					}
				}				
		}
		// BIJIL PRIME
		get_token();
		
		if (isbecomes){
			get_token();
			if (token.attr == 1){
				count=0;
				while (countVar >= count){
					int resultSearch = strcmp(tmp[count].value , token.charvalue);
					if (resultSearch == 0){
						count = countVar + 1;
					}
					count++;
					if (count > countVar){
						if (resultSearch != 0){
							printf("Identifier '%s' not declarated",token.charvalue);
							exit(-1);
						}
					}				
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
	if (!isvar){
		printf("Error: 'var' is expected");
		exit(-1);
	}
	get_token();
	
	
	if (token.attr != 1){
		printf("Error: 'identifier' is expected");
		exit(-1);
	} else{
		strcpy(tmp[countVar].value , token.charvalue);
		countVar++;
	}
	
	get_token();
	
	while (iscomma){
		
		get_token();
		if (token.attr != 1){
			printf("Error: 'identifier' is expected");
			exit(-1);
		} else{
			strcpy(tmp[countVar].value , token.charvalue);
			countVar++;
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
			strcpy(tmp[countVar].value , token.charvalue);
			countVar++;
		}
		get_token();
		inblock();
	}
	statement();
	
}

void inblock(void){
	int count ;
	
	if (islparen){
		get_token();
		if (token.attr != 1){
			printf("Error: 'identifier' is expected");
			exit(-1);
		} else{
			count = 0;
			while (countVar >= count){
				int resultSearch = strcmp(tmp[count].value , token.charvalue);
				if (resultSearch == 0){
					count = countVar + 1;
				}
				count++;
				if (count > countVar){
					if (resultSearch != 0){
						printf("Identifier '%s' not declarated",token.charvalue);
						exit(-1);
					}
				}				
			}
		}
		get_token();
		while (iscomma){
			count = 0;
			get_token();
			if (token.attr != 1){
				printf("Error: 'identifier' is expected");
				exit(-1);
			} else{
				while (countVar >= count){
				int resultSearch = strcmp(tmp[count].value , token.charvalue);
				if (resultSearch == 0){
					count = countVar + 1;
				}
				count++;
				if (count > countVar){
					if (resultSearch != 0){
						printf("Identifier '%s' not declarated",token.charvalue);
						exit(-1);
					}
				}				
			}
			}
			get_token();
		}
		
		if(!isrparen){
			printf("Error: ')' is expected");
			exit(-1);
		}
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
				strcpy(tmp[countVar].value , token.charvalue);
				countVar++;
			}
			
			get_token();
			while (iscomma){
				get_token();
				if (!isidentifier){
					printf("Error: 'identifier' is expected");
					exit(-1);
				} else{
					count = 0;
					while (countVar >= count){
						int resultSearch = strcmp(tmp[count].value , token.charvalue);
						if (resultSearch == 0){
							count = countVar + 1;
						}
						count++;
						if (count > countVar){
							if (resultSearch != 0){
								printf("Identifier '%s' not declarated",token.charvalue);
								exit(-1);
							}
						}				
					}
				}
				get_token();
			}
			
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
	
}

void error(void){
	printf("Error");
	exit(0);
}
