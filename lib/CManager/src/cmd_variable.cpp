#include "cmd_variable.hpp"

#include "variable.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "cmd_log.hpp"
#include "memory.hpp"
#include <Arduino.h>

extern error err;

int command_set(command *c, program *_p) {
  if (validate_command(c, (const char *)"set", 2) != 0) {
    return -1;
  }

  if (c->args[0].type != TYPE_VARIABLE) {
    err.code = ERR_INVALID_PARAMETER_TYPE;
    err.pid = c->pid;
    return -1;
  }

  variable *v_check = find_variable(c->args[0].data, c->pid);
  if (v_check == NULL) {
    variable v;
    int size = strlen(c->args[0].data) + 1;

    v.name = (char *)malloc(size);
    memset(v.name, 0, size);
    strcpy(v.name, c->args[0].data);

    v.datasize = c->args[1].datasize;
    v.deleted = false;
    v.data = (char *)malloc(c->args[1].datasize);

    memcpy(v.data, c->args[1].data, c->args[1].datasize);

    v.pid = c->pid;
    v.type = c->args[1].type;
    new_variable(&v);
  } else {
    free(v_check->data);
    v_check->datasize = c->args[1].datasize;
    v_check->deleted = false;
    v_check->type = c->args[1].type;
    v_check->data = (char *)malloc(c->args[1].datasize);
    memcpy(v_check->data, c->args[1].data, c->args[1].datasize);
  }
  return 0;
}
