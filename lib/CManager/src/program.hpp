#ifndef _program_hpp
#define _program_hpp

#ifdef BOARD_ESP32
#define MAX_SUB_COMMANDS 128
#define MAX_SUBS 32
#define PROG_SUBS 32
#define MAX_PROGS 16
#else
#define MAX_SUB_COMMANDS 36
#define MAX_SUBS 8
#define PROG_SUBS 8
#define MAX_PROGS 4
#endif

#define XSTR(x) STR(x)
#define STR(x) #x

#include "command.hpp"
#include "constants.hpp"
#include <Arduino.h>

struct sub {
	int back_history[16];
	int commands[MAX_SUB_COMMANDS];
	char name[16];
	unsigned int cursor;
	int command_count;
	long pid;
};

struct interrupt {
	int pin;
	int state;
	bool triggered;
	int routine;
};

class program {
      private:
	bool _serial_input;
	char valid_sub_count;

	int _compile_cursor;

	int parse(const char *cmd, unsigned int pid, int index);

      public:
	char _cmp_flag;
	int back_sub_history[PROG_SUBS];
	int cursor;
	int subs[PROG_SUBS];
	interrupt interrupts[8];

	bool _sleep;
	unsigned long _sleep_start;
	unsigned long _sleep_duration;
	unsigned int core;

	int find_sub(char *name);
	void append_to_history(unsigned int cursor, unsigned int instruction);
	void register_interrupt(int pin, int state, char routine);
	int check_interrupts();
	int pop_sub();

	long pid;
	char status_code;
	unsigned int priority;
	unsigned long start_time;
	unsigned long end_time;

	int compile(const char *line);
	void set_pid(long pid);
	void sdump();
	program();
	~program();
	int step();
	void destroy();
	void set_cmp_flag(unsigned int flag);
};

#endif
