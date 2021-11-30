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
    return -1;
  }

  variable *v_check = find_variable(c->args[0].data, c->pid);
  variable *set = get_var(c, 1);

  if (v_check == NULL) {
    variable *v = (variable *)malloc(sizeof(variable));
    v->next = NULL;
    v->pid = _p->pid;

    int size = set->datasize;
    v->name = (char *)malloc(strlen(c->args[0].data));
    strcpy(v->name, c->args[0].data);

    v->datasize = set->datasize;
    v->deleted = false;
    v->data = (char *)malloc(set->datasize);

    memcpy(v->data, set->data, set->datasize);

    v->pid = c->pid;
    v->type = set->type;
    new_variable(v);
  } else {
    free(v_check->data);
    v_check->datasize = set->datasize;
    v_check->deleted = false;
    v_check->type = set->type;
    v_check->data = (char *)malloc(set->datasize);
    memcpy(v_check->data, set->data, set->datasize);
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
    dest = (variable *)malloc(sizeof(variable));
    dest->next = NULL;
    dest->data = NULL;
    dest->type = TYPE_STR;
    dest->pid = c->pid;
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
