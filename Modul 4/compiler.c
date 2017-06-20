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
int get_token(void);
void program(void);
void statement(void);
void expression(void);
void factor(void);
void term(void);
int countBegin = 0;
char tmp;



int main(int argc, char *argv[])
{
	
	char d = 0;
	init_token("file.txt");
	program();
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
	printf("%s %3d", token.charvalue, token.attr);	
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
	
	statement();
	if (!isend && countBegin>0){
		if (countBegin >= 2 && token.attr != 4){
			printf("Error, ';' tidak terdeklarasi");
			exit(-1);
		}
		printf("Error : 'end' or ';' is expected\n");
		exit(-1);
	}
	
	get_token();
	printf("%s %3d \n",token.charvalue,token.attr);
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


void statement (void)
{
	
	if (isbegin){
		countBegin++;
		get_token();
		
		
		statement();
		while (issemicolon){
			get_token();
			printf("%s %3d\n", token.charvalue,token.attr);
			
			statement();
			
			if (isend){
				return;
			}
			get_token();
			printf("%s %3d\n", token.charvalue,token.attr);
		
		}
		
		
	} 
	else{
		expression();
	}
//	else if (token.attr==2){
//		if (countBegin == 0){
//			get_token();
//			if (!isOperatorNumbr){
//				printf("Error, operations is expected");
//				exit(-1);
//			}
//			get_token();
//			if (token.attr != 2){
//				printf("Error, number is expected");
//				exit(-1);
//			}
//				
//			return;
//		}
//		
//		get_token();
//		
//		
//		if (!isOperatorNumbr){
//			printf("Error, operations is expected");
//			exit(-1);
//		}
//		
//	
//		get_token();
//		
//		
//		if (token.attr != 2){
//			printf("Error, number is expected");
//			exit(-1);
//		}	
//	} else{
//		printf("Error: Number tidak terdefinisi");
//		exit(-1);
//	}
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
	} else if (islparen) {
		get_token();
		expression();
		if (isrparen) {
			get_token();
			return;
		} else error();
	} else error();
}

void error(void){
	printf("Error");
	exit(0);
}
