#include "cmd_serial.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "helpers.hpp"
#include <Arduino.h>

extern error err;

int command_serial_println(command *c, program *_p) {
  if (c->args[0].type == TYPE_STR) {
    Serial.println(c->args[0].data);
  }

  if (c->args[0].type == TYPE_NUM) {
    Serial.println(ctod(c->args[0].data));
  }

  if (c->args[0].type == TYPE_VARIABLE) {
    variable *v = find_variable(c->args[0].data, c->pid);
    if (v == NULL) {
      return -1;
    }

    if (v->type == TYPE_NUM) {
      Serial.println(ctod(v->data));
    }

    if (v->type == TYPE_STR) {
      Serial.println(v->data);
    }
  }
  return 0;
}

int command_serial_print(command *c, program *_p) {
  if (c->args[0].type == TYPE_STR) {
    Serial.print(c->args[0].data);
  }
  if (c->args[0].type == TYPE_NUM) {
    Serial.print(ctod(c->args[0].data));
  }
  return 0;
}
