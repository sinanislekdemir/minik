#include "interpreter.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "helpers.hpp"
#include "program.hpp"
#include "statement.hpp"

extern statement statements[STATEMENT_COUNT];
extern error err;

int run(command *c, program *_p) {
  for (unsigned int i = 0; i < STATEMENT_COUNT; i++) {
    if (strcmp(c->command, statements[i].command) == 0) {
      return statements[i].f(c, _p);
    }
  }

  err.code = ERR_COMMAND_NOT_UNDERSTOOD;
  err.pid = c->pid;
  return -1;
}
