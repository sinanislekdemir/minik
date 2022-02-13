#ifndef _memory_hpp
#define _memory_hpp

#include "constants.hpp"

#ifdef BOARD_ESP32
#define MAX_MEM 160 * 1024
#else
#define MAX_MEM 3 * 1024
#endif

#ifdef BOARD_ATMEGA
#define BITS 8
#elif BOARD_ESP32
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

struct _protected {
	short pid;
	unsigned int from;
	unsigned int to;
};

void free_area(unsigned int index, unsigned int size);
void free_program(char pid);
int write_area(unsigned int index, char *data);
int write_area(unsigned int index, char *data, unsigned int size);
int write_area(unsigned int index, int data);
int write_area(unsigned int index, double data);
int write_area(unsigned int index, long data);
int write_area(unsigned int index, char data);
int append_area(unsigned int index, char data);
int read_area_str(unsigned int index, unsigned int size, char *back);
double read_area_double(unsigned int index);
int read_area_int(unsigned int index);
long read_area_long(unsigned int index);
char read_area_char(unsigned int index);

void error_msg(const char *msg, char pid);

#endif
