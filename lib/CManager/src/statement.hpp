#ifndef _statement_hpp
#define _statement_hpp
#define STATEMENT_SPRINT "SPRINT"
#define STATEMENT_SPRINTLN "SPRINTLN"
#define STATEMENT_SREADLN "SREADLN"
#define STATEMENT_GOTO "GOTO"
#define STATEMENT_CALL "CALL"
#define STATEMENT_JE "JE"
#define STATEMENT_JNE "JNE"
#define STATEMENT_JG "JG"
#define STATEMENT_JGE "JGE"
#define STATEMENT_JL "JL"
#define STATEMENT_JLE "JLE"
#define STATEMENT_CMP "CMP"
#define STATEMENT_LOG "LOG"
#define STATEMENT_ADD "ADD"
#define STATEMENT_SUB "SUB"
#define STATEMENT_DIV "DIV"
#define STATEMENT_MUL "MUL"
#define STATEMENT_XOR "XOR"
#define STATEMENT_OR "OR"
#define STATEMENT_AND "AND"
#define STATEMENT_POW "POW"
#define STATEMENT_SIN "SIN"
#define STATEMENT_COS "COS"
#define STATEMENT_TAN "TAN"
#define STATEMENT_COT "COT"
#define STATEMENT_SINH "SINH"
#define STATEMENT_COSH "COSH"
#define STATEMENT_TANH "TANH"
#define STATEMENT_COTH "COTH"
#define STATEMENT_ASIN "ASIN"
#define STATEMENT_ACOS "ACOS"
#define STATEMENT_ATAN "ATAN"
#define STATEMENT_ACOT "ACOT"
#define STATEMENT_ASINH "ASINH"
#define STATEMENT_ACOSH "ACOSH"
#define STATEMENT_ATANH "ATANH"
#define STATEMENT_ACOTH "ACOTH"
#define STATEMENT_HALT "HALT"
#define STATEMENT_ALLOC "ALLOC"
#define STATEMENT_APPEND "APPEND"
#define STATEMENT_SET "SET"
#define STATEMENT_STR "STR"
#define STATEMENT_NUM "NUM"
#define STATEMENT_DEL "DEL"
#define STATEMENT_CPY "CPY"
#define STATEMENT_LROTATE "LROTATE"
#define STATEMENT_RROTATE "RROTATE"
#define STATEMENT_LSHIFT "LSHIFT"
#define STATEMENT_RSHIFT "RSHIFT"
#define STATEMENT_SLEEP "SLEEP"
#define STATEMENT_MILLIS "MILLIS"
#define STATEMENT_INT "INT"
#define STATEMENT_CORE "CORE"
#define STATEMENT_SYS "SYS"
#define STATEMENT_AREAD "AREAD"
#define STATEMENT_AWRITE "AWRITE"
#define STATEMENT_PINMODE "PINMODE"
#define STATEMENT_DWRITE "DWRITE"
#define STATEMENT_DREAD "DREAD"

#include "program.hpp"

typedef int (*statement_function)(command *c, program *_p);

struct statement {
	int (*f)(command *c, program *_p);
	const char *command;
	statement *next;
};

void register_statements();
const char *find_statement(const char * cmd);

#endif
