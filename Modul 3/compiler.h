/*
	File Name : compiler.h
	Vers : 0.01a
	Written by : Setiadi Rachmat
	Date : Fri Aug 28 10:08:03 WIT 2009
*/

#define isprogram			(token.attr == RWORD && token.value == PROGRAM)
#define isbegin				(token.attr == RWORD && token.value == BEGIN)
#define isend				(token.attr == RWORD && token.value == END)

#define isidentifier		(token.attr = IDENTIFIER)

#define isnumbr				(token.attr = NUMBER)
#define islast				(token.attr == NUMBER || token.attr == IDENTIFIER || token.attr == SYMBOL || token.attr == RWORD)

#define issemicolon			(token.attr == SYMBOL && token.value == SEMICOLON)
#define isperiod			(token.attr == SYMBOL && token.value == PERIOD)

#define isplus 				(token.attr == SYMBOL && token.value == PLUS)
#define isminus 			(token.attr == SYMBOL && token.value == MINUS)
#define istimes 			(token.attr == SYMBOL && token.value == TIMES)
#define isOperatorNumbr		(isplus || isminus || istimes)
#define islast				(isprogram || isbegin || isend || isidentifier || isnumbr || issemicolon || isperiod || isOperatorNumbr)
