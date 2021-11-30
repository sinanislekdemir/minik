#ifndef _program_hpp
#define _program_hpp

#include "command.hpp"
#include "constants.hpp"
#include "error.hpp"
#include <Arduino.h>

struct command_history {
  command *item;
  command_history *next;
};

struct sub {
  command_history back_history;
  command *root_instruction;
  char *name;
  command *cursor;
  unsigned int exit;
  unsigned int index;
  sub *next;
};

struct sub_history {
  sub *item;
  sub_history *next;
};

class program {
public:
  sub_history back_history;
  sub *cursor;
  sub *main;
  int _line_count;
  unsigned int _cmp_flag;

  int line_count();
  sub *compile_next_sub();
  sub *previous_sub();
  command *previous_instruction();
  sub *find_sub(char *name);
  void append_to_history(sub *cursor, command *instruction);

  char *source;
  unsigned int _sourcecode_cursor;
  unsigned int pid;
  unsigned int exit_code;
  int compile();
  program(int pid);
  ~program();
  int step();
  void destroy();
  void set_cmp_flag(unsigned int flag);
};

#endif