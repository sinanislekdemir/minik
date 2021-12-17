#include "cmd_navigate.hpp"
#include "helpers.hpp"

int command_call(command *c, program *_p) {
  sub *s = _p->find_sub(c->args[0].name);
#ifdef ENABLE_EXCEPTIONS
  if (s == NULL) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "Call location [%s] not found", c->args[0].name);
    c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif
  _p->append_to_history(_p->cursor, c);
  _p->cursor = s;
  _p->cursor->cursor = _p->cursor->root_instruction;
  return 1; // don't change cursor
}

int command_goto(command *c, program *_p) {
#ifdef ENABLE_EXCEPTIONS
  if (c->argc < 1) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "GOTO target is not defined");
    c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif
  variable *goto_location = get_var(c, 0);
#ifdef ENABLE_EXCEPTIONS
  if (goto_location->type != TYPE_NUM) {
    char *msg = (char *)malloc(64);
    memset(msg, 0, 64);
    sprintf(msg, "GOTO target is not a valid NUMBER, got [%s]",
            type_tostr(goto_location->type));
    c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
    free(msg);
    return -1;
  }
#endif

  int line = int(ctod(goto_location->data));
  int counter = 0;
  command *node = _p->cursor->root_instruction;
  while (node != NULL) {
    if (counter == line) {
      _p->cursor->cursor = node;
      break;
    }
    counter++;
    node = node->next;
  }

  return 1;
}

int command_halt(command *c, program *_p) { return -1; }
