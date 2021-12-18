#include "cmd_time.hpp"
#include "helpers.hpp"

int command_sleep(command *c, program *_p) {
  variable *v = get_var(c, 0);
#ifdef ENABLE_EXCEPTIONS
  if (v == NULL) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Invalid Parameter(s)");
    c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif
  _p->_sleep = true;
  _p->_sleep_duration = (unsigned long)(ctod(v->data));
  _p->_sleep_start = millis();
  return 0;
}

int command_millis(command *c, program *_p) {
  variable *res;
  res = find_variable(c->args[0].data, c->pid);
  if (res == NULL) {
    res = (variable *)malloc(sizeof(variable));
    res->next = NULL;
    res->data = NULL;
    res->type = TYPE_STR;
    res->pid = c->pid;
    unsigned int nl = strlen(c->args[0].data) + 1;
    res->name = (char *)malloc(nl);
    memset(res->name, 0, nl);
    strcpy(res->name, c->args[0].data);
    new_variable(res);
  }
  if (res->data != NULL) {
    free(res->data);
  }
  res->data = dtoc(double(millis()));
  res->datasize = sizeof(double);
  res->deleted = false;
  res->type = TYPE_NUM;
  return 0;
}
