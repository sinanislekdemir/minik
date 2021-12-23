#include "source.hpp"
#include "program.hpp"
#include "tasks.hpp"

extern int serial_lock;
extern int kernel_next_pid;

SourceEngine *source_engine = new SourceEngine();

SourceEngine::SourceEngine() {
  this->serial_open = false;
  this->boud_rate = 9600;
  this->buffer_cursor = 0;
  this->source_size = 1; // '\0'
  this->buffer = (char *)malloc(BUFFER_SIZE);
  memset(this->buffer, 0, BUFFER_SIZE);
  this->source = (char *)malloc(1);
  memset(this->source, 0, 1);
}

void SourceEngine::create_task() {
  program *prog = new program(kernel_next_pid++);
  prog->source = this->source;
  prog->compile();
  add_task(prog, 1);
}

void SourceEngine::append_to_source() {
  if (strcmp(this->buffer, ".") == 0) {
    this->create_task();
    this->buffer_cursor = 0;
    memset(this->buffer, 0, BUFFER_SIZE);
    this->source_size = 1;
    this->source = (char *)malloc(1);
    memset(this->source, 0, 1);
    return;
  }
  if (strlen(this->buffer) < 2) {
    this->buffer_cursor = 0;
    memset(this->buffer, 0, BUFFER_SIZE);
    return;
  }
  int new_size = this->source_size + strlen(this->buffer) + 1;
  char *temp = (char *)malloc(new_size);
  memset(temp, 0, new_size);
  strcpy(temp, this->source);
  strcat(temp, this->buffer);
  strcat(temp, "\n");
  this->source_size = new_size;
  free(this->source);
  this->source = temp;
  memset(this->buffer, 0, BUFFER_SIZE);
  this->buffer_cursor = 0;
}

int SourceEngine::process() {
#ifdef BOARD_ESP32
  if (Serial.available() && !this->serial_open) {
    this->serial_open = true;
    Serial.println(MSG_READY_TO_RECEIVE);
  }
#endif
#ifdef BOARD_ATMEGA
  if (Serial && !this->serial_open) {
    this->serial_open = true;
    Serial.println(MSG_READY_TO_RECEIVE);
  }
#endif
  if (!this->serial_open)
    return 1;
  if (serial_lock > KERNEL_LOCK)
    return 1;
  if (!Serial.available())
    return 1;
  if (serial_lock == NO_LOCK) {
    serial_lock = KERNEL_LOCK;
  }
  char b = char(Serial.read());

  if (b == '\n') {
    serial_lock = -1; // release the lock
    Serial.print(b);
    append_to_source();
    return 1;
  }

  if (int(b) < 32) {
    return 1;
  }
  Serial.print(b);
  this->buffer[buffer_cursor++] = b;
  if (buffer_cursor == BUFFER_SIZE) {
    serial_lock = -1;
    append_to_source();
  }
  return 1;
}
