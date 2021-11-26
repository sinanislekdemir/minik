#include "cmd_navigate.hpp"
#include "helpers.hpp"

int command_call(command *c, program *_p) {
  int sc = _p->sub_cursor;
  int scc = _p->subs[sc].cursor;
  int sub = _p->find_sub(_p->subs[sc].instructions[scc].args[0].name);
  if (sub == -1) {
    // TODO: Raise error, no sub
    return -1;
  }
  _p->append_to_history(sc, scc + 1);
  _p->sub_cursor = sub;
  _p->subs[sub].cursor = 0;
  return 0;
}

int command_goto(command *c, program *_p) {
  int sc = _p->sub_cursor;
  int scc = _p->subs[sc].cursor;
  if (_p->subs[sc].instructions[scc].argc < 1) {
    // TODO: raise no goto found
    return -1;
  }

  double goto_location = 0;
  if (_p->subs[sc].instructions[scc].args[0].type == TYPE_VARIABLE) {
    variable *v =
        find_variable(_p->subs[sc].instructions[scc].args[0].data, _p->pid);
    if (v->type == TYPE_NUM) {
      goto_location = ctod(v->data);
    } else {
      // TODO: raise invalid goto type
      return -1;
    }
  }
  if (_p->subs[sc].instructions[scc].args[0].type == TYPE_NUM) {
    goto_location = ctod(_p->subs[sc].instructions[scc].args[0].data);
  }
  scc = int(goto_location);
  _p->subs[sc].cursor = scc;
  return 0;
}

int command_halt(command *c, program *_p) { return -1; }
