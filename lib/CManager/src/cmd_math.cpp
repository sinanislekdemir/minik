#include "cmd_math.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "helpers.hpp"
#include "memory.hpp"
#include <math.h>
#include "tokenizer.hpp"

extern error err;

int save_result(command *c, double result) {
  variable *res;
  res = find_variable(c->args[0].data, c->pid);
  if (res == NULL) {
    err.code = ERR_VARIABLE_NOT_FOUND;
    err.pid = c->pid;
    return -1;
  }

  free(res->data);
  res->data = dtoc(result);
  res->datasize = sizeof(double);
  res->deleted = false;
  res->type = TYPE_NUM;
  return 0;
}

int command_add(command *c, program *_p) {
  if (validate_command(c, (const char *)"add", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double add = ctod(v1->data) + ctod(v2->data);
  return save_result(c, add);
}

int command_sub(command *c, program *_p) {
  if (validate_command(c, (const char *)"sub", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double sub = ctod(v1->data) - ctod(v2->data);
  return save_result(c, sub);
}

int command_div(command *c, program *_p) {
  if (validate_command(c, (const char *)"div", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double div = ctod(v1->data) / ctod(v2->data);
  return save_result(c, div);
}

int command_mul(command *c, program *_p) {
  if (validate_command(c, (const char *)"mul", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double mul = ctod(v1->data) * ctod(v2->data);
  return save_result(c, mul);
}

int command_xor(command *c, program *_p) {
  if (validate_command(c, (const char *)"xor", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double xord = int(ctod(v1->data)) % int(ctod(v2->data));
  return save_result(c, xord);
}

int command_or(command *c, program *_p) {
  if (validate_command(c, (const char *)"or", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double or_r = int(ctod(v1->data)) | int(ctod(v2->data));
  return save_result(c, or_r);
}

int command_and(command *c, program *_p) {
  if (validate_command(c, (const char *)"and", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double and_r = int(ctod(v1->data)) & int(ctod(v2->data));
  return save_result(c, and_r);
}

int command_pow(command *c, program *_p) {
  if (validate_command(c, (const char *)"pow", 3) != 0) {
    return -1;
  }

  variable *v1 = get_var(c, 1);
  variable *v2 = get_var(c, 2);

  if (v1 == NULL || v2 == NULL) {
    return -1;
  }

  double pow_r = pow(ctod(v1->data), ctod(v2->data));
  return save_result(c, pow_r);
}
