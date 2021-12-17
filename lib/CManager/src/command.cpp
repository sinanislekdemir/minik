#include "command.hpp"
#include "constants.hpp"
#include <Arduino.h>

variable *get_var(command *c, int index) {
  if (c->args[index].type == TYPE_NUM || c->args[index].type == TYPE_STR) {
    return &c->args[index];
  }

  if (c->args[index].type == TYPE_VARIABLE) {
    return find_variable(c->args[index].data, c->pid);
  }
  return NULL;
}

int validate_command(command *c, const char *cmd, int expected_argc) {
#ifdef ENABLE_EXCEPTIONS
  if (c->argc < expected_argc) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Not enough params CMD: [%s] expected [%d] received [%d]",
            c->cmd, c->argc, expected_argc);
    c->exception = raise(msg, c->pid, ERR_NOT_ENOUGH_ARGUMENTS);
    free(msg);
    return -1;
  }
#endif
  return 0;
}
