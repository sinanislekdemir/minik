#include "cmd_variable.hpp"
#include "helpers.hpp"
#include <Arduino.h>

extern error err;

int command_set(command *c, program *_p) {
#ifdef ENABLE_EXCEPTIONS
  if (validate_command(c, (const char *)"set", 2) != 0) {
    return -1;
  }
#endif

#ifdef ENABLE_EXCEPTIONS
  if (c->args[0].type != TYPE_VARIABLE) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Invalid variable type, expected VARIABLE got [%s]",
            type_tostr(c->args[0].type));
    c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif

  variable *v_check = find_variable(c->args[0].data, c->pid);
  variable *set = get_var(c, 1);

  if (v_check == NULL) {
    variable *v = (variable *)malloc(sizeof(variable));
    v->next = NULL;
    v->pid = _p->pid;

    unsigned int size = strlen(c->args[0].data) + 1;
    v->name = (char *)malloc(size);
    memset(v->name, 0, size);
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
#ifdef ENABLE_EXCEPTIONS
  if (validate_command(c, (const char *)"cpy", 4) != 0) {
    return -1;
  }

  if (c->args[0].type != TYPE_VARIABLE || c->args[3].type != TYPE_VARIABLE) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "COPY arguments 1 or 3 are not variable. 1=[%s] 3=[%s]",
            type_tostr(c->args[0].type), type_tostr(c->args[1].type));
    c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
    free(msg);
    return -1;
  }
#endif

  variable *source = find_variable(c->args[3].data, c->pid);
#ifdef ENABLE_EXCEPTIONS
  if (source == NULL) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Source can not be NONE");
    c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif

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
#ifdef ENABLE_EXCEPTIONS
  if (from->type != TYPE_NUM || size->type != TYPE_NUM) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Copy range is not integer [%s]:[%s]", type_tostr(from->type),
            type_tostr(size->type));
    c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
    free(msg);
    return -1;
  }
#endif

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
