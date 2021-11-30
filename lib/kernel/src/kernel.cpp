#include "kernel.hpp"

/**
   Main Kernel Entrypoint
 */
int kmain() {
  program *prog = new program(1);
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
  Serial.print("Platform ");
#ifdef BOARD_ESP32
  Serial.println("ESP32");
#endif
#ifdef BOARD_ATMEGA
  Serial.println("ATMEGA");
#endif
  Serial.println("Simba Kernel KMain");

  Serial.println("Ready to receive source code");
  Serial.flush();
#ifdef BOARD_ATMEGA
  Serial.print("Free ram: ");
  Serial.println(free_ram());
#endif
  prog->source = serial_get_multiline(MAX_LINE_LENGTH);
  Serial.println("Program source initialized");
  prog->compile();

  while (prog->step() == RUNNING) {
  }
#ifdef BOARD_ATMEGA
  Serial.print("Free ram: ");
  Serial.println(free_ram());
#endif
  delete prog;
#ifdef BOARD_ATMEGA
  Serial.print("Free ram after cleanup: ");
  Serial.println(free_ram());
#endif
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
  return result;
}
