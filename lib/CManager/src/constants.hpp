#ifndef _const_h
#define _const_h

#define STATEMENT_COUNT 23

#define VARIABLE_CAPACITY_STEP 8

#define TYPE_NONE 0
#define TYPE_STR 1
#define TYPE_NUM 2
#define TYPE_CONSTANT 3
#define TYPE_VARIABLE 4
#define TYPE_LABEL 5
#define TYPE_REGISTER 6
#define TYPE_FILE 7

#define CONSTANT_COUNT 5
#define LOG_SERIAL 0
#define LOG_FILE 1

#define CMP_N 0
#define CMP_JE 1
#define CMP_JG 2
#define CMP_JL 3

#define MAX_LINE_LENGTH 128
#define HISTORY_LIMIT 16
#define PROGRAM_END 0
#define RUNNING 1
#define SUB_END "---"
#define MAIN "main"

#define ERR_INVALID_COMMAND 100
#define ERR_NOT_ENOUGH_ARGUMENTS 101
#define ERR_INVALID_PARAMETER_TYPE 102
#define ERR_COMMAND_NOT_UNDERSTOOD 200
#define ERR_VARIABLE_NOT_FOUND 404

#endif
