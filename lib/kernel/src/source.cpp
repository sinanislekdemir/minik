#include "source.hpp"
#include "program.hpp"
#include "status.hpp"
#include "tasks.hpp"

extern int serial_lock;
extern int kernel_next_pid;

SourceEngine *source_engine = new SourceEngine();

SourceEngine::SourceEngine() {
	this->serial_open = false;
	this->boud_rate = 9600;
	this->buffer_cursor = 0;
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
	Serial.println(this->buffer);
	if (strcmp(this->buffer, ".") == 0) {
		this->create_task();

		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);

		this->source = (char *)malloc(1);
		memset(this->source, 0, 1);

		Serial.println(MSG_READY_TO_RECEIVE);
		serial_lock = NO_LOCK;
		return;
	}
	if (strcmp(this->buffer, "#ignore") == 0) {
		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);
		Serial.println(MSG_READY_TO_RECEIVE);
		return;
	}
	if (strlen(this->buffer) < 2) {
		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);
		return;
	}
	int new_size = strlen(this->source) + strlen(this->buffer) + 2;
	char *temp = (char *)malloc(new_size);
	memset(temp, 0, new_size);
	strcpy(temp, this->source);
	strcat(temp, this->buffer);
	strcat(temp, "\n");

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

	memset(this->buffer, 0, BUFFER_SIZE);
	Serial.readBytesUntil('\n', this->buffer, BUFFER_SIZE);
	append_to_source();
	serial_lock = -1;
	return 1;
}
