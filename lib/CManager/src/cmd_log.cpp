#include "cmd_log.hpp"

int output = LOG_SERIAL;

void set_output(int output_type) { output = output_type; }
int command_log(command *c, program *_p) {
  char *typestr = (char *)malloc(12);
  memset(typestr, 0, 12);
  if (output == LOG_SERIAL) {
    Serial.printf("Command: %s \n", c->command);
    for (uint i = 0; i < c->argc; i++) {
      Serial.printf("Arg %i:", i);
      Serial.print("    ");
      Serial.print(c->args[i].data);
      switch (c->args[i].type) {
      case TYPE_STR:
        sprintf(typestr, " (str)");
        break;
      case TYPE_NUM:
        sprintf(typestr, " (num)");
        break;
      case TYPE_CONSTANT:
        sprintf(typestr, " (const)");
        break;
      case TYPE_VARIABLE:
        sprintf(typestr, " (var)");
        break;
      case TYPE_LABEL:
        sprintf(typestr, " (lab)");
        break;
      case TYPE_REGISTER:
        sprintf(typestr, " (reg)");
        break;
      case TYPE_FILE:
        sprintf(typestr, " (file)");
        break;
      }
      Serial.println(typestr);
    }
  }
  free(typestr);
  return 0;
}
