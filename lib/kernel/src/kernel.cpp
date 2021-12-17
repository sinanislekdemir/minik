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
#ifdef BOARD_ESP32
  while (!Serial.available()) {
    breath();
  }
#endif
#ifdef BOARD_ATMEGA
  while (!Serial) {
    breath();
  }
#endif
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
  Serial.print("Free ram: ");
  Serial.println(free_ram());
  Serial.println("ready to receive");
  prog->source = serial_get_multiline(MAX_LINE_LENGTH);
  Serial.println("Program source initialized");

  unsigned long start, compile, end;
  start = millis();

  prog->compile();
  compile = millis();
  Serial.print("Compile: ");
  Serial.print(compile - start);
  Serial.println(" mseconds");

  while (prog->step() == RUNNING) {
  }
  end = millis();

  Serial.print("Execution: ");
  Serial.print(end - compile);
  Serial.println(" mseconds");
  Serial.print("Free ram: ");
  Serial.println(free_ram());
  delete prog;
  Serial.print("Free ram after cleanup: ");
  Serial.println(free_ram());
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
