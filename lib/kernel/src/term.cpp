#include "term.hpp"

#include "memory.hpp"
#include "program.hpp"
#include "status.hpp"
#include "tasks.hpp"

extern variable root_variable;
extern int kernel_next_pid;

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
WiFiServer *s = NULL;
WiFiClient terminal_client;
#endif
#endif

#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
#endif
#endif

Term::Term() {
	this->out_device = OUT_SERIAL;
	this->baud_rate = 9600;
	this->port = 23;
	this->has_client = false;
	this->_is_in_readline = false;
	this->io_cursor = 0;
	this->program_buffer = NULL;
	this->program_buffer_size = 0;
	this->programming_mode = false;
	this->io_buffer = (char *)malloc(BUFFER_SIZE);
	this->io_cursor = 0;
	memset(this->io_buffer, 0, BUFFER_SIZE);
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	this->set_port(this->port);
	this->start_server();
#endif
#endif
}

Term::~Term() { free(this->io_buffer); }

void Term::set_baud_rate(unsigned long rate) {
	this->baud_rate = rate;
	if (this->out_device == OUT_SERIAL) {
		Serial.print("new-baud-rate: ");
		Serial.println(rate);
		Serial.end();
		Serial.begin(rate);
	}
}

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
void Term::start_server() {
	if (s != NULL) {
		s->end();
		free(s);
	}
	s = new WiFiServer(this->port);
	s->begin();
}
#endif
#endif

void Term::set_output(uint8_t out) { this->out_device = out; }

void Term::set_port(int port) { this->port = port; }

int Term::kprint(char *data) {
	if (this->out_device == OUT_SERIAL) {
		Serial.print((const char *)data);
		return 0;
	}
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		SerialBT.write((const uint8_t *)data, strlen(data));
#endif
#endif
		return 0;
	}

	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		if (terminal_client.connected())
			terminal_client.write((const char *)data, strlen(data));
#endif
#endif
	}

	return 0;
}

int Term::readline() {
	if (this->out_device == OUT_SERIAL) {
		if (!Serial.available()) {
			return 0;
		}
		char b = Serial.read();
		if (b > 31 && b < 127) {
			this->io_buffer[io_cursor++] = b;
			Serial.print(b);
		}
		if (b == '\n') {
			Serial.print('\n');
			return 1;
		}
		return 0;
	}
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		SerialBT.readBytesUntil('\n', this->io_buffer, BUFFER_SIZE);
#endif
#endif
		return 1;
	}
	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		terminal_client.readBytesUntil('\n', this->io_buffer, BUFFER_SIZE);
		return 1;
#endif
#endif
	}
	return 0;
}

void Term::append_program_buffer() {
	if (strlen(this->io_buffer) < 2) {
		return;
	}
	strcat(this->program_buffer, this->io_buffer);
	strcat(this->program_buffer, "\n");
}

int Term::process() {
	if (!this->wait_for_client()) {
		return 1;
	}
	if (!this->available()) {
		return 1;
	}
	int r = this->readline();
	if (r == 0) {
		return 1;
	}
	if (this->programming_mode) {
		if (strcmp(this->io_buffer, ".") == 0) {
			program *prog = new program(kernel_next_pid++);
			prog->source = this->program_buffer;
			prog->compile();

			Serial.println(this->program_buffer);
			Serial.println(free_ram());

			this->program_buffer_size = 0;
			add_task(prog, 1);

			this->programming_mode = false;
			memset(this->io_buffer, 0, BUFFER_SIZE);
			this->io_cursor = 0;
			return 1;
		}
		this->append_program_buffer();
		memset(this->io_buffer, 0, BUFFER_SIZE);
		this->io_cursor = 0;
		return 1;
	}
	if (strcmp(this->io_buffer, "free") == 0) {
		char *tmp = (char *)malloc(16);
		memset(tmp, 0, 16);
		int ram = free_ram();
		itoa(ram, tmp, 10);
		kprint((char *)"Free ram size: ");
		kprint(tmp);
		kprint((char *)"\n");
		free(tmp);
	}
	if (strcmp(this->io_buffer, "memdump") == 0) {
		variable *node = &root_variable;
		while (node != NULL) {
			char *tmp = (char *)malloc(16);
			memset(tmp, 0, 16);
			kprint((char *)"Name: ");
			kprint(node->name);
			kprint((char *)"\n  Type: ");
			itoa(node->type, tmp, 10);
			kprint(tmp);
			kprint((char *)"\n  Size: ");
			memset(tmp, 0, 16);
			itoa(node->datasize, tmp, 10);
			kprint((char *)"\n  PID: ");
			kprint(tmp);
			kprint((char *)"\n");
			node = node->next;
			free(tmp);
		}
	}
	if (strcmp(this->io_buffer, "program") == 0) {
		kprint((char *)"Programming mode activated\n");
		this->allocate_program_buffer();
		kprint((char *)"End programming with a dot (.) as a single line\n");
		kprint((char *)"ready to receive\n");
		this->programming_mode = true;
		memset(this->io_buffer, 0, BUFFER_SIZE);
		this->io_cursor = 0;
		return 1;
	}
	kprint((char *)"> ");
	memset(this->io_buffer, 0, BUFFER_SIZE);
	this->io_cursor = 0;
	return 1;
}

bool Term::wait_for_client() {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	if (terminal_client.connected()) {
		return true;
	}
	if (s != NULL && s->hasClient()) {
		terminal_client = s->available();
		this->set_port(23);
		this->set_output(OUT_WIFI);
		kprint((char *)"Minik Terminal\n> ");
		this->has_client = true;
		return true;
	}
#endif
#endif
	if (Serial.available() && !this->has_client) {
		this->has_client = true;
		this->set_output(OUT_SERIAL);
		kprint((char *)"Minik Terminal\n> ");
		return true;
	}
	if (this->out_device == OUT_SERIAL && !Serial) {
		this->has_client = false;
	}
	return this->has_client;
}

int Term::available() {
	if (!this->has_client) {
		return false;
	}
	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		return terminal_client.available();
#endif
#endif
	}
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		return SerialBT.available();
#endif
#endif
	}
	if (this->out_device == OUT_SERIAL) {
		return Serial.available();
	}
	return 0;
}

int Term::allocate_program_buffer() {
	char *tmp = (char *)malloc(16);
	memset(tmp, 0, 16);

	if (this->program_buffer_size != 0) {
		free(this->program_buffer);
		this->program_buffer = NULL;
                this->program_buffer_size = 0;
	}

	// only the half of free ram should be allocated;
#ifdef BOARD_ESP32
	this->program_buffer_size = 10 * 1024;
#else
	int fr = free_ram();
	this->program_buffer_size = fr / 2;
#endif

	while (this->program_buffer == NULL) {
		this->program_buffer = (char *)malloc(this->program_buffer_size);
		if (this->program_buffer == NULL) {
			this->program_buffer_size -= 100;
		}
	}

	itoa(this->program_buffer_size, tmp, 10);
	kprint((char *)"Allocated memory: ");
	kprint(tmp);
	kprint((char *)"\n");

	memset(this->program_buffer, 0, this->program_buffer_size);

	kprint((char *)"Allocated program buffer\n");
	free(tmp);
	return this->program_buffer_size;
}
