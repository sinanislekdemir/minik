#include "cmd_serial.hpp"
#include "helpers.hpp"
#include <Arduino.h>

extern error err;
bool _breaker(char c) { return c == '\n'; }
bool _ignore(char c) { return (c < 32); }

char *_serial_getline(unsigned int buffer_size) {
  char *result = (char *)malloc(buffer_size);
  memset(result, 0, buffer_size);

  char c = 0;
  unsigned int cursor = 0;
  while (strlen(result) < buffer_size) {
    if (!Serial.available()) {
      continue;
    }
    c = Serial.read();
    if (_breaker(c)) {
      break;
    }
    if (_ignore(c)) {
      continue;
    }
    Serial.print(c);
    result[cursor++] = c;
  }

  Serial.println("");
  return result;
}

int command_serial_println(command *c, program *_p) {
  if (c->args[0].type == TYPE_STR) {
    Serial.println(c->args[0].data);
  }

  if (c->args[0].type == TYPE_NUM) {
    Serial.println(ctod(c->args[0].data));
  }

  if (c->args[0].type == TYPE_VARIABLE) {
    variable *v = find_variable(c->args[0].data, c->pid);
#ifdef ENABLE_EXCEPTIONS
    if (v == NULL) {
      char *msg = (char *)malloc(64);
      memset(msg, 0, 64);
      sprintf(msg, "Variable not found [%s]", c->args[0].data);
      c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
      free(msg);
      return -1;
    }
#endif

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

int command_getln(command *c, program *_p) { return 0; }
