#include "cmd_logic.hpp"
#include "helpers.hpp"

int _quick_jump(command *c, program *_p) {
  sub *s = _p->find_sub(c->args[0].name);
  if (s == NULL) {
    // TODO: Raise error no sub
    return -1;
  }
  _p->cursor = s;
  _p->cursor->cursor = _p->cursor->root_instruction;
  return 1;
}

int command_cmp(command *c, program *_p) {
  if (c->argc != 2) {
    return -1;
  }
  variable *v1 = get_var(c, 0);
  variable *v2 = get_var(c, 1);
  if (v1->type != v2->type) {
    // TODO raise error
    return -1;
  }

  if (memcmp(v1->data, v2->data, v1->datasize) == 0) {
    _p->set_cmp_flag(CMP_JE);
    return 0;
  }

  if (v1->type == TYPE_NUM) {
    double d1 = ctod(v1->data);
    double d2 = ctod(v2->data);
    if (d1 > d2) {
      _p->set_cmp_flag(CMP_JG);
      return 0;
    }
    if (d1 < d2) {
      _p->set_cmp_flag(CMP_JL);
      return 0;
    }
  }
  _p->set_cmp_flag(CMP_N);
  return 0;
}

int command_je(command *c, program *_p) {
  if (_p->_cmp_flag != CMP_JE) {
    return 0;
  }
  return _quick_jump(c, _p);
}

int command_jne(command *c, program *_p) {
  if (_p->_cmp_flag == CMP_JE) {
    return 0;
  }
  return _quick_jump(c, _p);
}

int command_jg(command *c, program *_p) {
  if (_p->_cmp_flag != CMP_JG) {
    return 0;
  }
  return _quick_jump(c, _p);
}

int command_jge(command *c, program *_p) {
  if (_p->_cmp_flag == CMP_JE || _p->_cmp_flag == CMP_JG) {
    return _quick_jump(c, _p);
  }
  return 0;
}

int command_jl(command *c, program *_p) {
  if (_p->_cmp_flag != CMP_JL) {
    return 0;
  }
  return _quick_jump(c, _p);
}

int command_jle(command *c, program *_p) {
  if (_p->_cmp_flag == CMP_JE || _p->_cmp_flag == CMP_JL) {
    return _quick_jump(c, _p);
  }
  return 0;
}
