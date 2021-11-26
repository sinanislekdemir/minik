#ifndef _tokenizer_h_
#define _tokenizer_h_

#include <Arduino.h>
#include "command.hpp"

command parse(const char *cmd, uint pid);
void destroy_command(command c);
int validate_command(command *c, const char *cmd, int expected_argc);

#endif
