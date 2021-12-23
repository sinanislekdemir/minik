/**
 * Main Kernel Definitions
 */
#ifndef _kernel_hpp
#define _kernel_hpp
#include "tokenizer.hpp"
#include "status.hpp"
#include "serial.hpp"
#include "program.hpp"
#include "statement.hpp"
#include <Arduino.h>

struct setup {
  int num_cores;
  bool serial;
  bool net;
  bool sdcard;
};

void stop();
int kmain();

// Add kernel tasks like breath or serial console to tasks list
void register_kernel_tasks();

#endif
