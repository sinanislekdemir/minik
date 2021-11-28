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
  statements[0].command = (char *)"SPRINT";

  statements[1].f = &command_serial_println;
  statements[1].command = (char *)"SPRINTLN";

  statements[2].f = &command_set;
  statements[2].command = (char *)"SET";

  statements[3].f = &command_add;
  statements[3].command = (char *)"ADD";

  statements[4].f = &command_sub;
  statements[4].command = (char *)"SUB";

  statements[5].f = &command_div;
  statements[5].command = (char *)"DIV";

  statements[6].f = &command_mul;
  statements[6].command = (char *)"MUL";

  statements[7].f = &command_xor;
  statements[7].command = (char *)"XOR";

  statements[8].f = &command_or;
  statements[8].command = (char *)"OR";

  statements[9].f = &command_and;
  statements[9].command = (char *)"AND";

  statements[10].f = &command_pow;
  statements[10].command = (char *)"POW";

  statements[11].f = &command_log;
  statements[11].command = (char *)"LOG";

  statements[12].f = &command_je;
  statements[12].command = (char *)"JE";

  statements[13].f = &command_jne;
  statements[13].command = (char *)"JNE";

  statements[14].f = &command_jg;
  statements[14].command = (char *)"JG";

  statements[15].f = &command_jge;
  statements[15].command = (char *)"JGE";

  statements[16].f = &command_jl;
  statements[16].command = (char *)"JL";

  statements[17].f = &command_jle;
  statements[17].command = (char *)"JLE";

  statements[18].f = &command_cmp;
  statements[18].command = (char *)"CMP";

  statements[19].f = &command_goto;
  statements[19].command = (char *)"GOTO";

  statements[20].f = &command_call;
  statements[20].command = (char *)"CALL";

  statements[21].f = &command_halt;
  statements[21].command = (char *)"HALT";

  statements[22].f = &command_cpy;
  statements[22].command = (char *)"CPY";
}
