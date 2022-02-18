#ifndef _program_hpp
#define _program_hpp

#ifdef BOARD_ESP32
#define MAX_SUB_COMMANDS 128
#define MAX_SUBS 64
#define PROG_SUBS 16
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
	short back_history[16];
	short commands[MAX_SUB_COMMANDS];
	char name[24];
	unsigned short cursor;
	short command_count;
	byte exit;
	byte pid;
};

struct interrupt {
	byte pin;
	byte state;
	bool triggered;
	short routine;
};

class program {
      private:
	bool _serial_input;
	char valid_sub_count;

	short _compile_cursor;

	int parse(const char *cmd, unsigned int pid, int index);

      public:
	char _cmp_flag;
	short back_sub_history[PROG_SUBS];
	short cursor;
	short subs[PROG_SUBS];
	interrupt interrupts[8];

	bool _sleep;
	unsigned long _sleep_start;
	unsigned long _sleep_duration;

	short find_sub(char *name);
	void append_to_history(unsigned short cursor, unsigned short instruction);
	void register_interrupt(char pin, unsigned int state, char routine);
	int check_interrupts();
	short pop_sub();

	char pid;
	char exit_code;
	unsigned int priority;
	unsigned long start_time;
	unsigned long end_time;
	bool compiling;

	int compile(const char *line);
	void set_pid(char pid);
	void sdump();
	program();
	~program();
	int step();
	void destroy();
	void set_cmp_flag(unsigned int flag);
};

#endif
