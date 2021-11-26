#include "kernel.hpp"

/**
   Main Kernel Entrypoint
 */
int kmain() {
  program *prog = new program();
  change_status_pin(STATUS_PIN);
  set_status(BOOT);
  breath();

  Serial.begin(9600);
  set_status(READY);
  while (!Serial) {
    breath();
  }
  set_status(SERIAL_PORT_OPENED);
  register_statements();
  breath();
  Serial.println("Simba Kernel KMain");

  Serial.println("Ready to receive source code");
  Serial.flush();

  prog->source = serial_get_multiline(MAX_LINE_LENGTH);
  Serial.println("Program source initialized");
  prog->compile();
  Serial.println("Program compiled");
  Serial.printf("Program size %d  bytes\n", sizeof(*prog));

  Serial.printf("# Subs %d\n", prog->sub_count());
  while (prog->step() == RUNNING) {
  }

  stop();
  return 0;
}

void stop() {
  if (Serial) {
    Serial.println("Simba shutdown");
  }
  set_status(STOP);
  while (1) {
    breath();
  }
}

char *kernel_stats(program *p) {
  char *result = (char *)malloc(1024);
  sprintf(result, "Free heap: %d\n", ESP.getFreeHeap());
  return result;
}
