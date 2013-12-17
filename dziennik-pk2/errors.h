/*
   ╔════════════════════════════════╗
   ║		 File errors.h			║
   ╚════════════════════════════════╝
*/


#ifndef _errors_H_
#define _errors_H_

// ##### Constants Declarations #####

#define ERR_UNKNOWN			0
#define ERR_TOOMUCH			1
#define ERR_TOOLESS			2
#define ERR_WRONGTYPE		3
#define ERR_NOSUBJECTS		4
#define ERR_UNK_OPT			5
#define ERR_NOATTACHEDSUBS	6
#define ERR_NOGRADES		7
#define ERR_SUBEXISTS		8
#define ERR_NOSTUDENTS		9
#define ERR_ILLEGALCHRS	   10
#define ERR_NOBASEREC	   11
#define ERR_NOBASESUB	   12
#define ERR_REALLOC		   13
#define ERR_NOTFOUND	   14
#define ERR_EMPTYLIST	   15
#define ERR_SAVINGSUB	   16
#define ERR_SAVINGREC	   17

// ##### Handling errors function declaration #####

void Catch_error(int);

#endif
