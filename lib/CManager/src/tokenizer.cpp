#include "tokenizer.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "helpers.hpp"

extern error err;

command parse(const char *cmd, uint pid) {
  command result;
  uint argument_count = argc(cmd, ' ');
  uint command_size = extract_size(cmd, ' ', 0);
  uint argument_size;
  result.command = extract(cmd, ' ', 0, command_size + 1);

  variable *args = new variable[argument_count];

  for (uint i = 0; i < argument_count - 1; i++) {
    argument_size = extract_size(cmd, ' ', i + 1);
    char *temp = extract(cmd, ' ', i + 1, argument_size + 1);

    args[i].type = arg_type(temp);
    args[i].pid = pid;
    args[i].deleted = false;
    args[i].name = (char *)malloc(strlen(temp) + 1);
    args[i].next = NULL;

    memset(args[i].name, 0, strlen(temp) + 1);
    strcpy(args[i].name, temp);

    if (args[i].type == TYPE_NUM) {
      args[i].datasize = sizeof(double);
      args[i].data = dtoc(atof(temp));
    } else {
      int lshift = 0;
      int rshift = 0;
      if (temp[0] == '"') {
        lshift = 1;
        rshift = 2;
      }
      args[i].data = (char *)malloc(argument_size + 1);
      memset(args[i].data, 0, argument_size + 1);
      memcpy(args[i].data, temp + lshift, argument_size - rshift);
      args[i].datasize = argument_size;
    }
    free(temp);
  }

  result.argc = argument_count - 1;
  result.args = args;
  result.pid = pid;
  return result;
}

void destroy_command(command c) {
  free(c.command);
  for (uint i = 0; i < c.argc; i++) {
    if (c.args[i].data != NULL)
      free(c.args[i].data);
    if (c.args[i].data != NULL)
      free(c.args[i].name);
  }
}

int validate_command(command *c, const char *cmd, int expected_argc) {
  if (c->argc < expected_argc) {
    err.code = ERR_NOT_ENOUGH_ARGUMENTS;
    err.pid = c->pid;
    return -1;
  }
  return 0;
}
