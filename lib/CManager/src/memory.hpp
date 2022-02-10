#ifndef _memory_hpp
#define _memory_hpp

#include "constants.hpp"
#include "variable.hpp"

#ifdef BOARD_ESP32
#define MAX_STR 100
#else
#define MAX_STR 10
#endif

#ifdef BOARD_ESP32
#define MAX_NUM 1000
#else
#define MAX_NUM 50
#endif

#if defined(BOARD_ATMEGA)
#define BITS 8
#elif defined(BOARD_ESP32)
#define BITS 32
#else
#define BITS 8
#endif

// jump request
struct jump_request {
	int request;
	char pid;
	short label;
};

struct dvariable {
	double val;
	char pid;
	bool free;
};

struct svariable {
	char data[MAX_LINE_LENGTH];
	char pid;
	bool free;
};
void free_variable(short index, char pid, char type);
void free_program(char pid);
void new_variable(svariable v);
void new_variable(dvariable v);
int init_mem();

double get_d(short index);
void get_s(short index, char *back);

int new_number(double value, char pid);
int new_string(char *value, char pid);

void update_number(short index, double value);
void update_string(short index, char *value);

void error_msg(const char *msg, char pid);
void mem_dump();

#endif
