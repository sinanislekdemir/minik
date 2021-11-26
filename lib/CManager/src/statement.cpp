
#include "statement.hpp"
#include "cmd_log.hpp"
#include "cmd_logic.hpp"
#include "cmd_math.hpp"
#include "cmd_navigate.hpp"
#include "cmd_serial.hpp"
#include "cmd_variable.hpp"
#include "constants.hpp"
#include "helpers.hpp"

statement statements[STATEMENT_COUNT];

void register_statements() {
  statements[0].f = &command_serial_print;
  statements[0].command = (char *)"sprint";

  statements[1].f = &command_serial_println;
  statements[1].command = (char *)"sprintln";

  statements[2].f = &command_set;
  statements[2].command = (char *)"set";

  statements[3].f = &command_add;
  statements[3].command = (char *)"add";

  statements[4].f = &command_sub;
  statements[4].command = (char *)"sub";

  statements[5].f = &command_div;
  statements[5].command = (char *)"div";

  statements[6].f = &command_mul;
  statements[6].command = (char *)"mul";

  statements[7].f = &command_xor;
  statements[7].command = (char *)"xor";

  statements[8].f = &command_or;
  statements[8].command = (char *)"or";

  statements[9].f = &command_and;
  statements[9].command = (char *)"and";

  statements[10].f = &command_pow;
  statements[10].command = (char *)"pow";

  statements[11].f = &command_log;
  statements[11].command = (char *)"log";

  statements[12].f = &command_je;
  statements[12].command = (char *)"je";

  statements[13].f = &command_jne;
  statements[13].command = (char *)"jne";

  statements[14].f = &command_jg;
  statements[14].command = (char *)"jg";

  statements[15].f = &command_jge;
  statements[15].command = (char *)"jge";

  statements[16].f = &command_jl;
  statements[16].command = (char *)"jl";

  statements[17].f = &command_jle;
  statements[17].command = (char *)"jle";

  statements[18].f = &command_cmp;
  statements[18].command = (char *)"cmp";

  statements[19].f = &command_goto;
  statements[19].command = (char *)"goto";

  statements[20].f = &command_call;
  statements[20].command = (char *)"call";

  statements[21].f = &command_halt;
  statements[21].command = (char *)"halt";
}
