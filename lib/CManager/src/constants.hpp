
#ifndef _constants_hpp
#define _constants_hpp

#define TYPE_NONE 0
#define TYPE_STR 1
#define TYPE_NUM 2
#define TYPE_CONSTANT 3
#define TYPE_ADDRESS 4
#define TYPE_LABEL 5
#define TYPE_REGISTER 6
#define TYPE_FILE 7
#define TYPE_BYTE 8

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
#define STATEMENT_NOOP 34
#define STATEMENT_HALT 38
#define STATEMENT_ALLOC 39
#define STATEMENT_APPEND 40
#define STATEMENT_SET 41
#define STATEMENT_AVAILABLE_1 42
#define STATEMENT_AVAILABLE_2 43
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

#define CONSTANT_COUNT 5
#define LOG_SERIAL 0
#define LOG_FILE 1

#define CMP_N 0
#define CMP_JE 1
#define CMP_JG 2
#define CMP_JL 3

#ifdef BOARD_ESP32
#define MAX_LINE_LENGTH 128
#else
#define MAX_LINE_LENGTH 80
#endif
#define PROGRAM_ERR -1
#define PROGRAM_FREE 0
#define PROGRAM_RUNNING 1
#define PROGRAM_HALT 2
#define PROGRAM_COMPILING 3

#define SUB_END "---"
#define MAIN "main"

#define ERR_INVALID_COMMAND 100
#define ERR_NOT_ENOUGH_ARGUMENTS 101
#define ERR_INVALID_PARAMETER_TYPE 102
#define ERR_COMMAND_NOT_UNDERSTOOD 200
#define ERR_VARIABLE_NOT_FOUND 404
#define ERR_ADDRESS_NOT_FOUND 405

#define ERR_STR_SOURCE_NONE "Source can not be NONE"
#define ERR_STR_VAR_NOT_FOUND "Variable not found"
#define ERR_STR_NOT_ENOUGH_PARAMS "Not enough params"
#define ERR_STR_INVALID_TYPE "Invalid variable type"
#define ERR_STR_VARIABLE_FULL "Variable is full"
#define ERR_STR_DIVISION_BY_ZERO "Division by zero"
#define ERR_STR_ADDRESS_NOT_FOUND "Sub address not found or not valid"

#define CONST_COUNT 13

struct constant {
        char keyword[16];
        double val;
};

#endif
