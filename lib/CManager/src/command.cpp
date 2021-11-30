#include "command.hpp"
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
