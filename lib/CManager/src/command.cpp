#include "command.hpp"
#include <Arduino.h>

variable *get_var(command *c, int index) {
  if (c->args[index].type != TYPE_NUM && c->args[index].type != TYPE_VARIABLE) {
    return NULL;
  }
  if (c->args[index].type == TYPE_NUM) {
    return &c->args[index];
  } else {
    return find_variable(c->args[index].data, c->pid);
  }
  return NULL;
}
