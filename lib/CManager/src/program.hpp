#ifndef _program_hpp
#define _program_hpp

#include "command.hpp"
#include "constants.hpp"
#include "error.hpp"
#include <Arduino.h>

struct sub {
  int instruction_count;
  /**
     Back history is used for this:

     main:
     set text "hi"
     call print
     set text "whats up"
     call print
     --
     print:
     sprintln text
     --

     when jumping from first call to print sub, jump instruction index is
     recorded in the back history, so when print sub ends, execution returns
     back to the last line in history. then the jump gets deleted in history.

     this lets 16 recurses then it's poof.
   */
  int back_history[HISTORY_LIMIT];
  command *instructions;
  char *name;
  uint cursor;
  uint exit;
};

class program {
public:
  int back_history[HISTORY_LIMIT];
  uint sub_cursor;
  error *err;
  sub *subs;
  int _line_count;
  int _sub_count;
  int _sourcecode_cursor;
  uint _cmp_flag;
  int sub_count();
  int sub_instruction_count(int index);
  int line_count();
  int compile_sub(int index);
  int previous_sub();
  int previous_instruction(int index);
  int find_sub(char *name);
  int append_to_history(int index, int jump_line);

  char *source;
  uint pid;
  uint exit_code;
  int compile();
  program();
  ~program();
  int step();
  void destroy();
  void set_cmp_flag(uint flag);
};

#endif
