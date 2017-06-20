/*
	File name 		: scanner.c
	Vers			: 0.01a
	Written by		: Setiadi Rachmat
	Date			: Fri Aug 28 10:08:03 WIT 2009
	
	Modified by		: Adi Maulana Triadi
	Date			: Friday March 03 13:38:50 WIB 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

FILE *infile;
token_t token;
int get_token(void);
char tempRword[16][11] = {"begin", "div", "do", "else", "end", "if", "procedure", "program", "then", "var", "while", "read", "write", "forward", "function"};
char symbolType[20][3] = {"+","-","*","(",")","=",",",".",";",":=","<","<=","<>",">",">=","[","]","..",":"};

/* initialisasi token */

void init_token(char *name){
	if ((infile = fopen("file.txt","r")) == NULL){
		printf("Error : Can't open source code %s \n", name);
		exit(-1);
	} else {
		return;
	}
}

int get_token(void){
	int i = 0;
	int a = 0;
	static int tmpValue;
	char ch;
	token.attr = 0;
	memset(token.charvalue, '\0', sizeof(token.charvalue));
	ch = fgetc(infile);
	
	if(ch == '*' || ch == ';' || ch == '.' || ch == '+' || ch == '-' || ch == ',' || ch == '<' || ch == '>' || ch == '=' || ch == ':' || ch == '(' || ch == ')'){
		token.charvalue[0] = ch;
		if (ch == '<' || ch == ':' || ch == '.' || ch == '>'){
			ch = fgetc(infile);
			if (ch == '=' || ch == '.' || ch == '>'){
			token.charvalue[1] = ch;
			}
			else{
				ungetc(ch,infile);
			}
		}
	} else if( ch == ' ' || ch == '\n' || ch == '\t'){
		get_token();
	} 

	 
	else {
		do {
			if( ch == ' ' || ch == '\n' || ch == '\t'){
				return;
			} else if( ch == EOF){
				return EOF;
			} else if (ch != '.' && ch != '>' && ch != ':' && ch != '<' && ch != '=' && ch != ')' && ch != '(' && ch != ',' && ch != '*' && ch != ';' && ch != '.' && ch != '+' && ch != '-'){
				token.charvalue[i] = tolower(ch);
			} 
			ch = fgetc(infile);
			if (ch == EOF){
				return;
			}
			i++;
		}
		while(ch != '\t' && ch != '\n' && ch != ' ' && ch != '.' && ch != '>' && ch != ':' && ch != '<' && ch != '=' && ch != ')' && ch != '(' && ch != ',' && ch != '*' && ch != ';' && ch != '.' && ch != '+' && ch != '-');	
		fseek(infile, -1,SEEK_CUR);
	}
	
	
	if(ch == '*' || ch == ';' || ch == '.' || ch == '+' || ch == '-' || ch == ',' || ch == '<' || ch == '>' || ch == '=' || ch == ':' || ch == '(' || ch == ')'){
		if(token.charvalue[0] == ch){
			token.attr = SYMBOL;
			int j = 0;
			while (j <= 19){
				int tmp = strcmp(symbolType[j], token.charvalue);
				if(tmp == 0){
					token.value = j;
					tmpValue = token.value;
				}
			j++;			
			}	
		}

	}else if((int) token.charvalue[0] >= (int)'0' && (int) token.charvalue[0] <= (int)'9'){
					token.attr = NUMBER;
					a = token.charvalue - '0'; // mengubah char ke int
					token.value = a;
	} 
	else{
		int c = 0;
		while(c < 16)
		{
			if(strcmp(tempRword[c],token.charvalue) == 0){
				token.value = c;	
				token.attr = RWORD;
				tmpValue = token.value;
			}
		c++;
		}					
		if(token.attr != RWORD && token.attr != SYMBOL){
			token.attr = IDENTIFIER;
		}		
		if(token.attr == IDENTIFIER){
			token.value = tmpValue;
		}
		
	}
		
}
