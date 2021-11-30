#include "cmd_navigate.hpp"
#include "helpers.hpp"

int command_call(command *c, program *_p) {
  sub *s = _p->find_sub(c->args[0].name);
  if (s == NULL) {
    // TODO: raise error
    return -1;
  }
  _p->append_to_history(_p->cursor, c);
  _p->cursor = s;
  _p->cursor->cursor = _p->cursor->root_instruction;
  return 1; // don't change cursor
}

int command_goto(command *c, program *_p) {
  if (c->argc < 1) {
    // TODO: raise no goto found
    return -1;
  }
  variable *goto_location = get_var(c, 0);
  if (goto_location->type != TYPE_NUM) {
    // TODO: raise invalid instructon number;
    return -1;
  }

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
