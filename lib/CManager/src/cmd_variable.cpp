#include "cmd_variable.hpp"

#include "cmd_log.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "helpers.hpp"
#include "memory.hpp"
#include "variable.hpp"
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
    variable *v = new variable;
    v->next = NULL;

    int size = strlen(c->args[0].data) + 1;

    v->name = (char *)malloc(size);
    memset(v->name, 0, size);
    strcpy(v->name, c->args[0].data);

    v->datasize = c->args[1].datasize;
    v->deleted = false;
    v->data = (char *)malloc(c->args[1].datasize);

    memcpy(v->data, c->args[1].data, c->args[1].datasize);

    v->pid = c->pid;
    v->type = c->args[1].type;
    new_variable(v);
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

int command_cpy(command *c, program *_p) {
  // cpy dest 2 6 source
  if (validate_command(c, (const char *)"cpy", 4) != 0) {
    return -1;
  }

  if (c->args[0].type != TYPE_VARIABLE || c->args[3].type != TYPE_VARIABLE) {
    // TODO: Raise something here
    return -1;
  }

  variable *source = find_variable(c->args[3].data, c->pid);
  if (source == NULL) {
    return -1;
  }

  variable *dest = find_variable(c->args[0].data, c->pid);
  if (dest == NULL) {
    dest = new variable;
    dest->next = NULL;
    dest->type = TYPE_STR;
    dest->name = (char *)malloc(strlen(c->args[0].data) + 1);
    memset(dest->name, 0, strlen(c->args[0].data) + 1);
    strcpy(dest->name, c->args[0].data);
    new_variable(dest);
  }

  variable *from = get_var(c, 1);
  variable *size = get_var(c, 2);
  if (from->type != TYPE_NUM || size->type != TYPE_NUM) {
    return -1;
  }

  int from_int = int(ctod(from->data));
  int size_int = int(ctod(size->data));

  if (dest->data != NULL) {
    free(dest->data);
  }
  dest->datasize = (size_int + 1);
  dest->data = (char *)malloc(size_int + 1);
  memset(dest->data, 0, size_int + 1);
  memcpy(dest->data, source->data + from_int, size_int);
  return 0;
}
