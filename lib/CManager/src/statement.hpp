#ifndef _statement_hpp
#define _statement_hpp
#define MAX_COMMAND_LENGTH 10
#define STATEMENT_SPRINT 1
#define STATEMENT_SPRINTLN 2
#define STATEMENT_SREADLN 3
#define STATEMENT_GOTO 4
#define STATEMENT_CALL 5
#define STATEMENT_JE 6
#define STATEMENT_JNE 7
#define STATEMENT_JG 8
#define STATEMENT_JGE 9
#define STATEMENT_JL 10
#define STATEMENT_JLE 11
#define STATEMENT_CMP 12
#define STATEMENT_LOG 13
#define STATEMENT_ADD 14
#define STATEMENT_SUB 15
#define STATEMENT_DIV 16
#define STATEMENT_MUL 17
#define STATEMENT_XOR 18
#define STATEMENT_OR 19
#define STATEMENT_AND 20
#define STATEMENT_POW 21
#define STATEMENT_SIN 22
#define STATEMENT_COS 23
#define STATEMENT_TAN 24
#define STATEMENT_COT 25
#define STATEMENT_SINH 26
#define STATEMENT_COSH 27
#define STATEMENT_TANH 28
#define STATEMENT_COTH 29
#define STATEMENT_ASIN 30
#define STATEMENT_ACOS 31
#define STATEMENT_ATAN 32
#define STATEMENT_ACOT 33
#define STATEMENT_ASINH 34
#define STATEMENT_ACOSH 35
#define STATEMENT_ATANH 36
#define STATEMENT_ACOTH 37
#define STATEMENT_HALT 38
#define STATEMENT_ALLOC 39
#define STATEMENT_APPEND 40
#define STATEMENT_SET 41
#define STATEMENT_STR 42
#define STATEMENT_NUM 43
#define STATEMENT_DEL 44
#define STATEMENT_CPY 45
#define STATEMENT_LROTATE 46
#define STATEMENT_RROTATE 47
#define STATEMENT_LSHIFT 48
#define STATEMENT_RSHIFT 49
#define STATEMENT_SLEEP 50
#define STATEMENT_MILLIS 51
#define STATEMENT_INT 52
#define STATEMENT_CORE 53
#define STATEMENT_SYS 54
#define STATEMENT_AREAD 55
#define STATEMENT_AWRITE 56
#define STATEMENT_PINMODE 57
#define STATEMENT_DWRITE 58
#define STATEMENT_DREAD 59

#include "program.hpp"

typedef int (*statement_function)(command *c, program *_p);

struct statement {
	int (*f)(command c, program *_p);
	char command;
};

void register_statements();
char find_statement(const char *cmd);

#endif
